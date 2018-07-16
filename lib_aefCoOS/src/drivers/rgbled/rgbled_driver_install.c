
/**
* rgbled_driver_install.c
*
* \copyright
* Copyright 2016 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief Implementation of RGB LED built-in driver installation routines.
*/

#include <aef/embedded/driver/rgbled/rgbled_driver_install.h>
#include "aef/embedded/system/system_core.h"
#include "aef/embedded/driver/device_manager.h"
#include "aef/embedded/driver/device_driver_status.h"

/**
* Install the RGBLED device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t rgbled_driver_install (void)
{
	extern const stream_driver_vtable_t rgbled_drv_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &rgbled_drv_vtable );
}

