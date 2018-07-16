
/**
* crypto_registry.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto registry implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

/**
* Crypto registry container
*/
static system_registry_t* crypto_registry = NULL;

/**
* Crypto registry macros
*/
#define CRYPTO_REGISTRY_CONTAINER_VALID	(crypto_registry != NULL)
#define CRYPTO_REGISTRY_VALID			(CRYPTO_REGISTRY_CONTAINER_VALID && (crypto_registry->registry != NULL))

/**
* Create the crypto registry container.
*
* \returns CRYPTO_STATUS_SUCCESS if the registry was created.
*
*          CRYPTO_FAILURE_GENERAL on failure to create the registry.
*/
crypto_status_t  crypto_registry_create (void)
{

	/**
	* Don't create if already in existance
	*/
	if ( CRYPTO_REGISTRY_CONTAINER_VALID )
		return CRYPTO_STATUS_SUCCESS;

	crypto_registry = system_registry_create(REGISTRY_ID_VTABLE);

	return ( (CRYPTO_REGISTRY_VALID) ? CRYPTO_STATUS_SUCCESS : CRYPTO_FAILURE_GENERAL );

} // crypto_registry_create

/**
* Destroy the crypto registry container.
*
* \returns CRYPTO_STATUS_SUCCESS if the registry was destroyed.
*
*          CRYPTO_FAILURE_GENERAL on failure to destroy the registry.
*/
crypto_status_t  crypto_registry_destroy ( void )
{

	registry_status_t status = system_registry_destroy(crypto_registry);

	if (status == REGISTRY_STATUS_SUCCESS)
		crypto_registry = NULL;

	return ( (status == REGISTRY_STATUS_SUCCESS) ? CRYPTO_STATUS_SUCCESS : CRYPTO_FAILURE_GENERAL );

} // crypto_registry_destroy

/**
* Add an item to the crypto registry container.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t  crypto_registry_add (uint32_t crypto_id, vtable_ptr_t item_ptr)
{

	registry_status_t status;
	vtable_entry_t crypto_registry_item;

	memset(&crypto_registry_item, 0, sizeof(vtable_entry_t));
	crypto_registry_item.vtableId = crypto_id;
	crypto_registry_item.vtablePtr = item_ptr;

	status = system_registry_add(crypto_registry, (registry_entry_t)&crypto_registry_item);

	return ((status == REGISTRY_STATUS_SUCCESS) ? CRYPTO_STATUS_SUCCESS : CRYPTO_FAILURE_GENERAL);

} // crypto_registry_add

/**
* Retrieve an item from the crypto registry.
*
* \returns  The address of the item in the registry. Returns NULL
*           if the item is not in the registry.
*/
vtable_entry_t*  crypto_registry_get_item(uint32_t crypto_id)
{

	if (CRYPTO_REGISTRY_VALID)
	{
		return (vtable_entry_t*)system_registry_get_item(crypto_registry, (void*)&crypto_id);
	}

	return (vtable_entry_t*)NULL;

}  // crypto_registry_get_item

/**
* Retrieve a crypto cipher vtable pointer from the crypto registry.
*
* \returns  The vtable pointer of the item in the registry. Returns NULL
*           if the item is not in the registry.
*/
vtable_handle_t	crypto_registry_get_vtable(uint32_t crypto_id)
{

	vtable_entry_t*	registry_item = crypto_registry_get_item( crypto_id );

	return ((registry_item != NULL) ? (vtable_handle_t)registry_item->vtablePtr : (vtable_handle_t)NULL);

} // crypto_registry_get_vtable

/**
* Retrieve the number of items in the crypto registry.
*
* \returns  The number of items in the registry. Returns 0
*           if the registy has not been created.
*/
uint32_t  crypto_registry_get_item_count(void)
{

	return system_registry_get_item_count(crypto_registry);

} // crypto_registry_get_item_count
