
/**
* ll_crypto_key_agreement.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto key agreement API implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_key_agreement.h>
#include "stddef.h"
#include "../src/key_agreement/curve25519/ref/crypto_key_agreement_register.h"

/**
* Register the crypto key_agreement curve25519 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  
ll_crypto_register_key_agreement_curve25519(void)
{

	return key_agreement_curve25519_register();

} // ll_crypto_register_key_agreement_curve25519

/**
* Get the ID of key_agreement curve25519.
*
* \returns the key_agreement ID, which can be used to get the string name
*          of this key agreement algorithm.
*/
key_agreement_id_t
ll_crypto_key_agreement_curve25519_get_id(void)
{

	key_agreement_vtable_t* key_agreement_impl = (key_agreement_vtable_t*)crypto_registry_get_vtable(KEY_AGREEMENT_CURVE25519);
	if (key_agreement_impl == NULL)
		return 0;

	return key_agreement_impl->id ();

} // ll_crypto_key_agreement_curve25519_get_id

/**
* create secret.
*
* Create secret key from random input bytes
*
* \param secret_key
* \param input_bytes
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_key_agreement_curve25519_create_secret(uint8_t* secret_key, const uint8_t* input_bytes)
{

	key_agreement_vtable_t* key_agreement_impl = (key_agreement_vtable_t*)crypto_registry_get_vtable(KEY_AGREEMENT_CURVE25519);
	if (key_agreement_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return key_agreement_impl->create_secret (secret_key, input_bytes);

} // ll_crypto_key_agreement_curve25519_create_secret

/**
* Create public
*
* Using the generated secret key create the public key.
*
* \param public_key
* \param secret_key
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_key_agreement_curve25519_create_public(uint8_t* public_key, const uint8_t* secret_key)
{

	key_agreement_vtable_t* key_agreement_impl = (key_agreement_vtable_t*)crypto_registry_get_vtable(KEY_AGREEMENT_CURVE25519);
	if (key_agreement_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return key_agreement_impl->create_public (public_key, secret_key);

} // ll_crypto_key_agreement_curve25519_create_public

/**
* Create shared secret
*
* Using the generated keys create the shared secret.
*
* \param shared_secret
* \param peer_public_key
* \param my_secret_key
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_key_agreement_curve25519_create_shared_secret(uint8_t* shared_secret, const uint8_t* peer_public_key, const uint8_t* my_secret_key)
{

	key_agreement_vtable_t* key_agreement_impl = (key_agreement_vtable_t*)crypto_registry_get_vtable(KEY_AGREEMENT_CURVE25519);
	if (key_agreement_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return key_agreement_impl->create_shared_secret (shared_secret, peer_public_key, my_secret_key);

} // ll_crypto_key_agreement_curve25519_create_shared_secret

/**
* Perform a self-test of this key agreement digest with the provided scratchpad.
*
* \param scratch           The scratchpad to use for self-testing.
* \param scratch_size      The size of the scratchpad.
*
* \returns CRYPTO_STATUS_SUCCESS on a successful self-test.
*
*          CRYPTO_FAILURE_INSUFFICIENT_SIZE if the scratchpad is not
*              large enough for a self-test.  The required size will be
*              written to scratch_size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this implementation
*              does not support self testing.
*
*          CRYPTO_FAILURE_SELF_TEST if the self test fails.
*/
crypto_status_t
ll_crypto_key_agreement_curve25519_selftest(uint8_t* scratch, size_t* scratch_size)
{

	key_agreement_vtable_t* key_agreement_impl = (key_agreement_vtable_t*)crypto_registry_get_vtable(KEY_AGREEMENT_CURVE25519);
	if (key_agreement_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return key_agreement_impl->selftest (scratch, scratch_size);

} // ll_crypto_key_agreement_curve25519_selftest

