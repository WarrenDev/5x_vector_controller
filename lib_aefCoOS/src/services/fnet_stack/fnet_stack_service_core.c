
/**
* fnet_stack_service_core.c
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  FNET Network Stack service core implementation.
*
*/

#include "fnet_stack_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>
#include <aef/embedded/system/system_management.h>
#include <aef/embedded/system/system_property_manager.h>
#include "aef/embedded/osal/critical_section.h"

/**
* Internal routines.
*/
static service_status_t fnet_stack_core_start (service_ctx_t* ctx);
static service_status_t fnet_stack_core_restart (service_ctx_t* ctx);
static service_status_t fnet_stack_core_stop (service_ctx_t* ctx);
static service_status_t fnet_stack_core_status (service_ctx_t* ctx);
static service_status_t fnet_stack_core_pause (service_ctx_t* ctx);
static service_status_t fnet_stack_core_continue (service_ctx_t* ctx);

static service_status_t fnet_stack_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_release (service_ctx_t* ctx);
static service_status_t fnet_stack_core_get_macaddr (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_get_ip4addr (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket_bind (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_listen (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_accept (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket_connect (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_recv (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket_recv_from (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket_send (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket_send_to (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred);
static service_status_t fnet_stack_core_socket_shutdown (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_close (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_setopt (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_getopt (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_get_name (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_socket_get_peer_name (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t fnet_stack_core_dns_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred);

/**
* FNET stack service identifiers
*/
#define FNET_CFG_HEAP_SIZE			(24 * 1024u)
#define FNET_ETH0_IP4_ADDR_LOCAL	FNET_IP4_ADDR_INIT(192U, 168U, 27U, 200U)
#define FNET_ETH0_IP4_MASK_LOCAL	FNET_IP4_ADDR_INIT(255U, 255U, 255U, 0U)
#define FNET_ETH0_IP4_GW_LOCAL		FNET_IP4_ADDR_INIT(192U, 168U, 1U, 1U)
#define FNET_ETH0_IP4_DNS_LOCAL		FNET_IP4_ADDR_INIT(8U, 8U, 8U, 8U)

/**
* FNET stack service network interface configuration
*/
static netif_init_params_t fnet_eth0_config =
{
		.netif_desc = FNET_CPU_ETH0_IF,
		.netif_mac_addr = { 0x00,0x00,0x00,0x00,0x00,0x00 },
		.netif_ip4_addr = FNET_ETH0_IP4_ADDR_LOCAL,
		.netif_ip4_subnet_mask = FNET_ETH0_IP4_MASK_LOCAL,
		.netif_ip4_gateway = FNET_ETH0_IP4_GW_LOCAL,
		.netif_ip4_dns = FNET_ETH0_IP4_DNS_LOCAL,
};

/**
* FNET stack service critical section object
*/
static critical_section_ctx_t fnet_cs;

/**
* Create the FNET stack mutex
* This mutex is used to manage multi-threaded access to the FNET stack
*
* \param    stack_mutex		Pointer to the mutex context
*
* \returns  FNET_OK if successful.
*           Error code if unsuccessful.
*/
static fnet_return_t
fnet_stack_mutex_init ( fnet_mutex_t* stack_mutex )
{
	if ( critical_section_create (&fnet_cs) == SYSTEM_STATUS_SUCCESS )
		return FNET_OK;
	return FNET_ERR;
}

/**
* Delete the FNET stack mutex
* This mutex is used to manage multi-threaded access to the FNET stack
*
* \param    stack_mutex		Pointer to the mutex context to delete
*
* \returns  none.
*/
static void
fnet_stack_mutex_free ( fnet_mutex_t* stack_mutex)
{
	critical_section_destroy (&fnet_cs);
}

/**
* Lock the FNET stack mutex
* This mutex is used to manage multi-threaded access to the FNET stack
*
* \param    stack_mutex		Pointer to the mutex context to lock
*
* \returns  none.
*/
static void
fnet_stack_mutex_lock ( fnet_mutex_t* stack_mutex)
{
	critical_section_acquire (&fnet_cs);
}

/**
* Unlock the FNET stack mutex
* This mutex is used to manage multi-threaded access to the FNET stack
*
* \param    stack_mutex		Pointer to the mutex context to unlock
*
* \returns  none.
*/
static void
fnet_stack_mutex_unlock ( fnet_mutex_t* stack_mutex)
{
	critical_section_release (&fnet_cs);
}

const fnet_mutex_api_t fnet_stack_mutex_vtable =
{
		.mutex_init   = fnet_stack_mutex_init,
		.mutex_free   = fnet_stack_mutex_free,
		.mutex_lock   = fnet_stack_mutex_lock,
		.mutex_unlock = fnet_stack_mutex_unlock,
};


/**
* Run the FNET stack poll service feature, which allow FNET stack
* services to run in the background.
*
* \param    instance		Pointer to instance data
*
* \returns  none
*/
void fnet_stack_service_poll (void* instance )
{
	fnet_poll_service();
}

/**
* Initialize the FNET Network stack service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t fnet_stack_core_init (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		fnet_stack_ctx_t* stack_ctx = (fnet_stack_ctx_t*)ctx->ctx;
		property_manager_vtable_t* property_manager = system_get_property_manager();
		uint16_t mac_addr_length;

		/**
		 * Load the MAC address from system properties
		 */
		property_manager->getproperty ( CERT_FIELD_TYPE_MAC_ADDR, &mac_addr_length, fnet_eth0_config.netif_mac_addr );

		memset ( stack_ctx, 0, sizeof(fnet_stack_ctx_t) );
		stack_ctx->init_params.netheap_ptr = NULL;

		/**
		 * Initialize the FNET heap
		 */
		if ( FNET_CFG_HEAP_SIZE > 0 )
		{
			stack_ctx->init_params.netheap_ptr = malloc(FNET_CFG_HEAP_SIZE);
			if ( stack_ctx->init_params.netheap_ptr == NULL )
				return SERVICE_FAILURE_INITIALIZATION;

			stack_ctx->init_params.netheap_size = FNET_CFG_HEAP_SIZE;
		}

		/**
		 * Initialize the FNET mutex API for multi-threading support
		 */
		stack_ctx->init_params.mutex_api = &fnet_stack_mutex_vtable;

		/**
		 * Initialize the FNET stack
		 */
		if ( fnet_init(&stack_ctx->init_params) != FNET_ERR )
		{
			/**
			 * Initialize Network Interfaces.
			 */
	        if ( fnet_netif_init(fnet_eth0_config.netif_desc, fnet_eth0_config.netif_mac_addr, sizeof(fnet_mac_addr_t)) != FNET_ERR )
	        {
//	            fnet_netif_set_ip4_addr(fnet_eth0_config.netif_desc, fnet_eth0_config.netif_ip4_addr, fnet_eth0_config.netif_ip4_subnet_mask);
//	            fnet_netif_set_ip4_gateway(fnet_eth0_config.netif_desc, fnet_eth0_config.netif_ip4_gateway);
	            fnet_netif_set_ip4_dns(fnet_eth0_config.netif_desc, fnet_eth0_config.netif_ip4_dns);
	            fnet_netif_set_default(fnet_eth0_config.netif_desc); /* Set default interface.*/

	            /**
	             * Initialize / enable the DHCP client
	             */
	            if ( fnet_dhcp_init (fnet_eth0_config.netif_desc, &stack_ctx->dhcp_params) )
	            {
	            	/**
	            	 * Register DHCP event handler callbacks here
	            	 */
	            }

	            system_management_func_attach (ctx->name, NULL, fnet_stack_service_poll);
				ctx->state = SERVICE_START_PENDING;
				return SERVICE_STATUS_SUCCESS;
	        }
		}
		return SERVICE_FAILURE_INITIALIZATION;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* De-initialize the FNET Network Stack service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t fnet_stack_core_deinit (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		fnet_stack_ctx_t* stack_ctx = (fnet_stack_ctx_t*)ctx->ctx;

		/**
		 * De-initialize the FNET stack
		 */
		fnet_release ();

		/**
		 * Release the FNET heap
		 */
		if ( stack_ctx->init_params.netheap_ptr )
			free ( stack_ctx->init_params.netheap_ptr );

		ctx->state = SERVICE_UNINITIALIZED;
		return SERVICE_STATUS_SUCCESS;
	}
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
fnet_stack_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
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
			status = fnet_stack_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = fnet_stack_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = fnet_stack_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = fnet_stack_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = fnet_stack_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = fnet_stack_core_continue(ctx);
			break;
		case IOCTL_FNET_STACK_INITIALIZE:
			status = fnet_stack_core_initialize(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_RELEASE:
			status = fnet_stack_core_release(ctx);
			break;
		case IOCTL_FNET_STACK_DNS_INIT:
			status = fnet_stack_core_dns_init(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_GET_MACADDR:
			status = fnet_stack_core_get_macaddr (ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_GET_IP4ADDR:
			status = fnet_stack_core_get_ip4addr (ctx, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET:
			status = fnet_stack_core_socket(ctx, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET_BIND:
			status = fnet_stack_core_socket_bind(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_LISTEN:
			status = fnet_stack_core_socket_listen(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_ACCEPT:
			status = fnet_stack_core_socket_accept(ctx, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET_CONNECT:
			status = fnet_stack_core_socket_connect(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_RECV:
			status = fnet_stack_core_socket_recv(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET_RECV_FROM:
			status = fnet_stack_core_socket_recv_from(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET_SEND:
			status = fnet_stack_core_socket_send(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET_SEND_TO:
			status = fnet_stack_core_socket_send_to(ctx, input_buffer, input_size, bytes_transferred);
			break;
		case IOCTL_FNET_STACK_SOCKET_SHUTDOWN:
			status = fnet_stack_core_socket_shutdown(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_CLOSE:
			status = fnet_stack_core_socket_close(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_SETOPT:
			status = fnet_stack_core_socket_setopt(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_GETOPT:
			status = fnet_stack_core_socket_getopt(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_GET_NAME:
			status = fnet_stack_core_socket_get_name(ctx, input_buffer, input_size);
			break;
		case IOCTL_FNET_STACK_SOCKET_GET_PEER_NAME:
			status = fnet_stack_core_socket_get_peer_name(ctx, input_buffer, input_size);
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
service_status_t fnet_stack_core_start (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		ctx->state = SERVICE_RUNNING;
		return SERVICE_STATUS_SUCCESS;
	}
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
service_status_t fnet_stack_core_restart (service_ctx_t* ctx)
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
service_status_t fnet_stack_core_stop (service_ctx_t* ctx)
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
service_status_t fnet_stack_core_status (service_ctx_t* ctx)
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
service_status_t fnet_stack_core_pause (service_ctx_t* ctx)
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
service_status_t fnet_stack_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the FNET Network static service.
*
* \param    ctx				Pointer to the service context
* \param    input_Buffer	Pointer to memory area containing init parameters
* \param	input_size		Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* De-initialize the FNET Network static service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_release (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		fnet_release ();
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Create a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
* \param	output_buffer		Pointer to the output buffer
* \param	output_size			Length of the output buffer
* \param	bytes_transferred	Number of bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(socket_parms_t) &&
		 output_buffer != NULL && output_size == sizeof(socket_desc_t) )
	{
		socket_parms_t* parms = (socket_parms_t*)input_buffer;
		socket_desc_t* value = (socket_desc_t*)output_buffer;

		value->fnet_socket = fnet_socket ( parms->family, parms->type, parms->protocol );
		if ( value->fnet_socket != FNET_ERR )
		{
			if ( bytes_transferred != NULL )
				*bytes_transferred = sizeof(socket_desc_t);
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Bind a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_bind (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(bind_parms_t) )
	{
		bind_parms_t* parms = (bind_parms_t*)input_buffer;
//		parms->name->sa_scope_id = fnet_netif_get_scope_id(fnet_eth0_config.netif_desc);

		if ( fnet_socket_bind ( parms->fnet_socket, parms->name, parms->namelen) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Listen for a connection request on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_listen (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(listen_parms_t) )
	{
		listen_parms_t* parms = (listen_parms_t*)input_buffer;

		if ( fnet_socket_listen ( parms->fnet_socket, parms->backlog ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Accept a connection request on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
* \param	output_buffer		Pointer to the output buffer
* \param	output_size			Length of the output buffer
* \param	bytes_transferred	Number of bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_accept (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(accept_parms_t) &&
		 output_buffer != NULL && output_size == sizeof(socket_desc_t) )
	{
		accept_parms_t* parms = (accept_parms_t*)input_buffer;
		socket_desc_t* value = (socket_desc_t*)output_buffer;

		value->fnet_socket = fnet_socket_accept ( parms->fnet_socket, parms->addr, parms->addrlen );
		if ( value->fnet_socket != FNET_ERR )
		{
			if ( bytes_transferred != NULL )
				*bytes_transferred = sizeof(socket_desc_t);
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Issue a connection request on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_connect (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(connect_parms_t) )
	{
		connect_parms_t* parms = (connect_parms_t*)input_buffer;

		if ( fnet_socket_connect ( parms->fnet_socket, parms->name, parms->namelen ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Issue a receive on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
* \param	bytes_transferred	Number of bytes received
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_recv (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(recv_parms_t) &&
		 bytes_transferred != NULL )
	{
		recv_parms_t* parms = (recv_parms_t*)input_buffer;

		int32_t result = fnet_socket_recv ( parms->fnet_socket, parms->buffer, parms->length, parms->flags );
		if ( result != FNET_ERR )
		{
			*bytes_transferred = result;
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Issue a receive from on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
* \param	bytes_transferred	Number of bytes received
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_recv_from (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(recv_from_parms_t) &&
		 bytes_transferred != NULL )
	{
		recv_from_parms_t* parms = (recv_from_parms_t*)input_buffer;

		int32_t result = fnet_socket_recvfrom ( parms->fnet_socket, parms->buffer, parms->length, parms->flags,
				                                parms->from_addr, parms->from_length );
		if ( result != FNET_ERR )
		{
			*bytes_transferred = result;
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Send data on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
* \param	bytes_transferred	Number of bytes sent
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_send (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(send_parms_t) &&
		 bytes_transferred != NULL )
	{
		send_parms_t* parms = (send_parms_t*)input_buffer;

		int32_t result = fnet_socket_send ( parms->fnet_socket, parms->buffer, parms->length, parms->flags );
		if ( result != FNET_ERR )
		{
			*bytes_transferred = result;
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Send to data on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
* \param	bytes_transferred	Number of bytes sent
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_send_to (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(send_to_parms_t) &&
		 bytes_transferred != NULL )
	{
		send_to_parms_t* parms = (send_to_parms_t*)input_buffer;

		int32_t result = fnet_socket_sendto ( parms->fnet_socket, parms->buffer, parms->length, parms->flags,
				                              parms->to_addr, parms->to_length );
		if ( result != FNET_ERR )
		{
			*bytes_transferred = result;
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Shutdown a network connection on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_shutdown (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(shutdown_parms_t) )
	{
		shutdown_parms_t* parms = (shutdown_parms_t*)input_buffer;

		if ( fnet_socket_shutdown ( parms->fnet_socket, parms->how ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Close a network connection on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_close (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(close_parms_t) )
	{
		close_parms_t* parms = (close_parms_t*)input_buffer;

		if ( fnet_socket_close ( parms->fnet_socket ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Retrieve a socket option on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_setopt (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(setopt_parms_t) )
	{
		setopt_parms_t* parms = (setopt_parms_t*)input_buffer;

		if ( fnet_socket_setopt ( parms->fnet_socket, parms->level, parms->optname,
				                  parms->optval, parms->optvallen ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Set a socket option on a network socket
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_getopt (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(getopt_parms_t) )
	{
		getopt_parms_t* parms = (getopt_parms_t*)input_buffer;

		if ( fnet_socket_getopt ( parms->fnet_socket, parms->level, parms->optname,
				                  parms->optval, parms->optvallen ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Get name
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_get_name (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(name_parms_t) )
	{
		name_parms_t* parms = (name_parms_t*)input_buffer;

		if ( fnet_socket_getname ( parms->fnet_socket, parms->name, parms->namelen ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Get peer name
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area containing parameters
* \param	input_size			Length of the input buffer
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_socket_get_peer_name (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING &&
	     input_buffer != NULL && input_size == sizeof(name_parms_t) )
	{
		name_parms_t* parms = (name_parms_t*)input_buffer;

		if ( fnet_socket_getpeername ( parms->fnet_socket, parms->name, parms->namelen ) == FNET_OK )
		{
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Get the network device hardware address (MAC address)
*
* \param    ctx					Pointer to the service context
* \param    output_Buffer		Pointer to memory area to store the hardware address
* \param	output_size			Size of the memory area
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_get_macaddr (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= sizeof(macaddr_parms_t) )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			macaddr_parms_t* mac_addr = (macaddr_parms_t*)output_buffer;
			if ( fnet_netif_get_hw_addr ( fnet_eth0_config.netif_desc, mac_addr->hw_addr, sizeof(mac_addr->hw_addr) ) == FNET_OK )
			{
				if ( bytes_transferred )
					*bytes_transferred = sizeof(macaddr_parms_t);
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Get the network device IP4 address
*
* \param    ctx					Pointer to the service context
* \param    output_Buffer		Pointer to memory area to store IP4 address
* \param	output_size			Size of the memory area
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_get_ip4addr (service_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= sizeof(ip4addr_parms_t) )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			ip4addr_parms_t* ip4_addr = (ip4addr_parms_t*)output_buffer;
			ip4_addr->ip4_addr = fnet_netif_get_ip4_addr ( fnet_eth0_config.netif_desc );
			if ( bytes_transferred )
				*bytes_transferred = sizeof(ip4addr_parms_t);
			return SERVICE_STATUS_SUCCESS;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Initiate DNS host name resolution
*
* \param    ctx					Pointer to the service context
* \param    input_Buffer		Pointer to memory area to store DNS parameters
* \param	input_size			Size of the DNS parameter memory area
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t
fnet_stack_core_dns_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && input_buffer != NULL && input_size == sizeof(dns_init_parms_t) )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			dns_init_parms_t* params = (dns_init_parms_t*)input_buffer;
			struct fnet_dns_params dns_params =
				{
					.host_name   = params->host_name,
					.addr_family = params->addr_family,
					.callback	 = params->callback,
					.cookie      = params->param,
				};
			char dnsAddrStr[20];
			uint32_t dnsLocalAddr = FNET_ETH0_IP4_DNS_LOCAL;
			memset ( &dns_params.dns_server_addr, 0, sizeof(struct sockaddr) );
			fnet_inet_ntop ( dns_params.addr_family, &dnsLocalAddr, dnsAddrStr, sizeof(dnsAddrStr));
			fnet_inet_ptos ( dnsAddrStr, &dns_params.dns_server_addr );

			if ( fnet_dns_init ( &dns_params ) == FNET_OK )
			{
				if ( bytes_transferred )
					*bytes_transferred = sizeof(dns_init_parms_t);
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}
