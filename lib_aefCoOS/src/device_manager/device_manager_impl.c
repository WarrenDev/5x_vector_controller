
/**
* device_manager_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Device manager implementation.
*/

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>

#include "device_manager_core.h"

#define DEVICE_MANAGER_RUNLEVEL		DRV_RUNLEVEL0
#define DEVICE_MANAGER_NAME			(char*)"DEVMGR"

/**
* Get the name tag of the device manager.
*
* \param    none
* \returns  Pointer to the device manager name tag
*/
static
char* device_manager_getname (void)
{
	return DEVICE_MANAGER_NAME;
}

/**
* Get the run level of the device manager.  The run level determines the
* system load order, and is always 0 for the device manager.
*
* \param    none
* \returns  Run level of the device manager
*/
static
device_runlevel_t device_manager_runlevel (void)
{
	return DEVICE_MANAGER_RUNLEVEL;
}

/**
* Initialize the device manager.
*
* \param    none
*
* \returns SERVICE_STATUS_SUCCESS on successful initialization.
*          SERVICE_FAILURE_GENERAL on error.
*/
static
driver_status_t device_manager_init (void)
{
	return device_manager_core_init ();
}

/**
* Get an existing device instance from the device manager.  This is a blocking call.
*
* \param    Device id
* \returns  Driver instance handle
*/
static
stream_driver_vtable_t* device_manager_getdevice (device_driver_id_t device_id)
{
	return device_manager_core_getdevice ( device_id );
}

/**
* Check status of devices in the device manager device table.  This is a non-blocking call.
*
* \param    Device driver id
* \returns  Driver instance handle
*/
static
stream_driver_vtable_t* device_manager_checkdevice (device_driver_id_t device_id)
{
	return device_manager_core_checkdevice ( device_id );
}

/**
* Place a new device into the device manager.
*
* \param   Device driver pointer
*
* \returns SERVICE_STATUS_SUCCESS on successful initialization.
*          SERVICE_FAILURE_GENERAL if unable to add the new device
*/
static
driver_status_t device_manager_adddevice (const stream_driver_vtable_t* device_ptr)
{
	return device_manager_core_adddevice ( device_ptr );
}

/**
* Return a list of installed devices.
*
* \param    none
* \returns  List of installed devices
*/
static
void* device_manager_listdevices (void)
{
	return device_manager_core_listdevices ();
}

/**
* Add a permission controller to the device manager.
*
* \param    Pointer to a permission controller
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
static
driver_status_t device_manager_setpermission (const void* permission_controller)
{
	return device_manager_core_setpermission ( permission_controller );
}

/**
* The Device Manager vtable
*/
const device_manager_vtable_t device_manager_vtable =
{
		.getname 		= device_manager_getname,
		.runlevel 		= device_manager_runlevel,
		.init 			= device_manager_init,
		.getdevice 		= device_manager_getdevice,
		.checkdevice 	= device_manager_checkdevice,
		.adddevice 		= device_manager_adddevice,
		.listdevices 	= device_manager_listdevices,
		.setpermission	= device_manager_setpermission
};
