
/**
* gpsd_service_core.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  GPSD service core implementation.
*
*/

#include "gpsd_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/driver/uart/uart_driver.h>
#include <aef/embedded/system/system_core.h>
#include <aef/embedded/system/system_management.h>
#include <aef/embedded/osal/event.h>
#include <aef/cutils/hexstring.h>
#include "string.h"
#include "stdlib.h"

/**
* Internal routines.
*/
static service_status_t gpsd_core_start (service_ctx_t* ctx);
static service_status_t gpsd_core_restart (service_ctx_t* ctx);
static service_status_t gpsd_core_stop (service_ctx_t* ctx);
static service_status_t gpsd_core_status (service_ctx_t* ctx);
static service_status_t gpsd_core_pause (service_ctx_t* ctx);
static service_status_t gpsd_core_continue (service_ctx_t* ctx);

static service_status_t gpsd_core_initialize (service_ctx_t* ctx, void* buffer, uint32_t length);
static service_status_t gpsd_core_enable_callback (service_ctx_t* ctx);
static service_status_t gpsd_core_disable_callback (service_ctx_t* ctx);
static service_status_t gpsd_core_poll (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_read);

static void gpsd_core_process_nmea (service_ctx_t* ctx, int8_t* NMEAStream, gps_fix_t* gps_fix_data);

static void NMEA_nmeaToDegreesLatLong ( gps_fix_t* gps_fix_data );
static uint8_t NMEA0183_ExtractData (int8_t* pGPSStream, gps_fix_t* pGPSData);
static void NMEA0183_ExtractGGA (int8_t* NMEAStream, gps_fix_t* gps_fix_data);
static void NMEA0183_ExtractRMC (int8_t* NMEAStream, gps_fix_t* gps_fix_data);

static bool gpsd_core_encode (char c);
static bool GetField (uint8_t *pData, uint8_t *pField, int nFieldNum, int nMaxFieldLen);

/**
* Service context variables
*/
static const stream_driver_vtable_t* gpsd_uart_drv = NULL;
static stream_driver_ctx_t* uart_ctx = NULL;
static device_driver_id_t uart_device_drv_id = DRV_IUART_B;
static gpsd_init_parms_t gpsd_parms;
static uint32_t baudRate = 9600;
static bool cb_enabled_flag = FALSE;
static event_ctx_t gpsd_event;

/**
* Local variables
*/
static char  GPSMid[10];
static int8_t  NMEAString[192];
//static int8_t  localBuffer[192];
static uint8_t ucData[30];
static uint8_t gpsd_ucNS;
static uint8_t gpsd_ucEW;
static uint16_t gpsd_encodedCharCount = 0;
static gps_fix_t gps_fix_data;

/**
* NMEA0183 message identifiers
*/
#define GPS_GGA_MSG             "GPGGA"         // Global positioning system fixed data
#define GPS_GLL_MSG             "GPGGL"         // Global position - Latitude/Longitude
#define GPS_RMC_MSG             "GPRMC"         // Recommended minimum specific BNSS data

#define GPS_MID_SIZE            6               // GPS MID identifier size
#define GPS_PREAMBLE            0xFA            // GPS packet identifier

/**
* GPGGA field identifier definitions
*/
#define GGAFIELD_UTC			1    			// (hhmmss.ss) UTC of Position
#define GGAFIELD_LATITUDE		2    			// (llll.ll) Latitude
#define GGAFIELD_NS				3    			// (a) N or S
#define GGAFIELD_LONGITUDE		4    			// (yyyyy.yy) Longitude
#define GGAFIELD_EW				5    			// (a) E or W
#define GGAFIELD_FIX			6    			// (x) GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
#define GGAFIELD_SAT			7    			// (xx) Number of satellites in use [not those in view]
#define GGAFIELD_HORZ			8    			// (x.x) Horizontal dilution of position
#define GGAFIELD_ALTITUDE		9    			// (x.x) Antenna altitude above/below mean sea level (geoid)
#define GGAFIELD_HEIGHT			10   			// (M) Meters  (Antenna height unit)
#define	GGAFIELD_GEOIDAL		11   			// (x.x) Geoidal separation (Diff. between WGS-84 earth ellipsoid and
       	   	   	   	   	   	   	   	   	   	   	//  mean sea level.  -=geoid is below WGS-84 ellipsoid)
