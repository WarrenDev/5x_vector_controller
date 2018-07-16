
/**
* uart_driver_install.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief UART built-in driver installation routines.
*/

#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/uart/uart_driver_install.h>
#include <aef/embedded/system/system_core.h>

/**
* Install the UARTA device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uarta_driver_install (void)
{
	extern const stream_driver_vtable_t uarta_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &uarta_vtable );
}

/**
* Install the UARTB device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uartb_driver_install (void)
{
	extern const stream_driver_vtable_t uartb_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &uartb_vtable );
}

/**
* Install the UARTC device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uartc_driver_install (void)
{
	extern const stream_driver_vtable_t uartc_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &uartc_vtable );
}

/**
* Install the UARTD device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uartd_driver_install (void)
{
	extern const stream_driver_vtable_t uartd_vtable;
	device_manager_vtable_t* device_manager = system_get_device_manager();

	if ( device_manager == NULL )
		return DRIVER_FAILURE_GENERAL;

	return device_manager->adddevice ( &uartd_vtable );
}

