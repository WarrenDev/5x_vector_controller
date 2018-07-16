
/**
* device_manager_core.h
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

#ifndef SRC_DEVICE_MANAGER_DEVICE_MANAGER_CORE_H_
#define SRC_DEVICE_MANAGER_DEVICE_MANAGER_CORE_H_

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include "CoOS.h"
#include <stddef.h>
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* Initialize the device manager.
*
* \param    none
*
* \returns DRIVER_STATUS_SUCCESS on successful initialization.
*          DRIVER_FAILURE_GENERAL on error.
*/
driver_status_t device_manager_core_init (void);

/**
* Get an existing device instance from the device manager.  This is a blocking call.
*
* \param    Driver id
*
* \returns  Driver instance handle
*/
stream_driver_vtable_t* device_manager_core_getdevice (device_driver_id_t device_id);

/**
* Get an existing device instance from the device manager.  This is a non-blocking call.
*
* \param    Driver id
*
* \returns  Driver instance handle
*/
stream_driver_vtable_t* device_manager_core_checkdevice (device_driver_id_t device_id);

/**
* Place a new device driver into the device manager.
*
* \param   Driver pointer
*
* \returns DRIVER_STATUS_SUCCESS on successful initialization.
*          DRIVER_FAILURE_GENERAL if unable to add the new device
*/
driver_status_t device_manager_core_adddevice (const stream_driver_vtable_t* device_ptr);

/**
* Return a list of installed devices.
*
* \param    none
*
* \returns  List of installed devices
*/
void* device_manager_core_listdevices (void);

/**
* Add a permission controller to the device manager.
*
* \param    Pointer to a permission controller
*
* \returns DRIVER_STATUS_SUCCESS on successful initialization.
*          DRIVER_FAILURE_GENERAL if unable to add the new permission controller
*/
driver_status_t device_manager_core_setpermission (const void* permission_controller);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_DEVICE_MANAGER_DEVICE_MANAGER_CORE_H_ */
