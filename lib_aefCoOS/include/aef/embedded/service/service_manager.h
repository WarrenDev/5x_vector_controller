
/**
* service_manager.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Interface for service manager support.
*
*/
#ifndef  AEF_SERVICE_MANAGER_HEADER_GUARD
# define AEF_SERVICE_MANAGER_HEADER_GUARD

#include <aef/embedded/service/service_id.h>
#include <aef/embedded/service/service_interface.h>
#include <aef/embedded/service/service_ioctl.h>
#include <aef/embedded/service/service_runlevel.h>
#include <aef/embedded/service/service_state.h>
#include <aef/embedded/service/service_status.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * The service manager Context is an opaque pointer to the context structure used
    * by the service manager.  This structure must be initialized by calling the 
    * appropriate initialization function prior to use.
    */
    typedef void* service_manager_ctx_t;

    /**
    * The service manager vtable type, which is used internally for working with
    * specific service manager implementations.
    */
    typedef struct service_manager_vtable
    {
        /**
        * Get the name tag of the service manager.
        *
        * \param    none
        *
        * \returns  Pointer to the service manager name tag
        */
        char* (*getname) (void);

        /**
        * Get the run level of the service manager.  The run level determines the
        * system load order, and is always 0 for the service manager.
        *
        * \param    none
        *
        * \returns  Run level of the service manager
        */
        service_runlevel_t (*runlevel) (void);

        /**
        * Initialize the service manager.
        *
        * \param    none
        *
        * \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
        *          	SERVICE_FAILURE_GENERAL on error.
        */
        service_status_t (*init) (void);

        /**
        * Get an existing service instance from the service manager.  This is a blocking call.
        *
        * \param    Service id
        *
        * \returns  Service instance handle
        */
        service_vtable_t* (*getservice) (service_id_t service_id);

        /**
        * Check status of a service.  This is a non-blocking call.
        *
        * \param    Service id
        *
        * \returns  Service instance handle
        */
        service_vtable_t* (*checkservice) (service_id_t service_id);

        /**
        * Place a new service into the service manager.
        *
        * \param    Service pointer
        *
        * \returns SERVICE_STATUS_SUCCESS on success.
        *          SERVICE_FAILURE_GENERAL if unable to add the new service
        */
        service_status_t (*addservice) (const service_vtable_t* service_ptr);

        /**
        * Start system services.  Services are initialized and started in run level order.
        *
        * \param    none
        *
        * \returns 	SERVICE_STATUS_SUCCESS on success.
        *          	SERVICE_FAILURE_GENERAL on error.
        */
        service_status_t (*startservices) (void);

        /**
        * Dispatch a system service control code to a service.
        *
        * \param    service_id		Service identifier
        * \param	control_code	Service control code
        * \param	parameter		Parameter to pass to the control routine
        * \param	size			Size of the parameter
        *
        * \returns 	SERVICE_STATUS_SUCCESS on success.
        *          	SERVICE_FAILURE_GENERAL on error.
        */
        service_status_t (*controlservice) (service_id_t service_id, service_ioctl_t control_code, void* parameter, uint32_t size);

        /**
        * Return a list of running services.
        *
        * \param    none
        * \returns  List of running services
        */
        void* (*listservices) (void);

        /**
        * Add a permission controller to the service manager.
        *
        * \param    Pointer to a permission controller
        *
        * \returns SERVICE_STATUS_SUCCESS on successful initialization.
        *          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
        */
        service_status_t (*setpermission) (const void* permission_controller);

    } service_manager_vtable_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_SERVICE_MANAGER_HEADER_GUARD
