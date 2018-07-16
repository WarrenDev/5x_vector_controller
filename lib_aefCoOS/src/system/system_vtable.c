
/**
* system_vtable.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementation of system vtable management.
*/

#include <aef/embedded/system/system_vtable.h>
#include <stddef.h>
#include <CoOS.h>
#include <string.h>

/**
* Create the vtable container.
*
* \param	vtable_container	Pointer to the system_registry_t container
*
* \returns SYSTEM_STATUS_SUCCESS if the registry was created.
*          SYSTEM_FAILURE_GENERAL on failure to create the registry.
*          SYSTEM_FAILURE_INITIALIZATION if the initial vtable_container pointer
*          passed in is not NULL.  It must be initialized by the caller to NULL.
*/
system_status_t system_vtable_create (system_registry_t** vtable_container)
{

	if ( *vtable_container != NULL )
		return SYSTEM_FAILURE_INITIALIZATION;

	*vtable_container = system_registry_create(REGISTRY_ID_VTABLE);

	return ( (*vtable_container != NULL) ? SYSTEM_STATUS_SUCCESS : SYSTEM_FAILURE_GENERAL );

} // system_vtable_create

/**
* Destroy the vtable container.
*
* \param	vtable_container	Pointer to the system_registry_t container
*
* \returns SYSTEM_STATUS_SUCCESS if the registry was destroyed.
*          SYSTEM_FAILURE_GENERAL on failure to destroy the registry.
*/
system_status_t system_vtable_destroy (system_registry_t* vtable_container)
{

	registry_status_t status = system_registry_destroy(vtable_container);

	return ( (status == REGISTRY_STATUS_SUCCESS) ? SYSTEM_STATUS_SUCCESS : SYSTEM_FAILURE_GENERAL );

} // system_vtable_destroy

/**
* Add an item to the vtable container.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	vtable_id			Id of the new item
* \param	item_ptr			Pointer to the new vtable_ptr_t item
*
* \returns SYSTEM_STATUS_SUCCESS if the item was successfully added.
*          SYSTEM_FAILURE_GENERAL on failure to add the item to the registry.
*/
system_status_t system_vtable_add (system_registry_t* vtable_container, uint32_t vtable_id, const vtable_ptr_t item_ptr)
{

	registry_status_t status;
	vtable_entry_t vtable_item;

	memset(&vtable_item, 0, sizeof(vtable_entry_t));
	vtable_item.vtableId = vtable_id;
	vtable_item.vtablePtr = item_ptr;

	status = system_registry_add(vtable_container, (registry_entry_t)&vtable_item);

	return ((status == REGISTRY_STATUS_SUCCESS) ? SYSTEM_STATUS_SUCCESS : SYSTEM_FAILURE_GENERAL);

} // system_vtable_add

/**
* Retrieve an item from the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	vtable_id			Id of the item to retrieve
*
* \returns  The address of the item in the registry.
* 			NULL if the item is not in the registry.
*/
vtable_entry_t* system_vtable_get_item(system_registry_t* vtable_container, uint32_t vtable_id)
{

	if (vtable_container != NULL)
	{
		return (vtable_entry_t*)system_registry_get_item(vtable_container, (void*)&vtable_id);
	}

	return (vtable_entry_t*)NULL;

}  // system_vtable_get_item

/**
* Retrieve a vtable pointer from the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	vtable_id			Id of the item to retrieve
*
* \returns  The vtable pointer of the item in the registry.
* 			NULL if the item is not in the registry.
*/
vtable_ptr_t system_vtable_get_vtable(system_registry_t* vtable_container, uint32_t vtable_id)
{

	vtable_entry_t*	vtable_item = system_vtable_get_item( vtable_container, vtable_id );

	return ((vtable_item != NULL) ? (vtable_ptr_t)vtable_item->vtablePtr : (vtable_ptr_t)NULL);

} // system_vtable_get_vtable

/**
* Retrieve the number of items in the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
*
* \returns  The number of items in the registry. Returns
* 			0 if the registry has not been created.
*/
uint32_t system_vtable_get_item_count(system_registry_t* vtable_container)
{
	return system_registry_get_item_count(vtable_container);
} // system_vtable_get_item_count

/**
* Iterate through the items in the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	callback			Pointer to function to call on each item
* \param	param				Parameter to pass to the callback function
*
* \returns  A pointer to a registry item, or NULL.
*/
void* system_vtable_iterator (system_registry_t* vtable_container, vtable_item_callback callback, uint32_t param)
{
	return system_registry_iterator(vtable_container, callback, param);
}

