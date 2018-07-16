
/**
* system_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementation of AEF system core routines.
*/

#include <aef/embedded/system/system_core.h>
#include <CoOS.h>
#include <string.h>

/**
* AEF descriptor tables
*/
static system_descriptor_table_t	sdt;
static user_descriptor_table_t		udt;

/**
* Internal routines
*/
static
system_status_t
system_init_sdt (void);

static
system_status_t
system_init_udt (void);

static
system_status_t
system_init_sdt_property_manager_descriptor (descriptor_entry_t* properties);

static
system_status_t
system_init_sdt_event_manager_descriptor (descriptor_entry_t* events);

static
system_status_t
system_init_sdt_device_manager_descriptor (descriptor_entry_t* device_manager);

static
system_status_t
system_init_sdt_service_manager_descriptor (descriptor_entry_t* service_manager);

static
system_status_t
system_init_sdt_fault_manager_descriptor (descriptor_entry_t* fault_manager);

static
system_status_t
system_init_udt_property_manager_descriptor (descriptor_entry_t* properties);

static
system_status_t
system_init_udt_event_manager_descriptor (descriptor_entry_t* events);

static
system_status_t
system_init_udt_service_manager_descriptor (descriptor_entry_t* service_manager);

/**
* Initialize descriptor tables
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_descriptor_tables (void)
{

	system_init_sdt ();
	system_init_udt ();

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Retrieve the system descriptor table properties descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT properties descriptor.
*/
descriptor_entry_t*
system_sdt_get_property_manager_descriptor (void)
{
	return &sdt.property_manager;
}

/**
* Retrieve the system descriptor table events descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT events descriptor.
*/
descriptor_entry_t*
system_sdt_get_event_manager_descriptor (void)
{
	return &sdt.event_manager;
}

/**
* Retrieve the system descriptor table device manager descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT device manager descriptor.
*/
descriptor_entry_t*
system_sdt_get_device_manager_descriptor (void)
{
	return &sdt.device_manager;
}

/**
* Retrieve the system descriptor table service manager descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT service manager descriptor.
*/
descriptor_entry_t*
system_sdt_get_service_manager_descriptor (void)
{
	return &sdt.service_manager;
}

/**
* Retrieve the system descriptor table fault manager descriptor
*
* /param none
*
* /return	descriptor_entry_t pointer to the SDT fault manager descriptor.
*/
descriptor_entry_t*
system_sdt_get_fault_manager_descriptor (void)
{
	return &sdt.fault_manager;
}

/**
* Retrieve the device manager vtable pointer
*
* /param none
*
* /return	device_manager_vtable_t pointer for the device manager.
*/
device_manager_vtable_t*
system_get_device_manager (void)
{
	return ( (device_manager_vtable_t*)sdt.device_manager.address );
}

/**
* Retrieve the service manager vtable pointer
*
* /param none
*
* /return	service_manager_vtable_t pointer for the service manager.
*/
service_manager_vtable_t*
system_get_service_manager (void)
{
	return ( (service_manager_vtable_t*)sdt.service_manager.address );
}

/**
* Retrieve the property manager vtable pointer
*
* /param none
*
* /return	property_manager_vtable_t pointer for the property manager.
*/
property_manager_vtable_t*
system_get_property_manager (void)
{
	return ( (property_manager_vtable_t*)sdt.property_manager.address );
}

