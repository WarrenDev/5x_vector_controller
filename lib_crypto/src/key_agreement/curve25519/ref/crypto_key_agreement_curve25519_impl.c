
/**
* crypto_key_agreement_curve25519_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto key agreement Curve25519 implementation.
*/

#include <aef/crypto/key_agreement.h>
#include "string.h"
#include "crypto_curve25519_core.h"

#define	IMPL_KEY_AGREEMENT_ID			KEY_AGREEMENT_CURVE25519

/**
* Get the ID of this digital signature digest.
*
* \returns the message digest ID, which can be used to get the string name
*          of this digital signature digest.
*/
static
key_agreement_id_t key_agreement_get_id(void)
{
	return IMPL_KEY_AGREEMENT_ID;
}

/**
* Create secret
*
*
* \param secret_key			Secret key
* \param input_bytes		input bytes
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
static
crypto_status_t key_agreement_create_secret (uint8_t* secret_key, const uint8_t* input_bytes)
{

	return curve25519_create_secret (secret_key, input_bytes);
}

/**
* Create public
*
*
* \param public_key			Public key
* \param secret_key			Secret key
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
static
crypto_status_t key_agreement_create_public (uint8_t* public_key, const uint8_t* secret_key)
{

	return curve25519_create_public (public_key, secret_key);

}

/**
* Create shared secret
*
* \param shared_secret
* \param peer_public_key
* \param my_secret_key
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
static
crypto_status_t key_agreement_create_shared_secret (uint8_t* shared_secret, const uint8_t* peer_public_key, const uint8_t* my_secret_key)
{

	return curve25519_create_shared_secret (shared_secret, peer_public_key, my_secret_key);

}

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
static
crypto_status_t key_agreement_selftest (uint8_t* scratch, size_t* scratch_size)
{
	return CRYPTO_STATUS_SUCCESS;
}

/**
* The key agreement vtable for Curve25519
*/
const key_agreement_vtable_t key_agreement_curve25519_vtable =
{
	key_agreement_get_id,
	key_agreement_create_secret,
	key_agreement_create_public,
	key_agreement_create_shared_secret,
	key_agreement_selftest
};

