
/**
* crypto_key_agreement_register.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto key agreement register implementation.
*/


#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/key_agreement.h>

extern const key_agreement_vtable_t key_agreement_curve25519_vtable;

/**
* Register the key agreement curve25519 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t key_agreement_curve25519_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(KEY_AGREEMENT_CURVE25519);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(KEY_AGREEMENT_CURVE25519, (vtable_ptr_t)&key_agreement_curve25519_vtable));

} // key_agreement_curve25519_register