/**
* Initialize system descriptor table
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_sdt (void)
{
	memset( &sdt, 0, sizeof(system_descriptor_table_t) );

	system_init_sdt_property_manager_descriptor ( &sdt.property_manager );
	system_init_sdt_event_manager_descriptor ( &sdt.event_manager );
	system_init_sdt_device_manager_descriptor ( &sdt.device_manager );
	system_init_sdt_service_manager_descriptor ( &sdt.service_manager );
	system_init_sdt_fault_manager_descriptor ( &sdt.fault_manager );

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Initialize system descriptor table property manager descriptor entry
*
* /param properties		Pointer to a property descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_sdt_property_manager_descriptor (descriptor_entry_t* property_manager)
{
	extern const property_manager_vtable_t property_manager_vtable;

	property_manager->type._bits.segment_type = 0;		// 32 bit
	property_manager->type._bits.privilege_level = 0;	// Ring 0
	property_manager->type._bits.in_memory = 1;			// in RAM
	property_manager->type._bits.type = 0;				// Data
	property_manager->address = &property_manager_vtable;

	return ((property_manager_vtable_t*)property_manager->address)->init();
}

/**
* Initialize system descriptor table event manager descriptor entry
*
* /param events			Pointer to an events descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_sdt_event_manager_descriptor (descriptor_entry_t* event_manager)
{
	event_manager->type._bits.segment_type = 0;			// 32 bit
	event_manager->type._bits.privilege_level = 0;		// Ring 0
	event_manager->type._bits.in_memory = 1;			// in RAM
	event_manager->type._bits.type = 0;					// Data
	event_manager->address = NULL;

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Initialize system descriptor table device manager descriptor entry
*
* /param device_manager		Pointer to the device manager descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_sdt_device_manager_descriptor (descriptor_entry_t* device_manager)
{
	extern const device_manager_vtable_t device_manager_vtable;

	device_manager->type._bits.segment_type = 0;		// 32 bit
	device_manager->type._bits.privilege_level = 0;		// Ring 0
	device_manager->type._bits.in_memory = 1;			// in RAM
	device_manager->type._bits.type = 1;				// Code
	device_manager->address = &device_manager_vtable;

	return ((device_manager_vtable_t*)device_manager->address)->init();
}

/**
* Initialize system descriptor table service manager descriptor entry
*
* /param service_manager		Pointer to the service manager descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_sdt_service_manager_descriptor (descriptor_entry_t* service_manager)
{
	extern const service_manager_vtable_t service_manager_vtable;

	service_manager->type._bits.segment_type = 0;		// 32 bit
	service_manager->type._bits.privilege_level = 0;	// Ring 0
	service_manager->type._bits.in_memory = 1;			// in RAM
	service_manager->type._bits.type = 1;				// Code
	service_manager->address = &service_manager_vtable;

	return ((service_manager_vtable_t*)service_manager->address)->init();
}

/**
* Initialize system descriptor table fault manager descriptor entry
*
* /param fault_manager			Pointer to the fault manager descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_sdt_fault_manager_descriptor (descriptor_entry_t* fault_manager)
{
	fault_manager->type._bits.segment_type = 0;			// 32 bit
	fault_manager->type._bits.privilege_level = 0;		// Ring 0
	fault_manager->type._bits.in_memory = 1;			// in RAM
	fault_manager->type._bits.type = 1;					// Code
	fault_manager->address = NULL;

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Initialize user descriptor table
*
* /param none
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_udt (void)
{
	memset( &udt, 0, sizeof(user_descriptor_table_t) );

	system_init_udt_property_manager_descriptor ( &udt.property_manager );
	system_init_udt_event_manager_descriptor ( &udt.event_manager );
	system_init_udt_service_manager_descriptor ( &udt.service_manager );

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Initialize user descriptor table property manager descriptor entry
*
* /param properties			Pointer to a property manager descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_udt_property_manager_descriptor (descriptor_entry_t* property_manager)
{
	property_manager->type._bits.segment_type = 0;		// 32 bit
	property_manager->type._bits.privilege_level = 3;	// Ring 3
	property_manager->type._bits.in_memory = 1;			// in RAM
	property_manager->type._bits.type = 0;				// Data

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Initialize user descriptor table event manager descriptor entry
*
* /param events			Pointer to an event manager descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_udt_event_manager_descriptor (descriptor_entry_t* event_manager)
{
	event_manager->type._bits.segment_type = 0;			// 32 bit
	event_manager->type._bits.privilege_level = 3;		// Ring 3
	event_manager->type._bits.in_memory = 1;			// in RAM
	event_manager->type._bits.type = 0;					// Data

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Initialize user descriptor table service manager descriptor entry
*
* /param service_manager 		Pointer to the service manager descriptor
*
* /return	SYSTEM_STATUS_SUCCESS if successful.
*			SYSTEM_FAILURE_GENERAL if an error is encountered.
*/
system_status_t
system_init_udt_service_manager_descriptor (descriptor_entry_t* service_manager)
{
	service_manager->type._bits.segment_type = 0;		// 32 bit
	service_manager->type._bits.privilege_level = 3;	// Ring 3
	service_manager->type._bits.in_memory = 1;			// in RAM
	service_manager->type._bits.type = 1;				// Code

	return SYSTEM_STATUS_SUCCESS;
}

