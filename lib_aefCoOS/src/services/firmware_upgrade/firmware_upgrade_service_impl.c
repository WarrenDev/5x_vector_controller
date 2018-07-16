
/**
* firmware_upgrade_service_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Firmware Upgrade system service implementation.
*/

#include <aef/embedded/service/service_id.h>
#include <aef/embedded/service/service_interface.h>
#include <aef/embedded/service/service_runlevel.h>
#include <aef/embedded/service/service_status.h>

#include "firmware_upgrade_service_core.h"

#define SERVICE_NAME		(char*)"FIRMWARE_UPGRADE"

static service_id_t			service_id = SRV_SYSTEM_FIRMWARE_UPGRADE;
static service_runlevel_t   service_runlevel = SRV_RUNLEVEL2;
static service_ctx_t		ctx;

/**
* Get the id tag of the system service.
*
* \param    none
*
* \returns  id tag of the system service
*/
static
service_id_t firmware_upgrade_getid (void)
{
	return service_id;
}

/**
* Get the name tag of the system service.
*
* \param    none
*
* \returns  Pointer to the system service name tag
*/
static
char* firmware_upgrade_getname (void)
{
	return ctx.name;
}

/**
* Get the run level of the system service.  The run level determines the
* system load order.
*
* \param    none
*
* \returns  Run level of the system service
*/
static
service_runlevel_t firmware_upgrade_runlevel (void)
{
	return service_runlevel;
}

/**
* Initialize a system service.
* This function is required by service loaded by the service manager.
*
* \param    init_parameters		Initialization parameters
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to init the system service.
*/
static
service_status_t firmware_upgrade_init (uint32_t init_parameters)
{
	memset (&ctx, 0, sizeof(service_ctx_t));
	ctx.name = SERVICE_NAME;

	return firmware_upgrade_core_init (&ctx);
}

/**
* De-initialize the system service..
* This function is required by system services loaded by the service manager.
*
* \param    None
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to de-init the system service.
*/
static
service_status_t firmware_upgrade_deinit (void)
{
	return firmware_upgrade_core_deinit (&ctx);
}

/**
* Send a command to a system service.
*
* \param    code				I/O control code to perform
* \param    input_buffer		Pointer to the input buffer
* \param    input_size			Input buffer size
* \param    output_buffer		Pointer to the output buffer
* \param    output_size			Output buffer size
* \param    bytes_transferred	Pointer to the actual bytes read or written
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_UNAVAILABLE if the service is not in the run state
*/
static
service_status_t
firmware_upgrade_iocontrol
(uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	return firmware_upgrade_core_ioctl (&ctx, code, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
}

/**
* The system firmware_upgrade service vtable
*/
const service_vtable_t firmware_upgrade_srv_vtable =
{
		firmware_upgrade_getid,
		firmware_upgrade_getname,
		firmware_upgrade_runlevel,
		firmware_upgrade_init,
		firmware_upgrade_deinit,
		firmware_upgrade_iocontrol,
};