#define GGAFIELD_UNITS			12   			// (M) Meters  (Units of geoidal separation)
#define GGAFEILD_AGE			13   			// (x.x) Age in seconds since last update from diff. reference station
#define GGAFEILD_REFERENCE		14   			// (xxxx) Diff. reference station ID#
#define GGAFIELD_CHECKSUM		15   			// Checksum

/**
* GPRMC field identifier definitions
*/
#define RMCFIELD_UTC			1    			// (tttttt) UTC of position fix
#define RMCFIELD_STATUS			2    			// (a) Data status (V=navigation receiver warning)
#define RMCFIELD_LATITUDE		3    			// (llll.ll) Latitude of fix
#define RMCFIELD_NS				4    			// (a) N or S
#define RMCFIELD_LONGITUDE		5    			// (yyyyy.yy) Longitude of fix
#define RMCFIELD_EW				6    			// (a) E or W
#define RMCFIELD_SPEED			7    			// (sss.s) Speed over ground in knots
#define RMCFIELD_TRACK			8    			// (ttt.t) Track made good in degrees True
#define RMCFIELD_DATE			9    			// (xxxxxx) UT date
#define RMCFIELD_MAGVAR			10   			// (vvv.v) Magnetic variation degrees (Easterly var. subtracts from true course)
#define RMCFIELD_MAGEW			11   			// (a) E or W
#define RMCFIELD_CHECKSUM		12   			// Checksum

/**
* GPSD service task
*
* This is a system management task routine.
* This routine is registered with the system management task.
* The system management task will call this routine to run the GPSD service
*
* \param    instance		Pointer to instance data
*
* \returns  none
*/
static void gpsd_service_task ( void* instance )
{
	if ( instance != NULL )
	{
		service_ctx_t* ctx = (service_ctx_t*)instance;

		/**
		* Read serial stream from GPS device
		*/
		uint32_t stream_size;
		int8_t stream_char;
		gpsd_uart_drv->iocontrol ( uart_ctx, IOCTL_UART_CHARPRESENT, NULL, 0, NULL, 0, &stream_size );

		while ( stream_size )
		{
			uint32_t bytes_read;
			gpsd_uart_drv->read ( uart_ctx, &stream_char, 1, &bytes_read );
			if ( gpsd_core_encode ( stream_char ) )
			{
				/**
				* Process NMEA string
				*/
				gpsd_core_process_nmea ( ctx, NMEAString, &gps_fix_data );
			}
			--stream_size;
		}
	}
	return;
}

/**
* Encode characters from the NMEA data stream.
*
* \param    c				Character to encode
*
* \returns  TRUE when a complete sentence has been received.
* 			FALSE if the sentence is not complete.
*/
bool gpsd_core_encode(char c)
{
	bool returnCode = FALSE;

	switch(c)
	{
		case '\n': // sentence end
			NMEAString[gpsd_encodedCharCount++] = c;
			NMEAString[gpsd_encodedCharCount] = '\0';
			returnCode = TRUE;
			break;

		case '$': // sentence begin
			gpsd_encodedCharCount = 0;
			NMEAString[gpsd_encodedCharCount++] = c;
			break;

		default: // ordinary characters
			NMEAString[gpsd_encodedCharCount++] = c;
			break;
	}

  return returnCode;
}

