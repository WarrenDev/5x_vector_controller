
/**
* i2c_driver_install.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief I2C built-in driver installation routines.
*/

#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/i2c/i2c_driver_install.h>
#include <aef/embedded/system/system_core.h>

/**
* Install the II2CA device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t ii2ca_driver_install (void)
{
	extern const stream_driver_vtable_t ii2ca_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &ii2ca_vtable );
}


