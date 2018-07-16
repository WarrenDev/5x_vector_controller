
/**
* spi_driver_install.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief SPI built-in driver installation routines.
*/

#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/spi/spi_driver_install.h>
#include <aef/embedded/system/system_core.h>

/**
* Install the SPIA device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t spia_driver_install (void)
{
	extern const stream_driver_vtable_t spia_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &spia_vtable );
}

/**
* Install the SPIB device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t spib_driver_install (void)
{
	extern const stream_driver_vtable_t spib_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &spib_vtable );
}