/**
* Initialize the GPSD service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_init (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		device_manager_vtable_t* device_manager = system_get_device_manager();
		gpsd_uart_drv = device_manager->getdevice(uart_device_drv_id);

		if ( gpsd_uart_drv )
		{
			ctx->state = SERVICE_START_PENDING;
			return SERVICE_STATUS_SUCCESS;
		}
	}
	ctx->state = SERVICE_UNINITIALIZED;
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* De-initialize the GPSD service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_deinit (service_ctx_t* ctx)
{
	ctx->state = SERVICE_DISABLED;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send a command to a system service instance.
*
* \param    ctx				Pointer to the service context
* \param    code			I/O control code to perform
* \param    input_buffer	Pointer to the input buffer
* \param    input_size		Input buffer size
* \param    output_buffer	Pointer to the output buffer
* \param    output_size		Output buffer size
* \param    bytes_read		Pointer to the actual bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
service_status_t
gpsd_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( ctx->state == SERVICE_UNINITIALIZED )
		return SERVICE_FAILURE_OFFLINE;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = gpsd_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = gpsd_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = gpsd_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = gpsd_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = gpsd_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = gpsd_core_continue(ctx);
			break;
		case IOCTL_GPSD_INITIALIZE:
			status = gpsd_core_initialize(ctx, input_buffer, input_size);
			break;
		case IOCTL_GPSD_ENABLE_CALLBACK:
			status = gpsd_core_enable_callback(ctx);
			break;
		case IOCTL_GPSD_DISABLE_CALLBACK:
			status = gpsd_core_disable_callback(ctx);
			break;
		case IOCTL_GPSD_POLL:
			status = gpsd_core_poll(ctx, output_buffer, output_size, bytes_transferred);
			break;
		default:
			break;
	}

	return status;
}

/**
* Start service control function.
* Starts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_start (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state != SERVICE_RUNNING )
		{
			ctx->state = SERVICE_START_PENDING;
			char* pname = gpsd_uart_drv->getname();

			uart_config_parms_t config;
			memset ( &gpsd_parms, 0, sizeof(gpsd_init_parms_t) );
			memset ( &gps_fix_data, 0, sizeof(gps_fix_t) );

//			event_create (&gpsd_event, "GPSD_EVENT", FALSE, FALSE);

			config.event_handle = &gpsd_event;
			config.baud         = baudRate;
			config.size		    = 8;
			config.parity       = 0;
			config.stop_bits    = 1;
			uart_ctx = gpsd_uart_drv->open(pname, UARTMODE_RAW, (uint32_t)&config );
			if (uart_ctx)
			{
				/**
				* Register the GPSD system management task
				*/
	            system_management_func_attach (ctx->name, ctx, gpsd_service_task);
				ctx->state = SERVICE_RUNNING;
				return SERVICE_STATUS_SUCCESS;
			}
			ctx->state = SERVICE_DISABLED;
			gpsd_encodedCharCount = 0;
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_INCORRECT_MODE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Restart service control function.
* Restarts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_restart (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_STOPPED )
		{
			return gpsd_core_start (ctx);
		}
		return SERVICE_FAILURE_INCORRECT_MODE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Stop service control function.
* Stops the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		/**
		* Deregister the GPSD system management task
		*/
//		event_destroy ( &gpsd_event );
        system_management_func_detach (gpsd_service_task);
		gpsd_uart_drv->close (uart_ctx);
		ctx->state = SERVICE_STOPPED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Status service control function.
