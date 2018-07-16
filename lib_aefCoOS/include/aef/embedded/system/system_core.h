
/**
* system_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definitions of AEF system core routines..
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_CORE_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_CORE_H_

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/service/service_manager.h>
#include <aef/embedded/system/system_property_manager.h>
#include <aef/embedded/system/system_descriptors.h>
#include <aef/embedded/system/system_status.h>

#define UNREFERENCED_PARAMETER(p) 	(p)=(p)
#define MAKEWORD(low,hi)			((uint16_t)(((uint8_t)(low))|(((uint16_t)((uint8_t)(hi)))<<8)))
#define HIWORD(w)					((uint16_t)(((uint32_t)(w) >> 16) & 0xFFFF))
#define LOWORD(w)					((uint16_t)(w))
#define MAKELONG(low,hi)			((uint32_t)(((uint16_t)(low))|(((uint32_t)((uint16_t)(hi)))<<16)))
#define HIBYTE(w)					((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#define LOBYTE(w)					((uint8_t)(w))

# ifdef   __cplusplus
extern "C" {
# endif

/**
* Initialize descriptor tables
*
* /param none
*
* /return	SYSTEM_STATUS_SUCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_descriptor_tables (void);

/**
* Retrieve the system descriptor table properties descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT properties descriptor.
*/
descriptor_entry_t*
system_sdt_get_properties_descriptor (void);

/**
* Retrieve the system descriptor table events descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT events descriptor.
*/
descriptor_entry_t*
system_sdt_get_events_descriptor (void);

/**
* Retrieve the system descriptor table device manager descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT device manager descriptor.
*/
descriptor_entry_t*
system_sdt_get_device_manager_descriptor (void);

/**
* Retrieve the system descriptor table service manager descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT service manager descriptor.
*/
descriptor_entry_t*
system_sdt_get_service_manager_descriptor (void);

/**
* Retrieve the system descriptor table fault manager descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT fault manager descriptor.
*/
descriptor_entry_t*
system_sdt_get_fault_manager_descriptor (void);

/**
* Retrieve the device manager vtable pointer
*
* /param none
*
* /return	device_manager_vtable_t pointer for the device manager.
*/
device_manager_vtable_t*
system_get_device_manager (void);

/**
* Retrieve the service manager vtable pointer
*
* /param none
*
* /return	service_manager_vtable_t pointer for the service manager.
*/
service_manager_vtable_t*
system_get_service_manager (void);

/**
* Retrieve the property manager vtable pointer
*
* /param none
*
* /return	property_manager_vtable_t pointer for the property manager.
*/
property_manager_vtable_t*
system_get_property_manager (void);


# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_CORE_H_ */
