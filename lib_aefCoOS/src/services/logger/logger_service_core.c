
/**
* logger_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Logger service core implementation.
*
*/

#include "logger_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines.
*/
static service_status_t logger_core_start (service_ctx_t* ctx);
static service_status_t logger_core_restart (service_ctx_t* ctx);
static service_status_t logger_core_stop (service_ctx_t* ctx);
static service_status_t logger_core_status (service_ctx_t* ctx);
static service_status_t logger_core_pause (service_ctx_t* ctx);
static service_status_t logger_core_continue (service_ctx_t* ctx);

static service_status_t logger_core_initialize (service_ctx_t* ctx, void* config);
static service_status_t logger_core_readevent (service_ctx_t* ctx, void* log_event);
static service_status_t logger_core_writeevent (service_ctx_t* ctx, void* log_event);
static service_status_t logger_core_deleteevent (service_ctx_t* ctx, void* log_eevnt);

/**
* Initialize the logger service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t logger_core_init (service_ctx_t* ctx)
{
	ctx->state = SERVICE_START_PENDING;
	return SERVICE_STATUS_SUCCESS;
}

/**
* De-initialize the logger service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t logger_core_deinit (service_ctx_t* ctx)
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
logger_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
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
			status = logger_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = logger_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = logger_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = logger_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = logger_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = logger_core_continue(ctx);
			break;
		case IOCTL_LOGGER_INITIALIZE:
			status = logger_core_initialize(ctx, input_buffer);
			break;
		case IOCTL_LOGGER_READ_EVENT:
			status = logger_core_readevent(ctx, output_buffer);
			break;
		case IOCTL_LOGGER_WRITE_EVENT:
			status = logger_core_writeevent(ctx, input_buffer);
			break;
		case IOCTL_LOGGER_DELETE_EVENT:
			status = logger_core_deleteevent(ctx, input_buffer);
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
service_status_t logger_core_start (service_ctx_t* ctx)
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
service_status_t logger_core_restart (service_ctx_t* ctx)
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
service_status_t logger_core_stop (service_ctx_t* ctx)
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
service_status_t logger_core_status (service_ctx_t* ctx)
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
service_status_t logger_core_pause (service_ctx_t* ctx)
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
service_status_t logger_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the hardware logger.
*
* \param    ctx				Pointer to the service context
* \param    config			Pointer to configuration information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
logger_core_initialize (service_ctx_t* ctx, void* config)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read a log event from log storage
*
* \param    ctx				Pointer to the service context
* \param	log_event		Poniter to memory for the log event
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
logger_core_readevent (service_ctx_t* ctx, void* log_event)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Write a log event to log storage
*
* \param    ctx				Pointer to the service context
* \param	log_event		Pointer to memory for the log event
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
logger_core_writeevent (service_ctx_t* ctx, void* log_event)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Delete a log event from log storage
*
* \param    ctx				Pointer to the service context
* \param	log_event		Pointer to memory for the log event
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
logger_core_deleteevent (service_ctx_t* ctx, void* log_event)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

