
/**
* system_property_manager_impl.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief System property manager implementation.
*/

#include <aef/embedded/system/system_runlevel.h>
#include <aef/embedded/system/system_status.h>
#include <aef/embedded/system/system_property_manager.h>

#include "system_property_manager_core.h"

#define PROPERTY_MANAGER_RUNLEVEL		SYS_RUNLEVEL0
#define PROPERTY_MANAGER_NAME			(char*)"PROPMGR"

/**
* Get the name tag of the service manager.
*
* \param    none
* \returns  Pointer to the service manager name tag
*/
static
char* property_manager_getname (void)
{
	return PROPERTY_MANAGER_NAME;
}

/**
* Get the run level of the service manager.  The run level determines the
* system load order, and is always 0 for the service manager.
*
* \param    none
* \returns  Run level of the service manager
*/
static
system_runlevel_t property_manager_runlevel (void)
{
	return PROPERTY_MANAGER_RUNLEVEL;
}

/**
* Initialize the property manager.
*
* \param    none
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to initialize the property manager
*/
system_status_t property_manager_init (void)
{
	return property_manager_core_init ();
}

/**
* Get an existing property instance from the property manager.  This is a blocking call.
*
* \param    property_id			Property identifier (certificate field type)
* \param	length				Pointer to size of the property value in bytes
* \param 	value				Property value
*
* \returns SYSTEM_STATUS_SUCCESS on success.
*          SYSTEM_FAILURE_GENERAL if unable to add the new property
*/
static
system_status_t property_manager_getproperty (certificate_field_type_t property_id, uint16_t* length, void* value)
{
	return property_manager_core_getproperty ( property_id, length, value );
}

/**
* Add a new property.  This is a blocking call.
*
* \param    property_id			Property identifier (certificate field type)
* \param	length				Size of the property value in bytes
* \param 	value				Property value
*
* \returns SYSTEM_STATUS_SUCCESS on success.
*          SYSTEM_FAILURE_GENERAL if unable to add the new property
*/
static
system_status_t property_manager_addproperty (certificate_field_type_t property_id, uint16_t length, const void* value)
{
	return property_manager_core_addproperty ( property_id, length, value );
}

/**
* Update an existing property.
*
* \param    property_id			Property identifier (certificate field type)
* \param	length				Size of the property value in bytes
* \param 	value				Property value
*
* \returns SYSTEM_STATUS_SUCCESS on success.
*          SYSTEM_FAILURE_GENERAL if unable to update the property
*/
static
system_status_t property_manager_updateproperty (certificate_field_type_t property_id, uint16_t length, const void* value)
{
	return property_manager_core_updateproperty ( property_id, length, value );
}

/**
* Delete an existing property.
*
* \param    property_id			Property identifier (certificate field type)
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to delete the property
*/
static
system_status_t property_manager_deleteproperty (certificate_field_type_t property_id)
{
	return property_manager_core_deleteproperty ( property_id );
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
system_status_t property_manager_setpermission (const void* permission_controller)
{
	return property_manager_core_setpermission ( permission_controller );
}

/**
* The System Property Manager vtable
*/
const property_manager_vtable_t property_manager_vtable =
{
		.getname 		= property_manager_getname,
		.runlevel		= property_manager_runlevel,
		.init			= property_manager_init,
		.getproperty 	= property_manager_getproperty,
		.addproperty 	= property_manager_addproperty,
		.updateproperty = property_manager_updateproperty,
		.deleteproperty = property_manager_deleteproperty,
		.setpermission  = property_manager_setpermission
};
