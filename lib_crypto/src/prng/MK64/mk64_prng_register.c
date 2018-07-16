
/**
* mk64_prng_register.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64 crypto PRNG register implementation.
*/


#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/prng.h>

extern const prng_vtable_t mk64_prng_vtable;

/**
* Register the MK64 PRNG crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t mk64_prng_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(PSEUDO_RNG);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(PSEUDO_RNG, (vtable_ptr_t)&mk64_prng_vtable));

} // mk64_prng_register




