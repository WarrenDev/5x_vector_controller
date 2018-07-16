
/**
* ble_manager_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  BLE Manager service core implementation.
*
*/

#include "ble_manager_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/driver/uart/uart_driver.h>
#include <aef/embedded/system/system_core.h>

#include "ble_observer/ble_observer_core.h"
#include "ble_hci/hci_com.h"
#include "ble_hci/bcomdef.h"
#include "ble_hci/gap.h"

/**
* Service Manager interface routines.
*/
static service_status_t ble_manager_core_start (service_ctx_t* ctx);
static service_status_t ble_manager_core_restart (service_ctx_t* ctx);
static service_status_t ble_manager_core_stop (service_ctx_t* ctx);
static service_status_t ble_manager_core_status (service_ctx_t* ctx);
static service_status_t ble_manager_core_pause (service_ctx_t* ctx);
static service_status_t ble_manager_core_continue (service_ctx_t* ctx);

/**
* Internal routines.
*/
static service_status_t ble_manager_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_central_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_central_start (service_ctx_t* ctx);
static service_status_t ble_manager_core_peripheral_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_peripheral_start (service_ctx_t* ctx);
static service_status_t ble_manager_core_begin_scan (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_end_scan (service_ctx_t* ctx);
static service_status_t ble_manager_core_create_link (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_destroy_link (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_destroy_link_ex (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_find_service (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_disc_all_chars (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_read_char_value (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_read_blob_char_value (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_write_char_value (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_prepare_write_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_execute_write_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_get_bdaddr (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t ble_manager_core_set_bdaddr (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_make_discoverable (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_update_advert_data (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_end_discovery (service_ctx_t* ctx);
static service_status_t ble_manager_core_authenticate (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_passkey_update (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_slave_security_update_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_signable (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_bond (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_terminate_auth (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_update_link_parms_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_exchange_mtu_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_find_info_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_add_service (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_del_service (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_add_attribute (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_add_attribute128 (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_read_char_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_read_blob_char_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_write_char_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_find_by_type_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_error_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_np_reset (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_np_force_boot (service_ctx_t* ctx);

static service_status_t ble_manager_core_observer_attach (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t ble_manager_core_observer_detach (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);

static const stream_driver_vtable_t* ble_hci_uart_drv = NULL;
static stream_driver_ctx_t* uart_ctx = NULL;

/**
 * device_mac_addr
 */

static uint8_t device_mac_addr[B_ADDR_LEN];					//!< Device MAC adderss
/**
* Device connection management
*/
static uint16_t	connection_handle;							//!< Device connection handle
static uint8_t	connection_device_addr[B_ADDR_LEN];			//!< Device connection address

/**
* Profile Parameters identifiers
*/
ble_role_parms_t gap_role_parms;

/**
* Initialize the BLE Manager service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_manager_core_init (service_ctx_t* ctx)
{
	device_manager_vtable_t* device_manager = system_get_device_manager();
	ble_hci_uart_drv = device_manager->getdevice(DRV_IUART_D);

	if ( ble_hci_uart_drv != NULL )
	{
		ctx->state = SERVICE_START_PENDING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* De-initialize the BLE Manager service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_manager_core_deinit (service_ctx_t* ctx)
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
ble_manager_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( ctx->state == SERVICE_UNINITIALIZED )
		return SERVICE_FAILURE_INITIALIZATION;

	if ( ctx->state != SERVICE_RUNNING )
		return SERVICE_FAILURE_OFFLINE;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = ble_manager_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = ble_manager_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = ble_manager_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = ble_manager_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = ble_manager_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = ble_manager_core_continue(ctx);
			break;
		case IOCTL_BLE_MANAGER_INITIALIZE:
			status = ble_manager_core_initialize(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_CENTRAL_INIT:
			status = ble_manager_core_central_init(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_CENTRAL_START:
			status = ble_manager_core_central_start(ctx);
			break;
		case IOCTL_BLE_MANAGER_PERIPHERAL_INIT:
			status = ble_manager_core_peripheral_init(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_PERIPHERAL_START:
			status = ble_manager_core_peripheral_start(ctx);
			break;
		case IOCTL_BLE_MANAGER_BEGIN_SCAN:
			status = ble_manager_core_begin_scan(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_END_SCAN:
			status = ble_manager_core_end_scan(ctx);
			break;
		case IOCTL_BLE_MANAGER_CREATE_LINK:
			status = ble_manager_core_create_link(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_DESTROY_LINK:
			status = ble_manager_core_destroy_link(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_DESTROY_LINK_EX:
			status = ble_manager_core_destroy_link_ex(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_FIND_SERVICE:
			status = ble_manager_core_find_service(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_DISC_ALL_CHARS:
			status = ble_manager_core_disc_all_chars(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_READ_CHAR_VALUE:
			status = ble_manager_core_read_char_value(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_READ_CHAR_VALUE_RSP:
			status = ble_manager_core_read_char_value_rsp(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_READ_BLOB_CHAR_VALUE:
			status = ble_manager_core_read_blob_char_value(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_READ_BLOB_CHAR_VALUE_RSP:
			status = ble_manager_core_read_blob_char_value_rsp(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_WRITE_CHAR_VALUE:
			status = ble_manager_core_write_char_value(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_WRITE_CHAR_VALUE_RSP:
			status = ble_manager_core_write_char_value_rsp(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_PREPARE_WRITE_REQ:
			status = ble_manager_core_prepare_write_req(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_EXECUTE_WRITE_REQ:
			status = ble_manager_core_execute_write_req(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_GET_BDADDR:
			status = ble_manager_core_get_bdaddr(ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_BLE_MANAGER_SET_BDADDR:
			status = ble_manager_core_set_bdaddr (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_MAKE_DISCOVERABLE:
			status = ble_manager_core_make_discoverable (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_UPDATE_ADVERT_DATA:
			status = ble_manager_core_update_advert_data (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_END_DISCOVERY:
			status = ble_manager_core_end_discovery (ctx);
			break;
		case IOCTL_BLE_MANAGER_AUTHENTICATE:
			status = ble_manager_core_authenticate (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_PASSKEY_UPDATE:
			status = ble_manager_core_passkey_update (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_SLAVE_SECURITY_UPDATE_REQ:
			status = ble_manager_core_slave_security_update_req (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_SIGNABLE:
			status = ble_manager_core_signable (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_BOND:
			status = ble_manager_core_bond (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_TERMINATE_AUTH:
			status = ble_manager_core_terminate_auth (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_UPDATE_LINK_PARM_REQ:
			status = ble_manager_core_update_link_parms_req (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_EXCHANGE_MTU:
			status = ble_manager_core_exchange_mtu_req (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_FIND_INFO_REQ:
			status = ble_manager_core_find_info_req (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_FIND_BY_TYPE_VALUE_RSP:
			status = ble_manager_core_find_by_type_value_rsp (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_ADD_SERVICE:
			status = ble_manager_core_add_service (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_DEL_SERVICE:
			status = ble_manager_core_del_service (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_ADD_ATTRIBUTE:
			status = ble_manager_core_add_attribute (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_ADD_ATTRIBUTE_LONG:
			status = ble_manager_core_add_attribute128 (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_ERROR_RSP:
			status = ble_manager_core_error_rsp (ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_OBSERVER_ATTACH:
			status = ble_manager_core_observer_attach(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_OBSERVER_DETACH:
			status = ble_manager_core_observer_detach(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_NP_RESET:
			status = ble_manager_core_np_reset(ctx, input_buffer, input_size);
			break;
		case IOCTL_BLE_MANAGER_NP_FORCE_BOOT:
			status = ble_manager_core_np_force_boot(ctx);
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
service_status_t ble_manager_core_start (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state != SERVICE_RUNNING )
		{
			ctx->state = SERVICE_RUNNING;
			return SERVICE_STATUS_SUCCESS;
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
service_status_t ble_manager_core_restart (service_ctx_t* ctx)
{
	return SERVICE_STATUS_SUCCESS;
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
service_status_t ble_manager_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		ctx->state = SERVICE_STOPPED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INCORRECT_MODE;
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
service_status_t ble_manager_core_status (service_ctx_t* ctx)
{
	if ( ctx != NULL)
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
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
service_status_t ble_manager_core_pause (service_ctx_t* ctx)
{
	if ( ctx != NULL)
	{
		ctx->state = SERVICE_PAUSED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
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
service_status_t ble_manager_core_continue (service_ctx_t* ctx)
{
	if ( ctx != NULL)
	{
		ctx->state = SERVICE_RUNNING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Initialize the BLE Manager.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			connection_handle = 0xFFFF;							//!< Device connection handle
			memset ( connection_device_addr, 0, B_ADDR_LEN );	//!< Device connection address
			memset ( device_mac_addr, 0, B_ADDR_LEN );			//!< Device MAC address

			memset ( &gap_role_parms, 0, sizeof(ble_role_parms_t) );

			ble_observer_core_init();
			hci_com_initialize(ble_hci_uart_drv,uart_ctx);

			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize central role.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to memory area containing init parameters
* \param	input_size		Size of init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_central_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(central_role_parms_t) )
			{
				memcpy ( &gap_role_parms, input_buffer, input_size );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Start central role.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_central_start (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( gap_role_parms.peripheral.gapRole_ProfileRole == GAP_PROFILE_CENTRAL )
			{
				HciExtGAP_DeviceInit ( GAP_PROFILE_CENTRAL,
								   	   gap_role_parms.central.gapRole_MaxScanRes,
									   gap_role_parms.central.gapRole_IRK,
									   gap_role_parms.central.gapRole_SRK,
									   gap_role_parms.central.gapRole_SignCounter );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INITIALIZATION;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize peripheral role.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to memory area containing init parameters
* \param	input_size		Size of init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_peripheral_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(peripheral_role_parms_t) )
			{
				memcpy ( &gap_role_parms, input_buffer, input_size );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Start peripheral role.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_peripheral_start (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( gap_role_parms.peripheral.gapRole_ProfileRole == GAP_PROFILE_PERIPHERAL )
			{
				HciExtGAP_DeviceInit ( GAP_PROFILE_PERIPHERAL,
									   0,
									   gap_role_parms.peripheral.gapRole_IRK,
									   gap_role_parms.peripheral.gapRole_SRK,
									   gap_role_parms.peripheral.gapRole_SignCounter );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INITIALIZATION;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Begin device discovery (scanning)
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_begin_scan (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(discovery_parms_t) )
			{
				discovery_parms_t* parms = (discovery_parms_t*)input_buffer;
				HciExtGAP_DeviceDiscReq ( parms->mode, parms->scan_type, parms->white_list );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* End device discovery (scanning).
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_end_scan (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			HciExtGAP_DeviceDiscCancel ();
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Create device link.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_create_link (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(create_link_parms_t) )
			{
				create_link_parms_t* parms = (create_link_parms_t*)input_buffer;
				HciExtGAP_EstLinkReq ( parms->high_duty_cycle, parms->white_list, parms->addr_type, parms->addr );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Destroy device link.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing destroy link parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_destroy_link (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(destroy_link_parms_t) )
			{
				destroy_link_parms_t* parms = (destroy_link_parms_t*)input_buffer;
				HciExtGAP_TerminateLink( parms->handle );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Destroy device link extended.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing destroy link parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_destroy_link_ex (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(destroy_link_parms_t) )
			{
				destroy_link_parms_t* parms = (destroy_link_parms_t*)input_buffer;
				HciExtGAP_TerminateLinkEx ( parms->handle, parms->reason );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Find service.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing find service parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_find_service (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(find_service_parms_t) )
			{
				find_service_parms_t* parms = (find_service_parms_t*)input_buffer;
				HciExtGATT_FindPrimaryServiceByUUID ( parms->handle, parms->uuid, parms->length );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Discover all chars.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing discover characteristic parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_disc_all_chars (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(disc_all_chars_parms_t) )
			{
				disc_all_chars_parms_t* parms = (disc_all_chars_parms_t*)input_buffer;
				HciExtGATT_DiscoverAllChars ( parms->handle, parms->start_handle, parms->end_handle );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read char value.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing read characteristic parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_read_char_value (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(read_char_parms_t) )
			{
				read_char_parms_t* parms = (read_char_parms_t*)input_buffer;
				HciExtGATT_ReadCharValue ( parms->handle, parms->attr_handle );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read blob char value
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing read characteristic parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_read_blob_char_value (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(read_blob_char_parms_t) )
			{
				read_blob_char_parms_t* parms = (read_blob_char_parms_t*)input_buffer;
				HciExtGATT_ReadBlobCharValue ( parms->handle, parms->attr_handle, parms->offset );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Write char value.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing write characteristic parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_write_char_value (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(write_char_parms_t) )
			{
				write_char_parms_t* parms = (write_char_parms_t*)input_buffer;
				HciExtGATT_WriteCharValue ( parms->handle, parms->attr_handle, parms->length, parms->value );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Prepare write request
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing prepare write parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_prepare_write_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(prepare_write_req_parms_t) )
			{
				prepare_write_req_parms_t* parms = (prepare_write_req_parms_t*)input_buffer;
				HciExtGATT_PrepareWriteReq ( parms->handle, parms->attr_handle, parms->offset, parms->length, parms->value );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Execute write request
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing execute write parameters
* \param	input_size		Size of the memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_execute_write_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(execute_write_req_parms_t) )
			{
				execute_write_req_parms_t* parms = (execute_write_req_parms_t*)input_buffer;
				UNREFERENCED_PARAMETER(parms);
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Get bd address
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_get_bdaddr (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( output_buffer != NULL && output_size >= B_ADDR_LEN  )
			{
				memcpy (output_buffer, device_mac_addr, B_ADDR_LEN);
				if ( bytes_transferred )
					*bytes_transferred = B_ADDR_LEN;
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Set bd address
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the new address memory
* \param	input_size		Size of address memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_set_bdaddr (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(configure_device_addr_t) )
			{
				configure_device_addr_t* parms = (configure_device_addr_t*)input_buffer;
				HciExtGAP_ConfigDeviceAddr ( parms->addr_type, parms->addr );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Make device discoverable
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the discoverable parameter memory
* \param	input_size		Size of discoverable parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_make_discoverable (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(make_discoverable_parms_t) )
			{
				make_discoverable_parms_t* parms = (make_discoverable_parms_t*)input_buffer;
				HciExtGAP_MakeDiscoverable ( parms->event_type, parms->initiator_addr_type, parms->initiator_addr, parms->channel_map, parms->filter_policy );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Update advertisement data
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the advertisement data parameter memory
* \param	input_size		Size of advertisement data parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_update_advert_data (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(advert_data_parms_t) )
			{
				advert_data_parms_t* parms = (advert_data_parms_t*)input_buffer;
				HciExtGAP_UpdateAdvertData ( parms->ad_type, parms->length, parms->advert_data );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* End device discovery
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_end_discovery (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			HciExtGAP_EndDiscovery ();
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Authenticate
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the authenticate parameter memory
* \param	input_size		Size of authenticate parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_authenticate (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(authenticate_parms_t) )
			{
				authenticate_parms_t* parms = (authenticate_parms_t*)input_buffer;
				HciExtGAP_Authenticate ( parms->params, parms->pairReq );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Update the passkey
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the passkey update parameter memory
* \param	input_size		Size of passkey update parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_passkey_update (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(passkey_update_parms_t) )
			{
				passkey_update_parms_t* parms = (passkey_update_parms_t*)input_buffer;
				HciExtGAP_PasskeyUpdate	( parms->handle, parms->passkey );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Slave security update request
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the slave security update parameter memory
* \param	input_size		Size of slave security update parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_slave_security_update_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(slave_security_update_parms_t) )
			{
				slave_security_update_parms_t* parms = (slave_security_update_parms_t*)input_buffer;
				HciExtGAP_SlaveSecurityUpdateReq ( parms->handle, parms->authReq );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Signable
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the signable parameter memory
* \param	input_size		Size of signable parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_signable (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(signable_parms_t) )
			{
				signable_parms_t* parms = (signable_parms_t*)input_buffer;
				HciExtGAP_Signable ( parms->handle, parms->authenticated, parms->key, parms->signCounter );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Bond
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the bond parameter memory
* \param	input_size		Size of bond parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_bond (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(bond_parms_t) )
			{
				bond_parms_t* parms = (bond_parms_t*)input_buffer;
				HciExtGAP_Bond ( parms->handle, parms->authenticated, parms->ltk, parms->diversifier, parms->randomNum, parms->keySize );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Terminate authentication
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the terminate auth parameter memory
* \param	input_size		Size of terminate auth parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_terminate_auth (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(terminate_auth_parms_t) )
			{
				terminate_auth_parms_t* parms = (terminate_auth_parms_t*)input_buffer;
				HciExtGAP_Terminate_Auth ( parms->handle, parms->reason );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Update link parameters request
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the link  parameter memory
* \param	input_size		Size of link parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_update_link_parms_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(link_parms_t) )
			{
				link_parms_t* parms = (link_parms_t*)input_buffer;
				HciExtGAP_UpdateLinkParmReq ( parms->handle, parms->intervalMin, parms->intervalMax, parms->connectionLatency, parms->connectionTimeout );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Exchange MTU request
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the MTU parameter memory
* \param	input_size		Size of MTU parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_exchange_mtu_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mtu_parms_t) )
			{
				mtu_parms_t* parms = (mtu_parms_t*)input_buffer;
				HciExtGATT_ExchangeMTU ( parms->mtu );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Find info request
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the find into request parameter memory
* \param	input_size		Size of find info request parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_find_info_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(find_info_parms_t) )
			{
				find_info_parms_t* parms = (find_info_parms_t*)input_buffer;
				HciExtGATT_FindInfoReq ( parms->handle, parms->start_handle, parms->end_handle );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Add service request
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the add service request parameter memory
* \param	input_size		Size of add service request parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_add_service (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(add_service_parms_t) )
			{
				add_service_parms_t* parms = (add_service_parms_t*)input_buffer;
				HciExtGATT_AddService ( parms->uuid, parms->numOfAttrs );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Delete service request
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the delete service request parameter memory
* \param	input_size		Size of delete service request parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_del_service (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(del_service_parms_t) )
			{
				del_service_parms_t* parms = (del_service_parms_t*)input_buffer;
				HciExtGATT_DelService ( parms->uuid );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Add attribure
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the add attribute request parameter memory
* \param	input_size		Size of add attribute request parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_add_attribute (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(attribute_parms_t) )
			{
				attribute_parms_t* parms = (attribute_parms_t*)input_buffer;
				HciExtGATT_AddAttribute ( parms->uuid, parms->permissions );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Add attribure 128-bit
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the add attribute request parameter memory
* \param	input_size		Size of add attribute request parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_add_attribute128 (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(attribute128_parms_t) )
			{
				attribute128_parms_t* parms = (attribute128_parms_t*)input_buffer;
				HciExtGATT_AddAttribute128 ( parms->uuid, parms->permissions );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Read characteristic value response
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the read characteristic value response parameter memory
* \param	input_size		Size of read characteristic value response parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_read_char_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(read_char_value_rsp_parms_t) )
			{
				read_char_value_rsp_parms_t* parms = (read_char_value_rsp_parms_t*)input_buffer;
				HciExtGATT_ReadCharValueRsp ( parms->handle, parms->value, parms->length );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Read blob characteristic value response
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the read blob characteristic value response parameter memory
* \param	input_size		Size of read blob characteristic value response parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_read_blob_char_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(read_char_value_rsp_parms_t) )
			{
				read_char_value_rsp_parms_t* parms = (read_char_value_rsp_parms_t*)input_buffer;
				HciExtGATT_ReadBlobCharValueRsp ( parms->handle, parms->value, parms->length );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Write characteristic value response
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the write characteristic value response parameter memory
* \param	input_size		Size of write characteristic value response parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_write_char_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(write_char_value_rsp_parms_t) )
			{
				write_char_value_rsp_parms_t* parms = (write_char_value_rsp_parms_t*)input_buffer;
				HciExtGATT_WriteCharValueRsp ( parms->handle );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Find by type value response
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the find by type value response parameter memory
* \param	input_size		Size of find by type value response parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_find_by_type_value_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(find_by_type_value_rsp_parms_t) )
			{
				find_by_type_value_rsp_parms_t* parms = (find_by_type_value_rsp_parms_t*)input_buffer;
				HciExtGATT_FindByTypeValueRsp ( parms->handle, parms->info, parms->count );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Error response
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the error response parameter memory
* \param	input_size		Size of error response parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_error_rsp (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(error_rsp_parms_t) )
			{
				error_rsp_parms_t* parms = (error_rsp_parms_t*)input_buffer;
				HciExtATT_ErrorRsp ( parms->connectionHandle, parms->reqOpCode, parms->handle, parms->errorCode );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Network processor reset
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the reset parameter memory
* \param	input_size		Size of reset parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_np_reset (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(reset_parms_t) )
			{
				reset_parms_t* parms = (reset_parms_t*)input_buffer;
				HciExtUtil_Reset ( parms->resetType );
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
*  Network processor force boot
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_np_force_boot (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			HciExtUtil_ForceBoot ();
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Attach HCI message observer
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the HCI observer parameter memory
* \param	input_size		Size of HCI observer parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_observer_attach (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(hci_observer_parms_t) )
			{
				hci_observer_parms_t* parms = (hci_observer_parms_t*)input_buffer;
				return ble_observer_core_attach (parms->instance, parms->hci_msg, parms->noti);
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Detach HCI message observer
*
* \param    ctx				Pointer to the service context
* \param	input_buffer	Pointer to the HCI observer parameter memory
* \param	input_size		Size of HCI observer parameter memory
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
ble_manager_core_observer_detach (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(hci_observer_parms_t) )
			{
				hci_observer_parms_t* parms = (hci_observer_parms_t*)input_buffer;
				return ble_observer_core_detach (parms->hci_msg, parms->noti);
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

