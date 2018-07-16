
/**
* crypto_sha2_256_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto SHA2 256 implementation.
*/

#include <aef/crypto/message_digest.h>
#include "string.h"
#include "crypto_sha2_core.h"

#define	IMPL_MESSAGE_DIGEST_ID			    MESSAGE_DIGEST_SHA2_256
#define IMPL_BLOCK_SIZE					    (SHA256_BLOCK_LENGTH * 8)
#define IMPL_DIGEST_SIZE				    SHA256_DIGEST_LENGTH
#define MESSAGE_DIGEST_CONTEXT_VALID(_ctx)	((_ctx) != NULL)

/**
* Get the ID of this message digest.
*
* \returns the message digest ID, which can be used to get the string name
*          of this cipher.
*/
static
message_digest_id_t sha2_256_get_message_digest_id(void)
{
	return IMPL_MESSAGE_DIGEST_ID;
}

/**
* Get the input block size of this message digest.
*
* \returns the input block size of this message digest.
*/
static
size_t sha2_256_get_block_size(void)
{

	return IMPL_BLOCK_SIZE;
}

/**
* Get the output size for this message digest.
*
* \returns the output size for this message digest.
*/
static
size_t sha2_256_get_digest_size(void)
{
	return IMPL_DIGEST_SIZE;
}

/**
* Initialize this message digest.
*
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this digest.
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
crypto_status_t sha2_256_init(message_digest_ctx_t ctx, size_t* ctx_size)
{

    SHA2_CTX* _ctx = (SHA2_CTX*)ctx;
    
    /**
	* Validate parameters
	*/
	if (*ctx_size < sizeof(SHA2_CTX))
	{
		*ctx_size = sizeof(SHA2_CTX);
		return CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE;
	}

	/**
	* Initialize the context
	*/
	memset(_ctx, 0, *ctx_size);

	/**
	* Initialize
	*/
	SHA256Init(_ctx);
	return CRYPTO_STATUS_SUCCESS;

}

/**
* Update this message digest with the given input data.
*
* The internal state of this message digest is updated with the given
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
crypto_status_t sha2_256_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size)
{
    SHA2_CTX* _ctx = (SHA2_CTX*)ctx;

    SHA256Update(_ctx, input, input_size);
	return CRYPTO_STATUS_SUCCESS;
}

/**
* Finalize this message digest and write the output hash.
*
* \param ctx           The context.
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
crypto_status_t sha2_256_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size)
{

    SHA2_CTX* _ctx = (SHA2_CTX*)ctx;
    
    if (*output_size != SHA256_DIGEST_LENGTH)
	{
		*output_size = SHA256_DIGEST_LENGTH;
		return CRYPTO_FAILURE_INSUFFICIENT_SIZE;
	}

	SHA256Final(output, _ctx);
	return CRYPTO_STATUS_SUCCESS;
}

/**
* Perform a self-test of this message digest with the provided scratchpad.
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
crypto_status_t sha2_256_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{
	return CRYPTO_STATUS_SUCCESS;
}

/**
* The message digest vtable for this message digest
*/
const message_digest_vtable_t sha2_256_vtable =
{
	sha2_256_get_message_digest_id,
	sha2_256_get_block_size,
	sha2_256_get_digest_size,
	sha2_256_init,
	sha2_256_update,
	sha2_256_finalize,
	sha2_256_selftest
};

