
/**
* system_vtable.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used for system vtable management.
*/

#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_VTABLE_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_VTABLE_H_

#include <aef/cutils/system_registry.h>
#include <aef/cutils/vtable_array.h>
#include <aef/embedded/system/system_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* Create the vtable container.
*
* \param	vtable_container	Pointer to the system_registry_t container
*
* \returns SYSTEM_STATUS_SUCCESS if the registry was created.
*          SYSTEM_FAILURE_GENERAL on failure to create the registry.
*/
system_status_t
system_vtable_create (system_registry_t** vtable_container);

/**
* Destroy the vtable container.
*
* \param	vtable_container	Pointer to the system_registry_t container
*
* \returns SYSTEM_STATUS_SUCCESS if the registry was destroyed.
*          SYSTEM_FAILURE_GENERAL on failure to destroy the registry.
*/
system_status_t
system_vtable_destroy (system_registry_t* vtable_container);

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
system_status_t
system_vtable_add (system_registry_t* vtable_container, uint32_t vtable_id, const vtable_ptr_t item_ptr);

/**
* Retrieve an item from the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	vtable_id			Id of the item to retrieve
*
* \returns  The address of the item in the registry. Returns NULL
*           if the item is not in the registry.
*/
vtable_entry_t*
system_vtable_get_item (system_registry_t* vtable_container, uint32_t vtable_id);

/**
* Retrieve a vtable pointer from the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	vtable_id			Id of the item to retrieve
*
* \returns  The vtable pointer of the item in the registry. Returns NULL
*           if the item is not in the registry.
*/
vtable_ptr_t
system_vtable_get_vtable (system_registry_t* vtable_container, uint32_t vtable_id);

/**
* Retrieve the number of items in the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
*
* \returns  The number of items in the registry. Returns 0
*           if the registry has not been created.
*/
uint32_t
system_vtable_get_item_count (system_registry_t* vtable_container);

/**
* Iterate through the items in the vtable registry.
*
* \param	vtable_container	Pointer to the system_registry_t container
* \param	callback			Pointer to function to call on each item
* \param	param				Parameter to pass to the callback function
*
* \returns  A pointer to a registry item, or NULL.
*/
void*
system_vtable_iterator (system_registry_t* vtable_container, vtable_item_callback callback, uint32_t param);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_VTABLE_H_ */
