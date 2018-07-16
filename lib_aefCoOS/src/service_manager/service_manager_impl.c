
/**
* service_manager_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Device manager implementation.
*/

#include <aef/embedded/service/service_id.h>
#include <aef/embedded/service/service_runlevel.h>
#include <aef/embedded/service/service_status.h>

#include "service_manager_core.h"

#define SERVICE_MANAGER_RUNLEVEL		SRV_RUNLEVEL0
#define SERVICE_MANAGER_NAME			(char*)"SRVMGR"

/**
* Get the name tag of the service manager.
*
* \param    none
* \returns  Pointer to the service manager name tag
*/
static
char* service_manager_getname (void)
{
	return SERVICE_MANAGER_NAME;
}

/**
* Get the run level of the service manager.  The run level determines the
* system load order, and is always 0 for the service manager.
*
* \param    none
* \returns  Run level of the service manager
*/
static
service_runlevel_t service_manager_runlevel (void)
{
	return SERVICE_MANAGER_RUNLEVEL;
}

/**
* Initialize the service manager.
*
* \param    none
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL on error.
*/
static
service_status_t service_manager_init (void)
{
	return service_manager_core_init ();
}

/**
* Get an existing service instance from the service manager.  This is a blocking call.
*
* \param    Service id
* \returns  Service instance handle
*/
static
service_vtable_t* service_manager_getservice (service_id_t service_id)
{
	return service_manager_core_getservice ( service_id );
}

/**
* Get an existing service instance from the service manager.  This is a non-blocking call.
*
* \param    Service driver id
* \returns  Service instance handle
*/
static
service_vtable_t* service_manager_checkservice (service_id_t service_id)
{
	return service_manager_core_checkservice ( service_id );
}

/**
* Place a new service into the service manager.
*
* \param   Service interface pointer
*
* \returns SERVICE_STATUS_SUCCESS on successful initialization.
*          SERVICE_FAILURE_GENERAL if unable to add the new service
*/
static
service_status_t service_manager_addservice (const service_vtable_t* service_ptr)
{
	return service_manager_core_addservice ( service_ptr );
}

/**
* Start system services.  Services are initialized and started in run level order.
*
* \param    none
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
static
service_status_t service_manager_startservices (void)
{
	return service_manager_core_start_services ();
}

/**
* Dispatch a system service control code to a service.
*
* \param    service_id		Service identifier
* \param	control_code	Service control code
* \param	parameter		Pointer to the parameter to pass to the control routine
* \param	size			Size of the parameter
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
static
service_status_t service_manager_controlservice (service_id_t service_id, service_ioctl_t control_code, void* parameter, uint32_t size)
{
	return service_manager_core_controlservice (service_id, control_code, parameter, size);
}

/**
* Return a list of installed services.
*
* \param    none
* \returns  List of installed services
*/
static
void* service_manager_listservices (void)
{
	return service_manager_core_listservices ();
}

/**
* Add a permission controller to the service manager.
*
* \param    Pointer to a permission controller
*
* \returns SERVICE_STATUS_SUCCESS on successful initialization.
*          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
static
service_status_t service_manager_setpermission (const void* permission_controller)
{
	return service_manager_core_setpermission ( permission_controller );
}

/**
* The Service Manager vtable
*/
const service_manager_vtable_t service_manager_vtable =
{
		.getname 		= service_manager_getname,
		.runlevel 		= service_manager_runlevel,
		.init 			= service_manager_init,
		.getservice 	= service_manager_getservice,
		.checkservice 	= service_manager_checkservice,
		.addservice 	= service_manager_addservice,
		.startservices 	= service_manager_startservices,
		.controlservice = service_manager_controlservice,
		.listservices 	= service_manager_listservices,
		.setpermission 	= service_manager_setpermission
};
