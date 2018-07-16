
/**
* service_interface.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief service interface definition.
*
*/
#ifndef  AEF_SERVICE_HEADER_GUARD
# define AEF_SERVICE_HEADER_GUARD

#include <aef/embedded/service/service_id.h>
#include <aef/embedded/service/service_runlevel.h>
#include <aef/embedded/service/service_state.h>
#include <aef/embedded/service/service_status.h>
#include <stdint.h>


/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* The service interface context is the context structure used by the
	* system services during operations.  This structure must be
	* initialized by calling the appropriate initialization function prior to use.
	*/
    typedef struct service_ctx_def
	{
		char* name;
		uint32_t mode;
		service_state_t state;
		void* params;
		void* ctx;
	} service_ctx_t;

    /**
    * The system service vtable type, which is used for implementing system services
    */
    typedef struct service_vtable
    {

        /**
        * Get the id tag of the service.
        *
        * \param    none
        *
        * \returns  id tag of the service
        */
    	service_id_t (*getid) (void);

        /**
        * Get the name tag of the service.
        *
        * \param    none
        *
        * \returns  Pointer to the service name tag
        */
        char* (*getname) (void);

        /**
        * Get the run level of the service.  The run level determines the
        * system load order.
        *
        * \param    none
        *
        * \returns  Run level of the service
        */
        service_runlevel_t (*runlevel) (void);

        /**
        * Initialize a service instance.
        *
        * \param    init_parameters
        *
        * \returns  SERVICE_STATUS_SUCCESS if successful.
        *           SERVICE_FAILURE_GENERAL if unable to initialize the service.
        */
        service_status_t (*init) (uint32_t init_parameters);

        /**
        * De-initialize the service.
        *
        * \returns  SERVICE_STATUS_SUCCESS if successful.
        *           SERVICE_FAILURE_GENERAL if unable to close the service.
        */
        service_status_t (*deinit) (void);

        /**
        * Send a command to a service.
        *
        * \param    code - I/O control code to perform
        * \param    input_buffer
        * \param    input_size
        * \param    output_buffer
        * \param    output_size
        * \param    bytes_read
        *
        * \returns  SERVICE_STATUS_SUCCESS if successful.
        *           SERVICE_FAILURE_GENERAL if unable perform the command.
        *           SERVICE_FAILURE_UNAVAILABLE if the service is not in the run state
        */
        service_status_t (*iocontrol)
            (uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

    } service_vtable_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_STREAM_DRIVER_HEADER_GUARD
