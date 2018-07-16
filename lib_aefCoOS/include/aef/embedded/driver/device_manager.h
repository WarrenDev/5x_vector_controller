
/**
* device_manager.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Interface for device manager support.
*
*/
#ifndef  AEF_DEVICE_MANAGER_HEADER_GUARD
# define AEF_DEVICE_MANAGER_HEADER_GUARD

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>
#include <aef/embedded/driver/stream_driver.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * The device manager Context is an opaque pointer to the context structure used
    * by the device manager.  This structure must be initialized by calling the
    * appropriate initialization function prior to use.
    */
    typedef void* device_manager_ctx_t;

    /**
    * The device manager vtable type, which is used internally for working with
    * specific device manager implementations.
    */
    typedef struct device_manager_vtable
    {
        /**
        * Get the name tag of the device manager.
        *
        * \param    none
        *
        * \returns  Pointer to the device manager name tag
        */
        char* (*getname) (void);

        /**
        * Get the run level of the device manager.  The run level determines the
        * system load order, and is always 0 for the device manager.
        *
        * \param    none
        *
        * \returns  Run level of the device manager
        */
        device_runlevel_t (*runlevel) (void);

        /**
        * Initialize the device manager.
        *
        * \param    none
        *
        * \returns SERVICE_STATUS_SUCCESS on successful initialization.
        *          SERVICE_FAILURE_GENERAL on error.
        */
        driver_status_t (*init) (void);

        /**
        * Get an existing device instance from the device manager.  This is a blocking call.
        *
        * \param    Driver id
        * \returns  Driver instance handle
        */
        stream_driver_vtable_t* (*getdevice) (device_driver_id_t device_id);

        /**
        * Check status device drivers in the device manager device driver table.  This is a non-blocking call.
        *
        * \param    Driver id
        * \returns  Driver instance handle
        */
        stream_driver_vtable_t* (*checkdevice) (device_driver_id_t device_id);

        /**
        * Place a new device driver into the device manager.
        *
        * \param    Device pointer
        *
        * \returns SERVICE_STATUS_SUCCESS on success.
        *          SERVICE_FAILURE_GENERAL if unable to add the new device
        */
        driver_status_t (*adddevice) (const stream_driver_vtable_t* device_ptr);

        /**
        * Return a list of installed devices.
        *
        * \param    none
        * \returns  List of installed devices
        */
        void* (*listdevices) (void);

        /**
        * Add a permission controller to the device manager.
        *
        * \param    Pointer to a permission controller
        *
        * \returns SERVICE_STATUS_SUCCESS on success.
        *          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
        */
        driver_status_t (*setpermission) (const void* permission_controller);

    } device_manager_vtable_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_SERVICE_MANAGER_HEADER_GUARD
