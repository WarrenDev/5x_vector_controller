
/**
* watchdog_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Watchdog service core implementation.
*
*/

#include "watchdog_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines.
*/
static service_status_t watchdog_core_start (service_ctx_t* ctx);
static service_status_t watchdog_core_restart (service_ctx_t* ctx);
static service_status_t watchdog_core_stop (service_ctx_t* ctx);
static service_status_t watchdog_core_status (service_ctx_t* ctx);
static service_status_t watchdog_core_pause (service_ctx_t* ctx);
static service_status_t watchdog_core_continue (service_ctx_t* ctx);

static service_status_t watchdog_core_initialize (service_ctx_t* ctx, uint32_t timeout);
static service_status_t watchdog_core_enable (service_ctx_t* ctx);
static service_status_t watchdog_core_disable (service_ctx_t* ctx);
static service_status_t watchdog_core_register (service_ctx_t* ctx, uint32_t* instance);
static service_status_t watchdog_core_update (service_ctx_t* ctx, uint32_t instance);
static void             watchdog_core_reboot (service_ctx_t* ctx);

static uint32_t watchdog_timeout;
static uint8_t  instance_index;
static uint8_t	watchdog_instance[WD_MAX_INSTANCES];

/**
* Initialize the watchdog service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t watchdog_core_init (service_ctx_t* ctx)
{
	return SERVICE_STATUS_SUCCESS;
}

/**
* De-initialize the watchdog service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t watchdog_core_deinit (service_ctx_t* ctx)
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
watchdog_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = watchdog_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = watchdog_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = watchdog_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = watchdog_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = watchdog_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = watchdog_core_continue(ctx);
			break;
		case IOCTL_WATCHDOG_INITIALIZE:
			status = watchdog_core_initialize(ctx, *((uint32_t*)input_buffer));
			break;
		case IOCTL_WATCHDOG_ENABLE:
			status = watchdog_core_enable(ctx);
			break;
		case IOCTL_WATCHDOG_DISABLE:
			status = watchdog_core_disable(ctx);
			break;
		case IOCTL_WATCHDOG_REGISTER:
			status = watchdog_core_register(ctx, (uint32_t*)output_buffer);
			break;
		case IOCTL_WATCHDOG_UPDATE:
			status = watchdog_core_update(ctx, *((uint32_t*)input_buffer));
			break;
		case IOCTL_WATCHDOG_REBOOT:
			watchdog_core_reboot(ctx);
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
service_status_t watchdog_core_start (service_ctx_t* ctx)
{
	ctx->state = SERVICE_START_PENDING;
	ctx->state = SERVICE_RUNNING;

	memset (watchdog_instance, 0, WD_MAX_INSTANCES);
	watchdog_timeout = WD_TIMEOUT_DEFAULT;
	instance_index = 0;

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
service_status_t watchdog_core_restart (service_ctx_t* ctx)
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
service_status_t watchdog_core_stop (service_ctx_t* ctx)
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
service_status_t watchdog_core_status (service_ctx_t* ctx)
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
service_status_t watchdog_core_pause (service_ctx_t* ctx)
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
service_status_t watchdog_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the hardware watchdog.
*
* \param    ctx				Pointer to the service context
* \param    timeout			Watchdog timeout value
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
watchdog_core_initialize (service_ctx_t* ctx, uint32_t timeout)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		watchdog_timeout = timeout;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Enable the hardware watchdog
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
watchdog_core_enable (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
	    ////////////////////////////////////////////////////////
	    // Unlock watchdog
//	    WDOG_UNLOCK = WD_UNLOCK_1;
//	    WDOG_UNLOCK = WD_UNLOCK_2;

	    ////////////////////////////////////////////////////////
	    // Enable watchdog
//	    WDOG_STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK | WDOG_STCTRLH_CLKSRC_MASK | WDOG_STCTRLH_ALLOWUPDATE_MASK;

	    ////////////////////////////////////////////////////////
	    // Set timeout
//	    WDOG_TOVALH = HIWORD(watchdog_timeout);
//	    WDOG_TOVALL = LOWORD(watchdog_timeout);

	    ////////////////////////////////////////////////////////
	    // Disable certain configuration updates
//	    WDOG_STCTRLH &= ~WDOG_STCTRLH_ALLOWUPDATE_MASK;

		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Disable the hardware watchdog
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
watchdog_core_disable (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
	    ////////////////////////////////////////////////////////
	    // Unlock watchdog
//	    WDOG_UNLOCK = WD_UNLOCK_1;
//	    WDOG_UNLOCK = WD_UNLOCK_2;

	    ////////////////////////////////////////////////////////
	    // Disable watchdog
//	    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;

		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Register a task or thread with the watchdog service.
* Returns a watchdog instance handle to the task.
*
* \param    ctx				Pointer to the service context
* \param	instance		Pointer to memory area to receive the instance handle
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
watchdog_core_register (service_ctx_t* ctx, uint32_t* instance)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING && instance_index < WD_MAX_INSTANCES )
	{
		*instance = instance_index;
		instance_index++;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Update watchdog instance.
*
* \param    ctx				Pointer to the service context
* \param	instance		Pointer to memory area to receive the instance handle
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
watchdog_core_update (service_ctx_t* ctx, uint32_t instance)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING && instance < WD_MAX_INSTANCES )
	{
		uint8_t index;
		watchdog_instance[instance] |= INSTANCE_REFRESH;

		/**
		 * Check to see if all registered watchdog instances have been refreshed
		 */
		for (index = 0; index < instance_index; index++)
		{
			if ( ! (watchdog_instance[index] & INSTANCE_REFRESH) )
				break;
		}

		/**
		 * If all registered watchdog instance have been refreshed, refresh the hardware
		 * watchdog and reset all instance refresh indicators.
		 */
		if ( index == instance_index )
		{
//			WDOG_REFRESH = WD_REFRESH_1;
//			WDOG_REFRESH = WD_REFRESH_2;
			for (index = 0; index < instance_index; index++)
			{
				watchdog_instance[index] &= ~INSTANCE_REFRESH;
			}
		}
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Reboot the MCU
*
* \param    ctx				Pointer to the service context
*
* \returns  None.
*/
void
watchdog_core_reboot (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
//	    WDOG_UNLOCK = WD_UNLOCK_2;
//	    WDOG_UNLOCK = WD_UNLOCK_1;
	    return;
	}
}


