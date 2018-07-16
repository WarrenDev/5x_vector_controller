
/**
* crypto_digital_signature_ed25519_impl.c
*
* \copyright
* Copyright 2015 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief crypto digital signature Ed25519 implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/digital_signature.h>
#include <aef/crypto/message_digest.h>
#include "string.h"
#include "../src/message_digest/sha2/ref/crypto_sha2_core.h"
#include "../src/message_digest/sha2/ref/crypto_sha2_register.h"
#include "crypto_ed25519_core.h"

#define	IMPL_DIGITAL_SIGNATURE_ID			DIGITAL_SIGNATURE_ED25519
#define IMPL_DIGEST_SIZE					SHA512_DIGEST_LENGTH
#define DIGITAL_SIGNATURE_CONTEXT_VALID		(_ctx != NULL)

/**
* Get the ID of this digital signature digest.
*
* \returns the message digest ID, which can be used to get the string name
*          of this digital signature digest.
*/
static
digital_signature_id_t digital_signature_get_id (void)
{
	return IMPL_DIGITAL_SIGNATURE_ID;
}

/**
* Get the output size for this digital signature digest.
*
* \returns the output size for this digital signature digest.
*/
static
size_t digital_signature_get_digest_size (void)
{
	return IMPL_DIGEST_SIZE;
}

/**
* Initialize this digital signature instance.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
static
crypto_status_t digital_signature_init (void)
{

	/**
	* Retrieve SHA512 vtable from the crypto registry
	*/
	message_digest_vtable_t* sha512_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (sha512_impl == NULL)
	{
		if (sha2_512_register() == CRYPTO_STATUS_SUCCESS)
		{
			sha512_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
		}
	}
	if (sha512_impl == NULL)
		return CRYPTO_FAILURE_INITIALIZATION;

	return CRYPTO_STATUS_SUCCESS;

}

/**
* Sign the key pair.
*
*
* \param pk				Public key
* \param sk				Secret key
* \param random_bytes	Random bytes
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
static
crypto_status_t digital_signature_sign_keypair(uint8_t* pk, uint8_t* sk, uint8_t* random_bytes)
{

	return crypto_sign_keypair (pk, sk, random_bytes);

}

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
static
crypto_status_t digital_signature_sign(uint8_t* sm, size_t* sm_size, const uint8_t* m, size_t m_size, const uint8_t* sk, const uint8_t* pk)
{

    return crypto_sign(sm, sm_size, m, m_size, sk);

}

/**
* SIgn_open
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
static
crypto_status_t digital_signature_sign_open (uint8_t* m, size_t* m_size, const uint8_t* sm, size_t sm_size, const uint8_t* pk)
{

	return crypto_sign_open (m, m_size, sm, sm_size, pk);

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
crypto_status_t digital_signature_selftest(uint8_t* scratch, size_t* scratch_size)
{
	return CRYPTO_STATUS_SUCCESS;
}

/**
* The digital signature vtable for Ed25519
*/
const digital_signature_vtable_t digital_signature_ed25519_vtable =
{
	digital_signature_get_id,
	digital_signature_get_digest_size,
	digital_signature_init,
	digital_signature_sign_keypair,
	digital_signature_sign,
	digital_signature_sign_open,
	digital_signature_selftest
};

