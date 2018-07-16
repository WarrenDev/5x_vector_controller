
/**
* system_property_manager_core.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Certificate-based system property Manager core definitions.
*
*/

#ifndef SRC_SYSTEM_PROPERTY_MANAGER_SYSTEM_PROPERTY_MANAGER_CORE_H_
#define SRC_SYSTEM_PROPERTY_MANAGER_SYSTEM_PROPERTY_MANAGER_CORE_H_

#include <aef/embedded/system/system_property_manager.h>
#include "CoOS.h"
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* Initialize the property manager.
*
* \param    none
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to initialize the property manager
*/
system_status_t property_manager_core_init (void);

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
system_status_t property_manager_core_getproperty (certificate_field_type_t property_id, uint16_t* length, void* value);

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
system_status_t property_manager_core_addproperty (certificate_field_type_t property_id, uint16_t length, const void* value);

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
system_status_t property_manager_core_updateproperty (certificate_field_type_t property_id, uint16_t length, const void* value);

/**
* Delete an existing property.
*
* \param    property_id			Property identifier (certificate field type)
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to delete the property
*/
system_status_t property_manager_core_deleteproperty (certificate_field_type_t property_id);

/**
* Add a permission controller to the property manager.
*
* \param    Pointer to a permission controller
*
* \returns SERVICE_STATUS_SUCCESS on successful.
*          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
system_status_t property_manager_core_setpermission (const void* permission_controller);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SYSTEM_PROPERTY_MANAGER_SYSTEM_PROPERTY_MANAGER_CORE_H_ */
