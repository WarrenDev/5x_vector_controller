
/**
* system_init.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Advanced Embedded Framework system initialization.
*/

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/i2c/i2c_driver_install.h>
#include <aef/embedded/driver/spi/spi_driver_install.h>
#include <aef/embedded/driver/uart/uart_driver_install.h>
#include <aef/embedded/driver/rgbled/rgbled_driver_install.h>
#include <aef/embedded/driver/mpu9250/mpu9250_driver_install.h>
#include <aef/embedded/service/ble_manager/ble_manager_service_install.h>
#include <aef/embedded/service/mqtt/mqtt_service_install.h>
#include <aef/embedded/service/console/console_service_install.h>
#include <aef/embedded/service/crypto_manager/crypto_manager_service_install.h>
#include <aef/embedded/service/database/database_service_install.h>
#include <aef/embedded/service/firmware_upgrade/firmware_upgrade_service_install.h>
#include <aef/embedded/service/logger/logger_service_install.h>
#include <aef/embedded/service/fnet_stack/fnet_stack_service_install.h>
#include <aef/embedded/service/watchdog/watchdog_service_install.h>
#include <aef/embedded/service/gpsd/gpsd_service_install.h>
#include <aef/embedded/service/service_manager.h>
#include <aef/embedded/system/system_core.h>
#include <aef/embedded/system/system_init.h>
#include <aef/embedded/system/system_management.h>
#include <CoOS.h>

static
system_status_t aef_system_install_drivers ( void );

static
system_status_t aef_system_install_services ( void );

static
system_status_t aef_system_start_services ( void );

static
system_status_t aef_system_start_management_task ( void );

/**
* Initialize the Advanced Embedded Framework
*
* Creates and initializes the descriptor tables, install drivers
* and services, and create and start the system management thread.
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if initialization failed.
*/
system_status_t
aef_system_init ( void )
{
	system_init_descriptor_tables();

	aef_system_start_management_task();

	aef_system_install_drivers();

	aef_system_install_services();

	aef_system_start_services();

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Install built-in device drivers
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if initialization failed.
*/
system_status_t
aef_system_install_drivers ( void )
{
	driver_status_t status;

	/*
	* UART peripheral drivers
	*/
//	status = uarta_driver_install ();
	status = uartb_driver_install ();
//	status = uartc_driver_install ();
//	status = uartd_driver_install ();

	/*
	* SPI peripheral drivers
	*/
//	status = spia_driver_install ();
//	status = spib_driver_install ();

	/*
	* I2C peripheral drivers
	*/
	status |= ii2ca_driver_install ();

	/*
	* MPU-9250 IMU driver
	*/
	status |= mpu9250_driver_install ();

	/*
	* RGB LED driver
	*/
//	status |= rgbled_driver_install ();

	return status;
}

/**
* Install built-in services
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if initialization failed.
*/
system_status_t
aef_system_install_services ( void )
{
	service_status_t status;

	status = console_service_install ();
	status = watchdog_service_install ();
	status = logger_service_install ();
//	status = database_service_install ();
//	status = ble_manager_service_install ();
//	status = firmware_upgrade_service_install ();
//	status = crypto_manager_service_install ();
	status = fnet_stack_service_install ();
	status = mqtt_service_install ();
	status = gpsd_service_install ();

	return status;
}

/**
* Start built-in services
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if failed.
*/
system_status_t
aef_system_start_services ( void )
{
	service_manager_vtable_t* service_manager = system_get_service_manager();

	if ( service_manager == NULL )
		return SERVICE_FAILURE_GENERAL;

	return service_manager->startservices();
}

/**
* Create the AEF system management thread
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if initialization failed.
*/
system_status_t
aef_system_start_management_task ( void )
{
	return system_management_function_core_init();
}