* Retrieves status of the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_status (service_ctx_t* ctx)
{
	if ( ctx  != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Pause service control function.
* Pauses the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_pause (service_ctx_t* ctx)
{
	if ( ctx  != NULL )
	{
		ctx->state = SERVICE_PAUSED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Continue service control function.
* Continues the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_continue (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		ctx->state = SERVICE_RUNNING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the GPSD service.
*
* \param    ctx				Pointer to the service context
* \param	buffer			Pointer to initialization parameters
* \param	length			Size of initialization parameters
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_initialize (service_ctx_t* ctx, void* buffer, uint32_t length)
{
	if ( ctx != NULL && buffer != NULL && length == sizeof(gpsd_init_parms_t) )
	{
		gpsd_init_parms_t *parms = (gpsd_init_parms_t*)buffer;

		gpsd_parms.callback_interval = parms->callback_interval;
		gpsd_parms.instance          = parms->instance;
		gpsd_parms.cbfunc            = parms->cbfunc;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Enable GPSD service callback.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_enable_callback (service_ctx_t* ctx)
{
	cb_enabled_flag = TRUE;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Disable GPSD service callback.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_disable_callback (service_ctx_t* ctx)
{
	cb_enabled_flag = FALSE;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Poll the GPSD service.
*
* \param    ctx				Pointer to the service context
* \param	buffer			Pointer to GPSD service output data (NMEA or gpsData_t)
* \param	length			Size of output data
* \param	bytes_read		Pointer to the number of bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_poll (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_read)
{
	if ( ctx != NULL && buffer != NULL && length >= sizeof(gps_fix_t) )
	{
		memcpy ( buffer, &gps_fix_data, sizeof(gps_fix_t) );
		if ( bytes_read != NULL )
			*bytes_read = sizeof(gps_fix_t);
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Process NMEA sentence.
*
* \param    ctx				Pointer to the service context
* \param	NMEAStream		Pointer to NMEA string
* \param	gps_fix_data	Pointer to the GPS fix data structure
*
* \returns  none
*/
void gpsd_core_process_nmea (service_ctx_t* ctx, int8_t* NMEAStream, gps_fix_t* gps_fix_data)
{
	/**
	* Process NMEA strings
	*/
	NMEA0183_ExtractData ( NMEAStream, gps_fix_data );

	/**
	* Call callback
	*/
	if ( (cb_enabled_flag == TRUE) && (gpsd_parms.cbfunc != NULL) )
	{
		(*gpsd_parms.cbfunc)(gpsd_parms.instance);
	}
}

/**
* NMEA_nmeaToDegreesLatLong
*
* \brief 	Convert NMEA GPS latitude and longitude values to degrees
*
* \param   gps_fix_data			Pointer to the gps_fix_t structure
*
* \return  None
*/
void NMEA_nmeaToDegreesLatLong( gps_fix_t* gps_fix_data )
{
    int DD_lat = (int)( ( (double)gps_fix_data->latitude)/(double)100.0 );
    double SS_lat = ( (double)gps_fix_data->latitude ) - ( (double)DD_lat*(double)100.0 );
    gps_fix_data->latitude = (double)( (double)DD_lat + ( SS_lat/(double)60.0 ) );

    int DD_long = (int)( ( (double)gps_fix_data->longitude)/(double)100.0 );
    double SS_long = ( (double)gps_fix_data->longitude ) - ( (double)DD_long*(double)100.0 );
    gps_fix_data->longitude = (double)( (double)DD_long + ( SS_long/(double)60.0 ) );

    if ( gpsd_ucNS == 'S' )
       gps_fix_data->latitude *= (double)(-1.0);
    if ( gpsd_ucEW == 'W' )
       gps_fix_data->longitude *= (double)(-1.0);

} // NEMA_nemaToDegreesLatLong

/**
* Extract data from a NMEA string
*
* \param    NMEAStream			Pointer to the NMEA string
* \param	gps_fix_data		Pointer to the gps_fix_t data structure
*
* \returns  TRUE if successful.
*           FALSE if unsuccessful.
*/
uint8_t NMEA0183_ExtractData(int8_t* NMEAStream, gps_fix_t* gps_fix_data)
{
  uint8_t result = 0;

    /**
    * Extract the GPS NMEA0183 message type from the stream
    * Remove the original first octet of the stream ('$')
    */
    memset(GPSMid, '\0', sizeof(GPSMid));
    memcpy(GPSMid, NMEAStream + 1, GPS_MID_SIZE - 1);
    gpsd_ucEW = gpsd_ucNS = 0x78;

    /**
    * Parse the GPGGA message
    */
    if (memcmp(GPSMid, GPS_GGA_MSG, GPS_MID_SIZE - 1) == 0)
    {
    	NMEA0183_ExtractGGA(NMEAStream, gps_fix_data);
    } // if

    /**
    * Parse the GPGLL message
    */
    if (memcmp(GPSMid, GPS_GLL_MSG, GPS_MID_SIZE - 1) == 0)
    {
    } // if

    /**
    * Parse the GPRMC message
    */
    if (memcmp(GPSMid, GPS_RMC_MSG, GPS_MID_SIZE - 1) == 0)
    {
    	NMEA0183_ExtractRMC(NMEAStream, gps_fix_data);
    } // if

    return result;
}

/**
* Extract information from a GPGGA NMEA string
*
* \param    NMEAStream			Pointer to the NMEA string
* \param	gps_fix_data		Pointer to the gps_fix_t data structure
*
* \returns  none
*/
void NMEA0183_ExtractGGA(int8_t* NMEAStream, gps_fix_t* gps_fix_data)
{
    /**
    * Retrieve "Position Fix Indicator" from the NMEA stream
    */
    memset(ucData, '\0', sizeof(ucData));
    GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_FIX, sizeof(ucData));
    gps_fix_data->mode = 1 + ucData[0];

    if ( gps_fix_data->mode >= MODE_2D )
    {
        // N/S
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_NS, sizeof(ucData)))
            gpsd_ucNS = ucData[0];

        // E/W
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_EW, sizeof(ucData)))
            gpsd_ucEW = ucData[0];

        // UTC
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_UTC, sizeof(ucData)))
        	gps_fix_data->time = atof((const char *)ucData);

        // Latitude
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_LATITUDE, sizeof(ucData)))
        	gps_fix_data->latitude = atof((const char *)ucData);

        // Longitude
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_LONGITUDE, sizeof(ucData)))
        	gps_fix_data->longitude = atof((const char *)ucData);

        // Altitude
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_ALTITUDE, sizeof(ucData)))
        	gps_fix_data->altitude = atof((const char *)ucData);

        // Climb
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, GGAFIELD_HEIGHT, sizeof(ucData)))
        	gps_fix_data->climb = atof((const char *)ucData);

    	NMEA_nmeaToDegreesLatLong ( gps_fix_data );
    } // if
}

