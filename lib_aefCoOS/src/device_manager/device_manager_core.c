
/**
* device_manager_core.c
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

#include "device_manager_core.h"

#include <aef/embedded/system/system_vtable.h>

static system_registry_t* device_driver_table = NULL;

/**
* Initialize the device manager.
*
* \param    none
*
* \returns DRIVER_STATUS_SUCCESS on successful initialization.
*          DRIVER_FAILURE_GENERAL on error.
*/
driver_status_t device_manager_core_init (void)
{
	return DRIVER_STATUS_SUCCESS;
}

/**
* Get an existing device instance from the device manager.  This is a blocking call.
*
* \param    device_id	Device id
*
* \returns  Driver instance handle
*/
stream_driver_vtable_t* device_manager_core_getdevice (device_driver_id_t device_id)
{
	return ( (stream_driver_vtable_t*) system_vtable_get_vtable(device_driver_table, device_id) );
}

/**
* Get an existing device instance from the device manager.  This is a non-blocking call.
*
* \param    device_id	Device id
*
* \returns  Driver instance handle
*/
stream_driver_vtable_t* device_manager_core_checkdevice (device_driver_id_t device_id)
{
	return ( (stream_driver_vtable_t*) system_vtable_get_vtable(device_driver_table, device_id) );
}

/**
* Place a new device into the device manager.
*
* \param   device_ptr	Device instance pointer
*
* \returns DRIVER_STATUS_SUCCESS on successful initialization.
*          DRIVER_FAILURE_GENERAL if unable to add the new device
*          DRIVER_FAILURE_INVALID_PARAMETER if device_ptr is NULL.
*/
driver_status_t device_manager_core_adddevice (const stream_driver_vtable_t* device_ptr)
{
	if ( device_ptr == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	/* If the device driver table doesn't exist, create it */
	if ( device_driver_table == NULL )
	{
		if ( system_vtable_create (&device_driver_table) != SYSTEM_STATUS_SUCCESS )
			return DRIVER_FAILURE_GENERAL;
	}

	/* Add the new device driver to the table */
	if ( system_vtable_add (device_driver_table, device_ptr->getid(), (const vtable_ptr_t) device_ptr) != SYSTEM_STATUS_SUCCESS )
		return DRIVER_FAILURE_GENERAL;

	/* Call the device driver init() routine to initialize the driver */
	return device_ptr->init(0);
}

/**
* Return a list of installed devices.
*
* \param    none
*
* \returns  List of installed devices
*/
void* device_manager_core_listdevices (void)
{
	return NULL;
}

/**
* Add a permission controller to the device manager.
*
* \param  	permission_controller	Pointer to a permission controller
*
* \returns DRIVER_STATUS_SUCCESS on successful initialization.
*          DRIVER_FAILURE_GENERAL if unable to add the new permission controller
*/
driver_status_t device_manager_core_setpermission (const void* permission_controller)
{
	return DRIVER_STATUS_SUCCESS;
}

