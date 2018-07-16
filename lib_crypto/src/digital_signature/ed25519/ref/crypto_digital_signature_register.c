
/**
* crypto_digital_signature_register.c
*
* \copyright
* Copyright 2015 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief crypto digital signature register implementation.
*/


#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/digital_signature.h>

extern const digital_signature_vtable_t digital_signature_ed25519_vtable;
extern const digital_signature_vtable_t digital_signature_ed25519_donna_vtable;

/**
* Register the digital signature Ed25519 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t digital_signature_ed25519_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS 
		                     : crypto_registry_add(DIGITAL_SIGNATURE_ED25519, (vtable_ptr_t)&digital_signature_ed25519_vtable));

} // digial_signature_ed25519_register

/**
* Register the digital signature Ed25519 Donna crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t digital_signature_ed25519_donna_register(void)
{

    vtable_handle_t handle = crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);

    return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
        : crypto_registry_add(DIGITAL_SIGNATURE_ED25519_DONNA, (vtable_ptr_t)&digital_signature_ed25519_donna_vtable));

} // digital_signature_ed25519_donna_register

