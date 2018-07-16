
/**
* devicecom_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Device communications service core implementation.
*
*/

#include "devicecom_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/driver/uart/uart_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Packet layout -
*
* ---------------------------------------------------------------------------
* ||  SOF   |  Sequence   |  Size   |  Packet Type  |  Payload   |   CRC   ||
* || 8 bits |   8 bits    | 16 bits |     8 bits    |  Size - 3  | 16 bits ||
* ---------------------------------------------------------------------------
*/

/**
* Misc. identifier definitions
*/
#define	DEVICECOM_BUFFER_SIZE			128			// Packet buffer size
#define DEVICECOM_SOF					0xDA		// Packet start-of-frame
#define DEVICECOM_XMITTIMEOUT			256L		// Transmit timeout (ms)
#define DEVICECOM_RECVTIMEOUT			256L		// Receive timeout (ms)
#define DEVICECOM_INITIALSIZE			3			// Initial size
#define DEVICECOM_MINPACKETSIZE			7			// Packet size without payload

/**
* Internal routines.
*/
static service_status_t devicecom_core_start (service_ctx_t* ctx);
static service_status_t devicecom_core_restart (service_ctx_t* ctx);
static service_status_t devicecom_core_stop (service_ctx_t* ctx);
static service_status_t devicecom_core_status (service_ctx_t* ctx);
static service_status_t devicecom_core_pause (service_ctx_t* ctx);
static service_status_t devicecom_core_continue (service_ctx_t* ctx);

