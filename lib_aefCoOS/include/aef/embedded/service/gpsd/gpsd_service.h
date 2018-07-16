
/**
* gpsd_service.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system GPSD service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_GPSD_GPSD_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_GPSD_GPSD_SERVICE_H_

#include <aef/embedded/service/srvddk/uefsrvddk.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
* The IOCTL function codes from 0x800 to 0xFFF are for customer use.
*
* The format of I/O control codes for the iocontrol call:
*    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method)
*
* Configuring system service I/O control codes in this manner produces
* unique system-wide I/O control codes.  It also provides a mechanism for
* isolating kernel mode memory spaces from user mode memory spaces and
* allows us to use the memory protection unit (MPU) of the processor.
*/

/**
* GPSD base function codes
*/
#define GPSD_INITIALIZE				0x800
#define GPSD_ENABLE_CALLBACK		0x801
#define GPSD_DISABLE_CALLBACK		0x802
#define GPSD_POLL					0x803

/**
* GPSD service I/O Control codes
*/
#define IOCTL_GPSD_INITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_GPSD,GPSD_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_GPSD_ENABLE_CALLBACK	SRVIOCTLCODE(SERVICE_TYPE_GPSD,GPSD_ENABLE_CALLBACK,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_GPSD_DISABLE_CALLBACK	SRVIOCTLCODE(SERVICE_TYPE_GPSD,GPSD_DISABLE_CALLBACK,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_GPSD_POLL				SRVIOCTLCODE(SERVICE_TYPE_GPSD,GPSD_POLL,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

/**
* Function pointer definition for GPSD callback function
*/
typedef void (*gpsd_callback_func_t) (void* instance);

/**
* GPSD initialization parameter structure definition
*/
typedef struct _gpsd_init_parms_def
{
	uint32_t callback_interval;
	void* instance;
	gpsd_callback_func_t cbfunc;
} gpsd_init_parms_t;


/**
* The structure describing an uncertainty volume in kinematic space.
* This is what GPSes are meant to produce; all the other info is
* technical impedimenta.
*
* All double values use NAN to indicate data not available.
*
* Usually all the information in this structure was considered valid
* by the GPS at the time of update.  This will be so if you are using
* a GPS chipset that speaks SiRF binary, Garmin binary, or Zodiac binary.
* This covers over 80% of GPS products in early 2005.
*
* If you are using a chipset that speaks NMEA, this structure is updated
* in bits by GPRMC (lat/lon, track, speed), GPGGA (alt, climb), GPGLL
* (lat/lon), and GPGSA (eph, epv).  Most NMEA GPSes take a single fix
* at the beginning of a 1-second cycle and report the same timestamp in
* GPRMC, GPGGA, and GPGLL; for these, all info is guaranteed correctly
* synced to the time member, but you'll get different stages of the same
* update depending on where in the cycle you poll.  A very few GPSes,
* like the Garmin 48, take a new fix before more than one of of
* GPRMC/GPGGA/GPGLL during a single cycle; thus, they may have different
* timestamps and some data in this structure can be up to 1 cycle (usually
* 1 second) older than the fix time.
*
* Error estimates are at 95% confidence.
*/
typedef double timestamp_t;	/* Unix time in seconds with fractional part */

typedef struct gps_fix_def
{
    timestamp_t time;		/* Time of update */
    int    mode;			/* Mode of fix */
#define MODE_NOT_SEEN	0	/* mode update not seen yet */
#define MODE_NO_FIX	1		/* none */
#define MODE_2D  	2		/* good for latitude/longitude */
#define MODE_3D  	3		/* good for altitude/climb too */
    double ept;				/* Expected time uncertainty */
    double latitude;		/* Latitude in degrees (valid if mode >= 2) */
    double epy;  			/* Latitude position uncertainty, meters */
    double longitude;		/* Longitude in degrees (valid if mode >= 2) */
    double epx;  			/* Longitude position uncertainty, meters */
    double altitude;		/* Altitude in meters (valid if mode == 3) */
    double epv;  			/* Vertical position uncertainty, meters */
    double track;			/* Course made good (relative to true north) */
    double epd;				/* Track uncertainty, degrees */
    double speed;			/* Speed over ground, meters/sec */
    double eps;				/* Speed uncertainty, meters/sec */
    double climb;       	/* Vertical speed, meters/sec */
    double epc;				/* Vertical speed uncertainty */
} gps_fix_t;

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_GPSD_GPSD_SERVICE_H_ */
