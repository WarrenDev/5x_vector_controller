
/**
* ll_crypto_digital_signature.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto digital signature API implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_digital_signature.h>
#include "stddef.h"
#include "../src/digital_signature/ed25519/ref/crypto_digital_signature_register.h"

/**
* Register the crypto digital signature ed25519 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t
ll_crypto_register_digital_signature_ed25519(void)
{

	return digital_signature_ed25519_register();

} // ll_crypto_register_digital_signature_ed25519

/**
* Get the ID of digital signature ed25519.
*
* \returns the digital signature ID, which can be used to get the string name
*          of this digital signature algorithm.
*/
digital_signature_id_t
ll_crypto_digital_signature_ed25519_get_id(void)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return 0;

	return digital_signature_impl->id();

} // ll_crypto_digital_signature_ed25519_get_id

/**
* Get the output size for this digital signature digest.
*
* \returns the output size for this digital signature digest.
*/
size_t 
ll_crypto_digital_signature_ed25519_get_digest_size(void)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return 0;

	return digital_signature_impl->digest_size();

} // ll_crypto_digital_signature_ed25519_get_digest_size

/**
* Initialize this digital signature instance.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t 
ll_crypto_digital_signature_ed25519_init(void)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return 0;

	return digital_signature_impl->init();

} // ll_crypto_digital_signature_ed25519_init

/**
* Sign the key pair.
*
* Create a new key pair from the given random bytes.  
*
* \param pk				Public key
* \param sk				Secret key
* \param random_bytes	Random bytes
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t 
ll_crypto_digital_signature_ed25519_sign_keypair(uint8_t* pk, uint8_t* sk, uint8_t* random_bytes)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->sign_keypair(pk, sk, random_bytes);

} // ll_crypto_digital_signature_ed25519_sign_keypair

/**
* Sign a message
*
*
* \param sm         Signaure returned
* \param sm_size    Size of the signature in bytes
* \param m			Message to sign
* \param m_size		Size of the message
* \param sk			Secret key
* \param pk         Public key (not used in this implementation)
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_digital_signature_ed25519_sign(uint8_t* sm, size_t* sm_size, const uint8_t* m, size_t m_size, const uint8_t* sk, const uint8_t* pk)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->sign( sm, sm_size, m, m_size, sk, pk );

} // ll_crypto_digital_signature_ed25519_sign

/**
* SIgn_open
*
* Verifies the signature on the given message using public_key.
*
* \param m
* \param m_size
* \param sm
* \param sm_size
* \param pk
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_digital_signature_ed25519_sign_open(uint8_t* m, size_t* m_size, const uint8_t* sm, size_t sm_size, const uint8_t* pk)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->sign_open ( m, m_size, sm, sm_size, pk );

} // ll_crypto_digital_signature_ed25519_sign_open

/**
* Perform a self-test of this digital signature digest with the provided scratchpad.
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
ll_crypto_digital_signature_ed25519_selftest(uint8_t* scratch, size_t* scratch_size)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->selftest ( scratch, scratch_size );

} // ll_crypto_digital_signature_ed25519_selftest

//=========

/**
* Register the crypto digital signature ed25519 donna sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t
ll_crypto_register_digital_signature_ed25519_donna(void)
{

	return digital_signature_ed25519_donna_register();

} // ll_crypto_register_digital_signature_ed25519_donna

/**
* Get the ID of digital signature ed25519 donna.
*
* \returns the digital signature ID, which can be used to get the string name
*          of this digital signature algorithm.
*/
digital_signature_id_t
ll_crypto_digital_signature_ed25519_donna_get_id(void)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return 0;

	return digital_signature_impl->id();

} // ll_crypto_digital_signature_ed25519_donna_get_id

/**
* Get the output size for this digital signature digest.
*
* \returns the output size for this digital signature digest.
*/
size_t
ll_crypto_digital_signature_ed25519_donna_get_digest_size(void)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return 0;

	return digital_signature_impl->digest_size();

} // ll_crypto_digital_signature_ed25519_donna_get_digest_size

/**
* Initialize this digital signature instance.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_digital_signature_ed25519_donna_init(void)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return 0;

	return digital_signature_impl->init();

} // ll_crypto_digital_signature_ed25519_donna_init

/**
* Sign the key pair.
*
* Create a new key pair from the given random bytes.
*
* \param pk				Public key
* \param sk				Secret key
* \param random_bytes	Random bytes
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_digital_signature_ed25519_donna_sign_keypair(uint8_t* pk, uint8_t* sk, uint8_t* random_bytes)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->sign_keypair(pk, sk, random_bytes);

} // ll_crypto_digital_signature_ed25519_donna_sign_keypair

/**
* Sign a message
*
*
* \param sm         Signature returned
* \param sm_size    Size of the signature in bytes
* \param m			Message to sign
* \param m_size		Size of the message
* \param sk			Secret key
* \param pk         Public key (not used in this implementation)
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_digital_signature_ed25519_donna_sign(uint8_t* sm, size_t* sm_size, const uint8_t* m, size_t m_size, const uint8_t* sk, const uint8_t* pk)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->sign( sm, sm_size, m, m_size, sk, pk );

} // ll_crypto_digital_signature_ed25519_donna_sign

/**
* Verify signature
*
* Verifies the signature on the given message using the public key..
*
* \param m
* \param m_size
* \param sm
* \param sm_size
* \param pk
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_digital_signature_ed25519_donna_sign_open(uint8_t* m, size_t* m_size, const uint8_t* sm, size_t sm_size, const uint8_t* pk)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->sign_open ( m, m_size, sm, sm_size, pk );

} // ll_crypto_digital_signature_ed25519_donna_sign_open

/**
* Perform a self-test of this digital signature digest with the provided scratchpad.
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
ll_crypto_digital_signature_ed25519_donna_selftest(uint8_t* scratch, size_t* scratch_size)
{

	digital_signature_vtable_t* digital_signature_impl = (digital_signature_vtable_t*)crypto_registry_get_vtable(DIGITAL_SIGNATURE_ED25519_DONNA);
	if (digital_signature_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return digital_signature_impl->selftest ( scratch, scratch_size );

} // ll_crypto_digital_signature_ed25519_donna_selftest

