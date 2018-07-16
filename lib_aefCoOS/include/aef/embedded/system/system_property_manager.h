
/**
* system_property_manager.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Interface for the system property manager.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_PROPERTY_MANAGER_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_PROPERTY_MANAGER_H_

#include <aef/embedded/system/system_runlevel.h>
#include <aef/embedded/system/system_status.h>
#include <aef/certificate/certificate_definitions.h>
#include <aef/certificate/certificate_core.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * The property manager context is an opaque pointer to the context structure used
    * by the property manager.  This structure must be initialized by calling the
    * appropriate initialization function prior to use.
    */
    typedef void* property_manager_ctx_t;

    /**
    * The property manager vtable type, which is used internally for working with
    * specific property manager implementations.
    */
    typedef struct property_manager_vtable
    {
        /**
        * Get the name tag of the property manager.
        *
        * \param    none
        *
        * \returns  Pointer to the property manager name tag
        */
        char* (*getname) (void);

        /**
        * Get the run level of the property manager.  The run level determines the
        * system load order, and is always 0 for the property manager.
        *
        * \param    none
        *
        * \returns  Run level of the property manager
        */
        system_runlevel_t (*runlevel) (void);

        /**
        * Initialize the property manager.
        *
        * \param    none
        *
        * \returns SERVICE_STATUS_SUCCESS on success.
        *          SERVICE_FAILURE_GENERAL if unable to initialize the property manager
        */
        system_status_t (*init) (void);

        /**
        * Get an existing property instance from the property manager.  This is a blocking call.
        *
        * \param    property_id			Property identifier (certificate field type)
        * \param	length				Pointer to size of the property value in bytes
        * \param 	value				Property value
        *
        * \returns  Property instance handle
        */
        system_status_t (*getproperty) (certificate_field_type_t property_id, uint16_t* length, void* value);

        /**
        * Add a new property.  This is a non-blocking call.
        *
        * \param    property_id			Property identifier (certificate field type)
        * \param	length				Size of the property value in bytes
        * \param 	value				Property value
        *
        * \returns SYSTEM_STATUS_SUCCESS on success.
        *          SYSTEM_FAILURE_GENERAL if unable to add the new property
        */
        system_status_t (*addproperty) (certificate_field_type_t property_id, uint16_t length, const void* value);

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
        system_status_t (*updateproperty) (certificate_field_type_t property_id, uint16_t length, const void* value);

        /**
        * Delete an existing property.
        *
        * \param    property_id			Property identifier (certificate field type)
        *
        * \returns SERVICE_STATUS_SUCCESS on success.
        *          SERVICE_FAILURE_GENERAL if unable to delete the property
        */
        system_status_t (*deleteproperty) (certificate_field_type_t property_id);

        /**
        * Add a permission controller to the property manager.
        *
        * \param    Pointer to a permission controller
        *
        * \returns SERVICE_STATUS_SUCCESS on success.
        *          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
        */
        system_status_t (*setpermission) (const void* permission_controller);

    } property_manager_vtable_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_PROPERTY_MANAGER_H_ */
