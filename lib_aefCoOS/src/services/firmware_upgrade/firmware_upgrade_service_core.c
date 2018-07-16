
/**
* firmware_upgrade_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  User Interface Manager service core implementation.
*
*/

#include "firmware_upgrade_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines.
*/
static service_status_t firmware_upgrade_core_start (service_ctx_t* ctx);
static service_status_t firmware_upgrade_core_restart (service_ctx_t* ctx);
static service_status_t firmware_upgrade_core_stop (service_ctx_t* ctx);
static service_status_t firmware_upgrade_core_status (service_ctx_t* ctx);
static service_status_t firmware_upgrade_core_pause (service_ctx_t* ctx);
static service_status_t firmware_upgrade_core_continue (service_ctx_t* ctx);

static service_status_t firmware_upgrade_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t firmware_upgrade_core_set_header (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t firmware_upgrade_core_begin (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t firmware_upgrade_core_add_chunk (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t firmware_upgrade_core_end (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t firmware_upgrade_core_validate_image (service_ctx_t* ctx);
static service_status_t firmware_upgrade_core_perform_upgrade (service_ctx_t* ctx);

//static device_driver_id_t hci_uart_drv_id = DRV_IUART_D;
//static const stream_driver_vtable_t* ble_hci_uart_drv = NULL;
//static stream_driver_ctx_t* uart_ctx = NULL;

/**
* Initialize the UI Manager service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t firmware_upgrade_core_init (service_ctx_t* ctx)
{
	device_manager_vtable_t* device_manager = system_get_device_manager();
//	ble_hci_uart_drv = device_manager->getdevice(hci_uart_drv_id);

//	return ( ble_hci_uart_drv == NULL ) ? SERVICE_FAILURE_GENERAL : SERVICE_STATUS_SUCCESS;
	ctx->state = SERVICE_START_PENDING;
	return SERVICE_STATUS_SUCCESS;
}

/**
* De-initialize the UI Manager service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t firmware_upgrade_core_deinit (service_ctx_t* ctx)
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
firmware_upgrade_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
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
			status = firmware_upgrade_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = firmware_upgrade_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = firmware_upgrade_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = firmware_upgrade_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = firmware_upgrade_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = firmware_upgrade_core_continue(ctx);
			break;
		case IOCTL_FW_UPGRADE_INITIALIZE:
			status = firmware_upgrade_core_initialize(ctx, input_buffer, input_size);
			break;
		case IOCTL_FW_UPGRADE_SET_HEADER:
			status = firmware_upgrade_core_set_header(ctx, input_buffer, input_size);
			break;
		case IOCTL_FW_UPGRADE_BEGIN:
			status = firmware_upgrade_core_begin(ctx, input_buffer, input_size);
			break;
		case IOCTL_FW_UPGRADE_ADD_CHUNK:
			status = firmware_upgrade_core_add_chunk(ctx, input_buffer, input_size);
			break;
		case IOCTL_FW_UPGRADE_END:
			status = firmware_upgrade_core_end(ctx, input_buffer, input_size);
			break;
		case IOCTL_FW_UPGRADE_VALIDATE_IMAGE:
			status = firmware_upgrade_core_validate_image(ctx);
			break;
		case IOCTL_FW_UPGRADE_PERFORM_UPGRADE:
			status = firmware_upgrade_core_perform_upgrade(ctx);
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
service_status_t firmware_upgrade_core_start (service_ctx_t* ctx)
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
service_status_t firmware_upgrade_core_restart (service_ctx_t* ctx)
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
service_status_t firmware_upgrade_core_stop (service_ctx_t* ctx)
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
service_status_t firmware_upgrade_core_status (service_ctx_t* ctx)
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
service_status_t firmware_upgrade_core_pause (service_ctx_t* ctx)
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
service_status_t firmware_upgrade_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the UI Manager.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Set the firmware upgrade control header.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing header
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_set_header (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( ctx != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Begin the firmware upgrade image download.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_begin (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( ctx != NULL && input_buffer != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Add a new firmware image chunk.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing the image chunk.
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_add_chunk (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( ctx != NULL && input_buffer != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* End the firmware upgrade process.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_end (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( ctx != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Validate firmware image.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_validate_image (service_ctx_t* ctx)
{
	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( ctx != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Perform the firmware upgrade.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
firmware_upgrade_core_perform_upgrade (service_ctx_t* ctx)
{
	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( ctx != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

