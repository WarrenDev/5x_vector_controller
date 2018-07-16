
/**
* secure_channel_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Secure channel service core implementation.
*
*/

#include "secure_channel_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines.
*/
static service_status_t secure_channel_core_start (service_ctx_t* ctx);
static service_status_t secure_channel_core_restart (service_ctx_t* ctx);
static service_status_t secure_channel_core_stop (service_ctx_t* ctx);
static service_status_t secure_channel_core_status (service_ctx_t* ctx);
static service_status_t secure_channel_core_pause (service_ctx_t* ctx);
static service_status_t secure_channel_core_continue (service_ctx_t* ctx);

static service_status_t secure_channel_core_initialize (service_ctx_t* ctx);
static service_status_t secure_channel_core_reset (service_ctx_t* ctx);
static service_status_t secure_channel_core_open (service_ctx_t* ctx, void* input_buffer, uint32_t input_length, void* output_buffer, uint32_t output_length, uint32_t* bytes_read);
static service_status_t secure_channel_core_close (service_ctx_t* ctx, void* input_buffer, uint32_t input_length, uint32_t* bytes_read);
static service_status_t secure_channel_core_read (service_ctx_t* ctx, void* input_buffer, uint32_t input_length, void* output_buffer, uint32_t output_length, uint32_t* bytes_read);
static service_status_t secure_channel_core_write (service_ctx_t* ctx, void* input_buffer, uint32_t input_length, void* output_buffer, uint32_t output_length, uint32_t* bytes_written);
static service_status_t secure_channel_core_sync (service_ctx_t* ctx, void* input_buffer, uint32_t input_length, uint32_t* bytes_transferred);

/**
* Initialize the console service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t secure_channel_core_init (service_ctx_t* ctx)
{

	if ( ctx )
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
service_status_t secure_channel_core_deinit (service_ctx_t* ctx)
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
secure_channel_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
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
			status = secure_channel_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = secure_channel_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = secure_channel_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = secure_channel_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = secure_channel_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = secure_channel_core_continue(ctx);
			break;
		case IOCTL_SECURE_CHANNEL_INITIALIZE:
			status = secure_channel_core_initialize(ctx);
			break;
		case IOCTL_SECURE_CHANNEL_RESET:
			status = secure_channel_core_reset(ctx);
			break;
		case IOCTL_SECURE_CHANNEL_OPEN:
			status = secure_channel_core_open(ctx, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_SECURE_CHANNEL_CLOSE:
			status = secure_channel_core_close(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_SECURE_CHANNEL_READ:
			status = secure_channel_core_read (ctx, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_SECURE_CHANNEL_WRITE:
			status = secure_channel_core_write (ctx, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_SECURE_CHANNEL_SYNC:
			status = secure_channel_core_sync (ctx, input_buffer, input_size, bytes_transferred);
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
service_status_t secure_channel_core_start (service_ctx_t* ctx)
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
service_status_t secure_channel_core_restart (service_ctx_t* ctx)
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
service_status_t secure_channel_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
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
service_status_t secure_channel_core_status (service_ctx_t* ctx)
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
service_status_t secure_channel_core_pause (service_ctx_t* ctx)
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
service_status_t secure_channel_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialzie the certificate manager service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
secure_channel_core_initialize(service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Reset the certificate manager service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
secure_channel_core_reset(service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Open a secure channel
*
* \param    ctx					Pointer to the service context
* \param    input_buffer		Pointer to memory area for the characters read
* \param	input_size			Number of characters to read
* \param    output_buffer		Pointer to memory area for the characters read
* \param	output_size			Number of characters to write
* \param	bytes_transferred	Number of characters actually read
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
secure_channel_core_open( service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Close a secure channel
*
* \param    ctx					Pointer to the service context
* \param    input_buffer		Pointer to memory area for the characters read
* \param	input_size			Number of characters to read
* \param	bytes_transferred	Number of characters actually read
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
secure_channel_core_close( service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Perform a read on a secure channel
*
* \param    ctx					Pointer to the service context
* \param    input_buffer		Pointer to memory area for the characters read
* \param	input_size			Number of characters to read
* \param    output_buffer		Pointer to memory area for the characters read
* \param	output_size			Number of characters to write
* \param	bytes_transferred	Number of characters actually read
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
secure_channel_core_read( service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Perform a write on a secure channel
*
* \param    ctx					Pointer to the service context
* \param    input_buffer		Pointer to memory area for the characters read
* \param	input_size			Number of characters to read
* \param    output_buffer		Pointer to memory area for the characters read
* \param	output_size			Number of characters to write
* \param	bytes_transferred	Number of characters actually read
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
secure_channel_core_write( service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Perform a sync operation on a secure channgle
*
* \param    ctx					Pointer to the service context
* \param    input_buffer		Pointer to memory area for the characters read
* \param	input_size			Number of characters to write
* \param    output_buffer		Pointer to memory area for the characters read
* \param	output_size			Number of characters to write
* \param	bytes_transferred	Number of characters actually written
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
secure_channel_core_sync( service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

