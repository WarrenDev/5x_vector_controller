
/**
* gpsd_service_install.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementation of GPSD system service installation routines.
*
*/

#include <aef/embedded/service/gpsd/gpsd_service_install.h>
#include <aef/embedded/service/service_manager.h>
#include <aef/embedded/service/service_status.h>
#include <aef/embedded/system/system_core.h>

/**
* Install the system GPSD system service.
*
* \param    none
*
* \returns  SERVICE_STATUS_SUCCESSS if the service is successfully installed.
* 			SERVICE_FAILURE_GENERAL if service installation failed.
*/
service_status_t gpsd_service_install (void)
{
	extern const service_vtable_t system_gpsd_srv_vtable;
	service_manager_vtable_t* service_manager = system_get_service_manager();

	if ( service_manager == NULL )
		return SERVICE_FAILURE_GENERAL;

	return service_manager->addservice ( &system_gpsd_srv_vtable );
}