/**
* Extract information from a GPRMC NMEA string
*
* \param    NMEAStream			Pointer to the NMEA string
* \param	gps_fix_data		Pointer to the gps_fix_t data structure
*
* \returns  none
*/
void NMEA0183_ExtractRMC(int8_t* NMEAStream, gps_fix_t* gps_fix_data)
{
    if ( gps_fix_data->mode >= MODE_2D )
    {
        // Speed
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, RMCFIELD_SPEED, sizeof(ucData)))
        	gps_fix_data->speed = atof((const char *)ucData);

        // Track
        memset(ucData, '\0', sizeof(ucData));
        if (GetField((uint8_t*)NMEAStream, ucData, RMCFIELD_TRACK, sizeof(ucData)))
        	gps_fix_data->track = atof((const char *)ucData);
    }
}

/**
* Get a field from a NMEA string.
*
* \param    pData				Pointer to the NMEA string
* \param	pField				Pointer to field memory area
* \param	nFieldNum			Field number
* \param	nMaxFieldLen		Maximum field length
*
* \returns  TRUE if successful.
*           FALSE if unsuccessful.
*/
bool GetField(uint8_t *pData, uint8_t *pField, int nFieldNum, int nMaxFieldLen)
{
    /**
    * Validate params
    */
    if (pData == NULL || pField == NULL || nMaxFieldLen <= 0)
    {
        return FALSE;
    }

    /**
    * Go to the beginning of the selected field
    */
    int i = 0;
    int nField = 0;
    while (nField != nFieldNum && pData[i])
    {
        if (pData[i] == ',')
        {
            nField++;
        }

        i++;

        if (pData[i] == 0)       // was NULL
        {
            pField[0] = '\0';
            return FALSE;
        }
    }

    if (pData[i] == ',' || pData[i] == '*')
    {
        pField[0] = '\0';
        return FALSE;
    }

    /**
    * copy field from pData to Field
    */
    int i2 = 0;
    while (pData[i] != ',' && pData[i] != '*' && pData[i])
    {
        pField[i2] = pData[i];
        i2++; i++;

        /**
        * check if field is too big to fit on passed parameter. If it is,
        * crop returned field to its max length.
        */
        if (i2 >= nMaxFieldLen)
        {
            i2 = nMaxFieldLen - 1;
            break;
        }
    }
    pField[i2] = '\0';
    return TRUE;
}

