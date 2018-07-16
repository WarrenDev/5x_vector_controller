
/**
* service_manager_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  UART stream driver core implementation.
*
*/

#include "service_manager_core.h"

#include <aef/embedded/system/system_vtable.h>

/**
* Internal routines
 */
static
void* start_service_using_runlevel_cb (vtable_entry_t* item, uint32_t param);

static system_registry_t* service_table = NULL;

/**
* Initialize the service manager.
*
* \param    none
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL on error.
*/
service_status_t service_manager_core_init (void)
{
	return SERVICE_STATUS_SUCCESS;
}

/**
* Get an existing service instance from the service manager.  This is a blocking call.
*
* \param    Service id
* \returns  Service instance handle
*/
service_vtable_t* service_manager_core_getservice (service_id_t service_id)
{
	return ( (service_vtable_t*) system_vtable_get_vtable(service_table, service_id) );
}

/**
* Get an existing service instance from the service manager.  This is a non-blocking call.
*
* \param    Service id
* \returns  Service instance handle
*/
service_vtable_t* service_manager_core_checkservice (service_id_t service_id)
{
	return ( (service_vtable_t*) system_vtable_get_vtable(service_table, service_id) );
}

/**
* Place a new service into the service manager.
*
* \param    Service pointer
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL if unable to add the new service
*/
service_status_t service_manager_core_addservice (const service_vtable_t* service_ptr)
{
	if ( service_ptr == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	/* If the service table doesn't exist, create it */
	if ( service_table == NULL )
	{
		if ( system_vtable_create (&service_table) != SYSTEM_STATUS_SUCCESS )
			return SERVICE_FAILURE_GENERAL;
	}

	/* Add the new service to the table */
	if ( system_vtable_add (service_table, service_ptr->getid(), (const vtable_ptr_t)service_ptr) != SYSTEM_STATUS_SUCCESS )
		return SERVICE_FAILURE_GENERAL;

	/* Call the service init() routine to initialize the service */
	return service_ptr->init(0);
}

/**
* Return a list of installed services.
*
* \param    none
* \returns  List of installed services
*/
void* service_manager_core_listservices (void)
{
	return NULL;
}

/**
* Send a system service control code to a service.
*
* \param    service_id		Service identifier
* \param	control_code	Service control code
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
service_status_t service_manager_core_controlservice (service_id_t service_id, service_ioctl_t control_code, void* parameter, uint32_t size)
{
	const service_vtable_t* service_ptr = service_manager_core_getservice (service_id);
	if ( service_ptr != NULL )
	{
		return service_ptr->iocontrol(control_code, NULL, 0, NULL, 0, NULL);
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Add a permission controller to the service manager.
*
* \param    Pointer to a permission controller
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
service_status_t service_manager_core_setpermission (const void* permission_controller)
{
	return SERVICE_STATUS_SUCCESS;
}

/**
* Start all system services.  Start services in runlevel order.
*
* \param    None.
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if a problem was encountered.
*/
service_status_t service_manager_core_start_services (void)
{
	for (service_runlevel_t runlevel = SRV_RUNLEVEL1; runlevel <= SRV_RUNLEVEL10; runlevel++)
	{
		system_vtable_iterator(service_table, start_service_using_runlevel_cb, runlevel);
	}
	return SERVICE_STATUS_SUCCESS;
}

/**
* Start a service in the system services vtable based on runlevel.
*
* \param    item		Pointer to the vtable item containing a service vtable
* \param	runlevel	Run level to start
*
* \returns 	NULL
*/
void* start_service_using_runlevel_cb (vtable_entry_t* item, uint32_t runlevel)
{
	const service_vtable_t* service_ptr = (service_vtable_t*)item->vtablePtr;
	if ( service_ptr && (service_ptr->runlevel() == runlevel) )
	{
		service_ptr->iocontrol(IOCTL_SERVICE_START, NULL, 0, NULL, 0, NULL);
	}
	return NULL;
}
