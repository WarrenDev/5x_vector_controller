
/**
* service_manager_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Device Manager core definitions.
*
*/

#ifndef SRC_SERVICE_MANAGER_SERVICE_MANAGER_CORE_H_
#define SRC_SERVICE_MANAGER_SERVICE_MANAGER_CORE_H_

#include <aef/embedded/service/service_interface.h>
#include <aef/embedded/service/service_ioctl.h>
#include <aef/embedded/service/service_manager.h>
#include "CoOS.h"
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* Initialize the service manager.
*
* \param    none
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL on error.
*/
service_status_t service_manager_core_init (void);

/**
* Get an existing service instance from the service manager.  This is a blocking call.
*
* \param    service id
* \returns  Driver instance handle
*/
service_vtable_t* service_manager_core_getservice (service_id_t service_id);

/**
* Get an existing service instance from the service manager.  This is a non-blocking call.
*
* \param    service id
* \returns  Driver instance handle
*/
service_vtable_t* service_manager_core_checkservice (service_id_t service_id);

/**
* Place a new service into the service manager.
*
* \param    service pointer
*
* \returns SERVICE_STATUS_SUCCESS on successful initialization.
*          SERVICE_FAILURE_GENERAL if unable to add the new service
*/
service_status_t service_manager_core_addservice (const service_vtable_t* service_ptr);

/**
* Return a list of installed services.
*
* \param    none
* \returns  List of installed services
*/
void* service_manager_core_listservices (void);

/**
* Dispatch a system service control code to a service.
*
* \param    service_id		Service identifier
* \param	control_code	Service control code
* \param	parameter		Parameter to pass to the control routine
* \param	size			Size of the parameter
*
* \returns 	SERVICE_STATUS_SUCCESS on successful initialization.
*          	SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
service_status_t service_manager_core_controlservice (service_id_t service_id, service_ioctl_t control_code, void* parameter, uint32_t size);

/**
* Add a permission controller to the service manager.
*
* \param    Pointer to a permission controller
*
* \returns SERVICE_STATUS_SUCCESS on successful initialization.
*          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
service_status_t service_manager_core_setpermission (const void* permission_controller);

/**
* Start all system services.  Start services in runlevel order.
*
* \param    None.
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if a problem was encountered.
*/
service_status_t service_manager_core_start_services (void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICE_MANAGER_SERVICE_MANAGER_CORE_H_ */
