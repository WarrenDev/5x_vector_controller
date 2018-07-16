
/**
* crypto_message_authentication_384_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto message authentication 384 implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/message_authentication.h>
#include "string.h"
#include "crypto_message_authentication_core.h"

#define	IMPL_MESSAGE_AUTHENTICATION_ID			    MESSAGE_AUTHENTICATION_384
#define IMPL_BLOCK_SIZE							    (SHA384_BLOCK_LENGTH * 8)
#define IMPL_DIGEST_SIZE						    SHA384_DIGEST_LENGTH
#define MESSAGE_AUTHENTICATION_CONTEXT_VALID(_ctx)	((_ctx) != NULL)

#define INNER_KEY_PAD							0x36
#define OUTER_KEY_PAD							0x5C

/**
* Get the ID of this message authentication digest.
*
* \returns the message digest ID, which can be used to get the string name
*          of this message authentication digest.
*/
static
message_authentication_id_t message_authentication_get_id(void)
{
	return IMPL_MESSAGE_AUTHENTICATION_ID;
}

/**
* Get the input block size of this message authentication digest.
*
* \returns the input block size of this message authentication digest.
*/
static
size_t message_authentication_get_block_size(void)
{

	return IMPL_BLOCK_SIZE;
}

/**
* Get the output size for this message authentication digest.
*
* \returns the output size for this message authentication digest.
*/
static
size_t message_authentication_get_digest_size(void)
{
	return IMPL_DIGEST_SIZE;
}

/**
* Initialize this message digest.
*
* \param key           The key to use for this message authentication digest.
* \param key_size      The size of the key for this message authentication digest.
* \param ctx           The context to initialize.
* \param ctx_size      The size of the context for this digest.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this digest.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this digest.
*/
static
crypto_status_t message_authentication_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size)
{

    MESSAGE_AUTHENTICATION_CTX* _ctx = (MESSAGE_AUTHENTICATION_CTX*)ctx;

    /**
    * Validate parameters
    */
    if (*ctx_size < sizeof(MESSAGE_AUTHENTICATION_CTX))
    {
        *ctx_size = sizeof(MESSAGE_AUTHENTICATION_CTX);
        return CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE;
    }

    /**
	* Initialize the context
	*/
	memset(_ctx, 0, *ctx_size);
    _ctx->sha2_context_size = sizeof(SHA2_CTX);

	/**
	* Retrieve SHA384 vtable from the crypto registry
	*/
	message_digest_vtable_t* sha384_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (sha384_impl == NULL)
	{
		if (sha2_384_register() == CRYPTO_STATUS_SUCCESS)
		{
			sha384_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
		}
	}
	if (sha384_impl == NULL)
		return CRYPTO_FAILURE_INITIALIZATION;

	/**
	* Prepare authentication key
	*/
	int     index;
	uint8_t keyBuffer[SHA384_BLOCK_LENGTH];
	size_t  keyBufferSize = sizeof(keyBuffer);
	memset(keyBuffer, 0, keyBufferSize);

	if (key_size > SHA384_BLOCK_LENGTH)
	{

		sha384_impl->init(&_ctx->sha2_context, &_ctx->sha2_context_size);
        sha384_impl->update(&_ctx->sha2_context, key, key_size);
        sha384_impl->finalize(&_ctx->sha2_context, keyBuffer, &keyBufferSize);
	}
	else
	{
		memcpy(keyBuffer, key, key_size);
	}

	for (index = 0; index < SHA384_BLOCK_LENGTH; index++)
	{
		keyBuffer[index] ^= INNER_KEY_PAD;
	}

	/**
	* Initialize
	*/
    sha384_impl->init(&_ctx->sha2_context, &_ctx->sha2_context_size);
    sha384_impl->update(&_ctx->sha2_context, keyBuffer, keyBufferSize);
	memset(keyBuffer, 0, keyBufferSize);

	return CRYPTO_STATUS_SUCCESS;

}

/**
* Update this message authentication digest with the given input data.
*
* The internal state of this message authentication digest is updated with the given
* input data.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
static
crypto_status_t message_authentication_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size)
{

    MESSAGE_AUTHENTICATION_CTX* _ctx = (MESSAGE_AUTHENTICATION_CTX*)ctx;

    message_digest_vtable_t* sha384_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (sha384_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

    sha384_impl->update(&_ctx->sha2_context, input, input_size);
	return CRYPTO_STATUS_SUCCESS;
}

/**
* Finalize this message authentication digest and write the output hash.
*
* \param ctx           The context.
* \param key           The key to use for this message authentication digest.
* \param key_size      The size of the key for this message authentication digest.
* \param output        The output buffer to store the message digest.
* \param output_size   The size of the output buffer.
*
* \returns CRYPTO_SUCCESS if finalization completed successfully.
*
*          CRYPTO_FAILURE_INSUFFICIENT_SIZE if the output buffer is
*              insufficient to perform this operation.  The required
*              output size is stored in the output_size parameter.
*/
static
crypto_status_t message_authentication_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size)
{

    MESSAGE_AUTHENTICATION_CTX* _ctx = (MESSAGE_AUTHENTICATION_CTX*)ctx;

    if (*output_size != SHA384_DIGEST_LENGTH)
	{
		*output_size = SHA384_DIGEST_LENGTH;
		return CRYPTO_FAILURE_INSUFFICIENT_SIZE;
	}

	/**
	* Retrieve SHA384 vtable from the crypto registry
	*/
	message_digest_vtable_t* sha384_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (sha384_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	/**
	* Prepare authentication key
	*/
	int     index;
	uint8_t keyBuffer[SHA384_BLOCK_LENGTH];
	size_t  keyBufferSize = sizeof(keyBuffer);
	memset(keyBuffer, 0, keyBufferSize);

	if (key_size > SHA384_BLOCK_LENGTH)
	{
		SHA2_CTX  local_ctx;
		size_t	  local_ctx_size = sizeof(SHA2_CTX);
		sha384_impl->init(&local_ctx, &local_ctx_size);
		sha384_impl->update(&local_ctx, key, key_size);
		sha384_impl->finalize(&local_ctx, keyBuffer, &keyBufferSize);
	}
	else
	{
		memcpy(keyBuffer, key, key_size);
	}

	for (index = 0; index < SHA384_BLOCK_LENGTH; index++)
	{
		keyBuffer[index] ^= OUTER_KEY_PAD;
	}

	/**
	* Finalize HMAC digest
	*/
    sha384_impl->finalize(&_ctx->sha2_context, output, output_size);

    sha384_impl->init(&_ctx->sha2_context, &_ctx->sha2_context_size);
    sha384_impl->update(&_ctx->sha2_context, keyBuffer, keyBufferSize);
    sha384_impl->update(&_ctx->sha2_context, output, *output_size);
    sha384_impl->finalize(&_ctx->sha2_context, output, output_size);
	memset(keyBuffer, 0, keyBufferSize);

	return CRYPTO_STATUS_SUCCESS;
}

/**
* Perform a self-test of this message authentication digest with the provided scratchpad.
*
* \param ctx               The context.
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
crypto_status_t message_authentication_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{
	return CRYPTO_STATUS_SUCCESS;
}

/**
* The message digest vtable for this message digest
*/
const message_authentication_vtable_t message_authentication_384_vtable =
{
	message_authentication_get_id,
	message_authentication_get_block_size,
	message_authentication_get_digest_size,
	message_authentication_init,
	message_authentication_update,
	message_authentication_finalize,
	message_authentication_selftest
};

