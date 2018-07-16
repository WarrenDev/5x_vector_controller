
/**
* console_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Console service core implementation.
*
*/

#include "console_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/driver/uart/uart_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines.
*/
static service_status_t console_core_start (service_ctx_t* ctx);
static service_status_t console_core_restart (service_ctx_t* ctx);
static service_status_t console_core_stop (service_ctx_t* ctx);
static service_status_t console_core_status (service_ctx_t* ctx);
static service_status_t console_core_pause (service_ctx_t* ctx);
static service_status_t console_core_continue (service_ctx_t* ctx);

static service_status_t console_core_readchar (service_ctx_t* ctx, uint8_t* puchar);
static service_status_t console_core_read (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_read);
static service_status_t console_core_readwait (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_read);
static service_status_t console_core_charpresent (service_ctx_t* ctx);
static service_status_t console_core_write (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_written);

static const stream_driver_vtable_t* console_uart_drv = NULL;
static stream_driver_ctx_t* uart_ctx = NULL;

/**
* Initialize the console service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t console_core_init (service_ctx_t* ctx)
{
	device_manager_vtable_t* device_manager = system_get_device_manager();
	console_uart_drv = device_manager->getdevice(DRV_IUART_A);

	if ( console_uart_drv )
	{
		ctx->state = SERVICE_START_PENDING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* De-initialize the console service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t console_core_deinit (service_ctx_t* ctx)
{
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
console_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
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
			status = console_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = console_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = console_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = console_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = console_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = console_core_continue(ctx);
			break;
		case IOCTL_CONSOLE_READ_CHAR:
			status = console_core_readchar(ctx, (uint8_t*)output_buffer);
			break;
		case IOCTL_CONSOLE_READ:
			status = console_core_read(ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_CONSOLE_READ_WAIT:
			status = console_core_readwait(ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_CONSOLE_CHAR_PRESENT:
			status = console_core_charpresent(ctx);
			break;
		case IOCTL_CONSOLE_WRITE:
			status = console_core_write (ctx, input_buffer, input_size, bytes_transferred);
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
service_status_t console_core_start (service_ctx_t* ctx)
{

	ctx->state = SERVICE_START_PENDING;

	ctx->state = SERVICE_RUNNING;
	return SERVICE_STATUS_SUCCESS;
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
service_status_t console_core_restart (service_ctx_t* ctx)
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
service_status_t console_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		console_uart_drv->close (uart_ctx);
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
service_status_t console_core_status (service_ctx_t* ctx)
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
service_status_t console_core_pause (service_ctx_t* ctx)
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
service_status_t console_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read a character.
*
* \param    ctx				Pointer to the service context
* \param    puchar			Pointer to memory area for the character read
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
console_core_readchar (service_ctx_t* ctx, uint8_t* puchar)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read a stream of characters.  Returns the characters currently available.
*
* \param    ctx				Pointer to the service context
* \param    buffer			Pointer to memory area for the characters read
* \param	length			Number of characters to read
* \param	bytes_read		Number of characters actually read
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
console_core_read (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_read)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read a stream of characters.  Blocks until the desired number of characters are received.
*
* \param    ctx				Pointer to the service context
* \param    buffer			Pointer to memory area for the characters read
* \param	length			Number of characters to read
* \param	bytes_read		Number of characters actually read
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
console_core_readwait (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_read)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Determine if a character is available to read
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
console_core_charpresent (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Write a stream of characters.
*
* \param    ctx				Pointer to the service context
* \param    buffer			Pointer to memory area for the characters read
* \param	length			Number of characters to write
* \param	bytes_written	Number of characters actually written
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
console_core_write (service_ctx_t* ctx, void* buffer, uint32_t length, uint32_t* bytes_written)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return console_uart_drv->write(uart_ctx, buffer, length, bytes_written );
	}
	return SERVICE_FAILURE_GENERAL;
}

