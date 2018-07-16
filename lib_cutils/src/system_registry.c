
/**
* system_registry.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief system registry implementation.
*/

#include <aef/cutils/system_registry.h>
#include <stddef.h>
#include <string.h>
#include "bsp.h"

/**
* System registry macros
*/
#define SYSTEM_REGISTRY_CONTAINER_VALID(c)	((c) != NULL)
#define SYSTEM_REGISTRY_VALID(c)			(SYSTEM_REGISTRY_CONTAINER_VALID((c)) && ((c)->registry != NULL))

/**
* Create the system registry container.
*
* \returns  Handle to the registry container if the registry was created.
*
*           NULL on failure to create the registry.
*/
system_registry_t* system_registry_create(registry_type_t type)
{

	/**
	* Create the registry container
	*/
	uint32_t registry_table_size = 0;
	system_registry_t* registry_container = (system_registry_t*)malloc(sizeof(system_registry_t));

	if (SYSTEM_REGISTRY_CONTAINER_VALID(registry_container))
	{

		switch (type)
		{

			case REGISTRY_TYPE_VTABLE:
				registry_table_size = sizeof(vtable_entry_t) * SYSTEM_REGISTRY_SIZE_INITIAL;
				break;
			case REGISTRY_TYPE_UUID:
				registry_table_size = sizeof(uuid_entry_t) * SYSTEM_REGISTRY_SIZE_INITIAL;
				break;
			case REGISTRY_TYPE_STRING:
				registry_table_size = sizeof(string_entry_t) * SYSTEM_REGISTRY_SIZE_INITIAL;
				break;
			default:
				return NULL;

		} // switch

		/**
		* Create the registry table
		*/
		registry_container->registry = (registry_table_t*) malloc ( registry_table_size );

		if ( SYSTEM_REGISTRY_VALID(registry_container) )
		{

			registry_container->registry_id = SYSTEM_REGISTRY_DEFAULT_ID;
			registry_container->registry_type = type;
			registry_container->registry_entries = 0;
			memset(registry_container->registry, 0, registry_table_size);

			return registry_container;

		} // if

		/**
		* Failed to allocate the registry table, destroy the container
		*/
		free(registry_container);
		registry_container = NULL;

	} // if

	return registry_container;

} // system_registry_create

/**
* Destroy the system registry container.
*
* \returns REGISTRY_STATUS_SUCCESS if the registry was destroyed.
*
*          REGISTRY_FAILURE_GENERAL on failure to destroy the registry.
*/
registry_status_t system_registry_destroy (system_registry_t* registry_container)
{

	if ( SYSTEM_REGISTRY_CONTAINER_VALID(registry_container) )
	{

		if ( SYSTEM_REGISTRY_VALID(registry_container) )
		{
			free(registry_container->registry);
			registry_container->registry = NULL;
		}

		free(registry_container);

		return REGISTRY_STATUS_SUCCESS;

	}

	return REGISTRY_FAILURE_GENERAL;

} // system_registry_destroy

/**
* Add an item to a system registry container.
*
* \returns REGISTRY_STATUS_SUCCESS if the item was successfully added.
*
*          REGISTRY_FAILURE_GENERAL on failure to add the item to the registry.
*/
registry_status_t system_registry_add (system_registry_t* registry_container, registry_entry_t registry_item)
{


	if (SYSTEM_REGISTRY_VALID(registry_container))
	{

		switch (registry_container->registry_type)
		{

			case REGISTRY_TYPE_VTABLE:
				((vtable_entry_t*)registry_item)->handle = registry_container->registry_entries + 1;
				if (vtable_binary_insert(registry_container->registry,
					(vtable_entry_t*)registry_item,
					0,
					(int32_t)(registry_container->registry_entries - 1)) != VT_DUPLICATE_ENTRY)
				{

					registry_container->registry_entries++;
					return REGISTRY_STATUS_SUCCESS;

				} // if

				break;

			case REGISTRY_TYPE_UUID:
				((uuid_entry_t*)registry_item)->handle = registry_container->registry_entries + 1;
				if (uuid_binary_insert(registry_container->registry,
					(uuid_entry_t*)registry_item,
					0,
					(int32_t)(registry_container->registry_entries - 1)) != UUID_DUPLICATE_ENTRY)
				{

					registry_container->registry_entries++;
					return REGISTRY_STATUS_SUCCESS;

				} // if

				break;

			case REGISTRY_TYPE_STRING:
				((string_entry_t*)registry_item)->handle = registry_container->registry_entries + 1;
				if (string_binary_insert(registry_container->registry,
					(string_entry_t*)registry_item,
					0,
					(int32_t)(registry_container->registry_entries - 1)) != STRING_DUPLICATE_ENTRY)
				{

					registry_container->registry_entries++;
					return REGISTRY_STATUS_SUCCESS;

				} // if

				break;

		} // switch 

	} // if

	return REGISTRY_FAILURE_GENERAL;

} // system_registry_add

/**
* Retrieve an item from the system registry.
*
* \returns  The address of the item in the registry. Returns NULL
*           if the item is not in the registry.
*/
registry_entry_t system_registry_get_item (system_registry_t* registry_container, void* item)
{

	if (SYSTEM_REGISTRY_VALID(registry_container))
	{

		switch (registry_container->registry_type)
		{

			case REGISTRY_TYPE_VTABLE:
				return (registry_entry_t)
					vtable_binary_retrieve (registry_container->registry,
										    *(uint32_t*)item,
										    0,
										    (int32_t)(registry_container->registry_entries - 1));

			case REGISTRY_TYPE_UUID:
				return (registry_entry_t)
					uuid_binary_retrieve (registry_container->registry,
										  item,
										  0,
										  (int32_t)(registry_container->registry_entries - 1));

			case REGISTRY_TYPE_STRING:
				return (registry_entry_t)
					string_binary_retrieve (registry_container->registry,
										    item,
										    0,
										    (int32_t)(registry_container->registry_entries - 1));

		} // switch 

	} // if

	return (registry_entry_t)NULL;

}  // system_registry_get_item

/**
* Retrieve the number of items in the system registry.
*
* \returns  The number of items in the registry. Returns 0
*           if the registry has not been created.
*/
uint32_t system_registry_get_item_count (system_registry_t* registry_container)
{

	return ((SYSTEM_REGISTRY_CONTAINER_VALID(registry_container)) ? registry_container->registry_entries : 0);

} // system_registry_get_item_count

/**
* Iterate through all of the items in a registry table and call the callback function on each.
*
* \param	registry_container	Pointer to a registry
* \param	callback			Pointer to a callback function
* \param	param				Data to pass to the callback function
*
* \returns  A pointer to a registry item, or NULL.
*/
registry_entry_t system_registry_iterator(system_registry_t* registry_container, void* callback, uint32_t param)
{
	if (SYSTEM_REGISTRY_VALID(registry_container))
	{

		switch (registry_container->registry_type)
		{

			case REGISTRY_TYPE_VTABLE:
				return (registry_entry_t)
					vtable_table_iterator (registry_container->registry,
										   registry_container->registry_entries,
										   (vtable_item_callback)callback,
										   param);

			case REGISTRY_TYPE_UUID:
				return (registry_entry_t)
					uuid_table_iterator (registry_container->registry,
										 registry_container->registry_entries,
										 (uuid_item_callback)callback,
										 param);

			case REGISTRY_TYPE_STRING:
				return (registry_entry_t)
					string_table_iterator (registry_container->registry,
										   registry_container->registry_entries,
										   (string_item_callback)callback,
										   param);

		} // switch

	} // if

	return (registry_entry_t)NULL;
}