static service_status_t devicecom_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t devicecom_core_reset (service_ctx_t* ctx);
static service_status_t devicecom_core_read (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t devicecom_core_write (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferrred);
static service_status_t devicecom_recv_packet (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t devicecom_send_packet (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred);

static uint32_t devicecom_core_create_packet ( uint8_t* packet, uint8_t packet_type, uint8_t* payload, uint16_t payload_size );
static uint32_t devicecom_core_get_packet ( uint8_t* packet );
static uint16_t	crc16 ( uint8_t* data_p, uint16_t length );

static const stream_driver_vtable_t* devicecom_uart_drv = NULL;
static stream_driver_ctx_t* uart_ctx = NULL;
static uint8_t message_buffer[DEVICECOM_BUFFER_SIZE];
static uint8_t sequence;

/**
* Initialize the devicecom service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t devicecom_core_init (service_ctx_t* ctx)
{
	device_manager_vtable_t* device_manager = system_get_device_manager();
	devicecom_uart_drv = device_manager->getdevice(DRV_IUART_C);

	if ( devicecom_uart_drv != NULL )
	{
		ctx->state = SERVICE_START_PENDING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* De-initialize the devicecom service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t devicecom_core_deinit (service_ctx_t* ctx)
{
	return SERVICE_STATUS_SUCCESS;
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
devicecom_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = devicecom_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = devicecom_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = devicecom_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = devicecom_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = devicecom_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = devicecom_core_continue(ctx);
			break;
		case IOCTL_DEVICE_COM_INITIALIZE:
			status = devicecom_core_initialize(ctx, input_buffer, input_size);
			break;
		case IOCTL_DEVICE_COM_RESET:
			status = devicecom_core_reset(ctx);
			break;
		case IOCTL_DEVICE_COM_READ:
			status = devicecom_core_read(ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_DEVICE_COM_WRITE:
			status = devicecom_core_write(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_DEVICE_COM_RECV_PACKET:
			status = devicecom_recv_packet (ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_DEVICE_COM_SEND_PACKET:
			status = devicecom_send_packet (ctx, input_buffer, input_size, bytes_transferred);
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
service_status_t devicecom_core_start (service_ctx_t* ctx)
{

	ctx->state = SERVICE_START_PENDING;
//	pname = devicecom_uart_drv->getname();
//	uart_ctx = devicecom_uart_drv->open(pname, 0, flag );
//	if (uart_ctx)
//	{
//		devicecom_uart_drv->iocontrol(uart_ctx, IO_IOCTL_SERIAL_SET_BAUD, &baud, 4, NULL, 0, NULL );
//		devicecom_uart_drv->iocontrol(uart_ctx, IO_IOCTL_SERIAL_SET_FLAGS, &flag, 4, NULL, 0, NULL );
//		devicecom_uart_drv->iocontrol(uart_ctx, IO_IOCTL_SERIAL_SET_DATA_BITS, &data_bits, 4, NULL, 0, NULL );
//		devicecom_uart_drv->iocontrol(uart_ctx, IO_IOCTL_SERIAL_SET_PARITY, &parity, 4, NULL, 0, NULL );
//		devicecom_uart_drv->iocontrol(uart_ctx, IO_IOCTL_SERIAL_SET_STOP_BITS, &stop_bits, 4, NULL, 0, NULL );

//		sequence = 0;
//		ctx->state = SERVICE_RUNNING;
//		return SERVICE_STATUS_SUCCESS;
//	}

	ctx->state = SERVICE_DISABLED;
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
service_status_t devicecom_core_restart (service_ctx_t* ctx)
{
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
service_status_t devicecom_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		devicecom_uart_drv->close (uart_ctx);
		ctx->state = SERVICE_STOPPED;
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
service_status_t devicecom_core_status (service_ctx_t* ctx)
{
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
service_status_t devicecom_core_pause (service_ctx_t* ctx)
{
	ctx->state = SERVICE_PAUSED;
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
service_status_t devicecom_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the hardware devicecom.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to configuration information
* \param	input_size		Size of the configuration information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
devicecom_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Reset the Essex communications service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
devicecom_core_reset (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read data from the UART
*
* \param    ctx					Pointer to the service context
* \param	output_abuffer		Pointer to the response buffer
* \param	output_size			Size of the response buffer
* \param	bytes_transferred	Pointer to the number of bytes received
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
devicecom_core_read (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Write data over the UART
*
* \param    ctx					Pointer to the service context
* \param	input_buffer		Pointer to the data to write
* \param	input_size			Size of the data to write
* \param	bytes_transferred	Pointer to the number of bytes received
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
devicecom_core_write (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Receive a protocol packet from the UART
*
* \param    ctx					Pointer to the service context
* \param	output_buffer		Pointer to the buffer to receive the data
* \param	output_size			Size of the buffer
* \param	bytes_transferred	Pointer to the number of bytes received
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
devicecom_recv_packet (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( output_buffer == NULL || output_size < sizeof(device_com_response_t) )
			return SERVICE_FAILURE_INVALID_PARAMETER;

		device_com_response_t* response = (device_com_response_t*)output_buffer;

		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send a protocol packet over the UART
*
* \param    ctx					Pointer to the service context
* \param	input_buffer		Pointer to the packet data to write
* \param	input_size			Size of the data to write
* \param	bytes_transferred	Pointer to the number of bytes received
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if input_buffer or input_size are incorrect.
*/
service_status_t
devicecom_send_packet (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( input_buffer == NULL || input_size != sizeof(device_com_message_t) )
			return SERVICE_FAILURE_INVALID_PARAMETER;

		device_com_message_t* message = (device_com_message_t*)input_buffer;
		uint32_t length = devicecom_core_create_packet ( message_buffer, message->message_type, message->message_data, message->message_size );
		return devicecom_uart_drv->write (uart_ctx, message_buffer, length, bytes_transferred );
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Retrieve a protocol packet from the UART
*
* \param	packet				Pointer to the packet buffer
*
* \returns  Length of the protocol packet
*/
uint32_t devicecom_core_get_packet ( uint8_t* packet )
{
uint32_t total_bytes = 0L;
uint32_t bytes_to_read;
uint32_t bytes_available = 0L;
uint16_t checksum;
uint8_t* buffer;

	buffer = packet;

	/**
	* Find the STX byte
	*/
	do
	{
		devicecom_uart_drv->iocontrol (uart_ctx, IOCTL_UART_CHARPRESENT, NULL, 0, NULL, 0, &bytes_available);
		if ( !bytes_available )
			return bytes_available;

		devicecom_uart_drv->iocontrol (uart_ctx, IOCTL_UART_GETCHAR, NULL, 0, buffer, 1, &bytes_available);
	} while ( *buffer != DEVICECOM_SOF );

	total_bytes++;

	/**
	* Retrieve the balance of the message header
	*/
	bytes_available = 0L;
	devicecom_uart_drv->read (uart_ctx, buffer+total_bytes, 4, &bytes_available);
	if ( !bytes_available )
		return 0L;

	/**
	* Update total bytes read
	* Get the number of bytes to read to retrieve the balance of the packet
	*/
	total_bytes += bytes_available;
	bytes_to_read = (uint32_t) MAKEWORD(*(buffer+3),*(buffer+2));

	/**
	* Read the balance of the packet
	*/
	bytes_available = 0L;
	devicecom_uart_drv->read (uart_ctx, buffer+total_bytes, bytes_to_read, &bytes_available);
	if ( !bytes_available )
		return 0L;

	total_bytes += bytes_available;

	/**
	* Compute the checksum - TODO
	*/
	checksum = 0;

	/**
	* Return the packet length
	*/
	return total_bytes;
}

/**
* Create a protocol packet
*
* \param	packet				Pointer to the packet buffer
* \param	packet_type			Packet type
* \param	payload				Pointer to the packet payload
* \param	payload_size		Size of the payload
*
* \returns  Length of the protocol packet
*/
uint32_t devicecom_core_create_packet ( uint8_t* packet, uint8_t packet_type, uint8_t* payload, uint16_t payload_size )
{
	if ( packet == NULL || (payload_size && payload == NULL) )
		return 0;

	uint8_t* buffer = packet;
	uint16_t packet_size = DEVICECOM_INITIALSIZE + payload_size;

	/**
	 * Load header
	 */
	*(uint8_t*)buffer++ = DEVICECOM_SOF;
	*(uint8_t*)buffer++ = ++sequence;
	*(uint8_t*)buffer++ = LOBYTE(packet_size);
	*(uint8_t*)buffer++ = HIBYTE(packet_size);
	*(uint8_t*)buffer++ = packet_type;

	/**
	 * Load payload
	 */
	if ( payload_size != 0 )
		memcpy ( buffer, payload, payload_size );
	buffer += payload_size;

	/*
	 * Load checksum
	 */
	uint16_t crc = crc16 ( packet+1, payload_size+4 );
	*(uint8_t*)buffer++ = LOBYTE(crc);
	*(uint8_t*)buffer   = HIBYTE(crc);

	/**
	 * Return packet length
	 */
	return ( payload_size + DEVICECOM_MINPACKETSIZE );
}

/**
* crc16
*                                        16   12   5
*   This is the CCITT CRC 16 polynomial X  + X  + X  + 1.
*   This works out to be 0x1021, but the way the algorithm works
*   lets us use 0x8408 (the reverse of the bit pattern).  The high
*   bit is always assumed to be set, thus we only use 16 bits to
*   represent the 17 bit value.
*/
#define POLY 0x8408
uint16_t crc16( uint8_t* data_p, uint16_t length )
{
	uint8_t i;
	uint16_t data;
	uint16_t crc = 0xffff;

	if (length == 0)
		return (~crc);

	do
	{
		for (i=0, data=(uint16_t)0xff & *data_p++;
			 i < 8;
             i++, data >>= 1)
		{
			if ((crc & 0x0001) ^ (data & 0x0001))
				crc = (crc >> 1) ^ POLY;
			else  crc >>= 1;
		}
	} while (--length);

	crc = ~crc;
	data = crc;
	crc = (crc << 8) | (data >> 8 & 0xff);

	return (crc);
} // crc16

