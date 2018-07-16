
/**
* ll_crypto_message_digest.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto message digest API implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_message_digest.h>
#include "stddef.h"
#include "../src/message_digest/sha2/ref/crypto_sha2_register.h"

/**
* Register the crypto sha2 224 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_sha224(void)
{

	return sha2_224_register();

} // ll_crypto_register_sha224

/**
* Register the crypto sha2 256 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_sha256(void)
{

	return sha2_256_register();

} // ll_crypto_register_sha256

/**
* Register the crypto sha2 384 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_sha384(void)
{

	return sha2_384_register();

} // ll_crypto_register_sha384

/**
* Register the crypto sha2 512 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_sha512(void)
{

	return sha2_512_register();

} // ll_crypto_register_sha512


/**
* Get the ID of message digest 224.
*
* \returns the message digest ID, which can be used to get the string name
*          of this message digest.
*/
message_digest_id_t	ll_crypto_message_digest_224_get_id(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->id();

} // ll_crypto_message_digest_224_get_id

/**
* Get the ID of message digest 256.
*
* \returns the message digest ID, which can be used to get the string name
*          of this message digest.
*/
message_digest_id_t	ll_crypto_message_digest_256_get_id(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->id();

} // ll_crypto_message_digest_256_get_id

/**
* Get the ID of message digest 384.
*
* \returns the message digest ID, which can be used to get the string name
*          of this message digest.
*/
message_digest_id_t	ll_crypto_message_digest_384_get_id(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->id();

} // ll_crypto_message_digest_384_get_id

/**
* Get the ID of message digest 512.
*
* \returns the message digest ID, which can be used to get the string name
*          of this message digest.
*/
message_digest_id_t	ll_crypto_message_digest_512_get_id(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->id();

} // ll_crypto_message_digest_512_get_id

/**
* Get the input block size of message digest 224.
*
* \returns the input block size of this message digest.
*/
size_t	ll_crypto_message_digest_224_block_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->block_size();

} // ll_crypto_message_digest_224_block_size

/**
* Get the input block size of message digest 256.
*
* \returns the input block size of this message digest.
*/
size_t	ll_crypto_message_digest_256_block_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->block_size();

} // ll_crypto_message_digest_256_block_size

/**
* Get the input block size of message digest 384.
*
* \returns the input block size of this message digest.
*/
size_t	ll_crypto_message_digest_384_block_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->block_size();

} // ll_crypto_message_digest_384_block_size

/**
* Get the input block size of message digest 512.
*
* \returns the input block size of this message digest.
*/
size_t	ll_crypto_message_digest_512_block_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->block_size();

} // ll_crypto_message_digest_512_block_size

/**
* Get the digest size of message digest 224.
*
* \returns the digest size of this message digest.
*/
size_t	ll_crypto_message_digest_224_digest_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->digest_size();

} // ll_crypto_message_digest_224_digest_size

/**
* Get the digest size of message digest 256.
*
* \returns the digest size of this message digest.
*/
size_t	ll_crypto_message_digest_256_digest_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->digest_size();

} // ll_crypto_message_digest_256_digest_size

/**
* Get the digest size of message digest 384.
*
* \returns the digest size of this message digest.
*/
size_t	ll_crypto_message_digest_384_digest_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->digest_size();

} // ll_crypto_message_digest_384_digest_size

/**
* Get the digest size of message digest 512.
*
* \returns the digest size of this message digest.
*/
size_t	ll_crypto_message_digest_512_digest_size(void)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return 0;

	return message_digest_impl->digest_size();

} // ll_crypto_message_digest_512_digest_size

/**
* Initialize message digest 224.
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
crypto_status_t	ll_crypto_message_digest_224_init(message_digest_ctx_t ctx, size_t* ctx_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->init(ctx, ctx_size);

} // ll_crypto_message_digest_224_init

/**
* Initialize message digest 256.
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
crypto_status_t	ll_crypto_message_digest_256_init(message_digest_ctx_t ctx, size_t* ctx_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->init(ctx, ctx_size);

} // ll_crypto_message_digest_256_init

/**
* Initialize message digest 384.
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
crypto_status_t	ll_crypto_message_digest_384_init(message_digest_ctx_t ctx, size_t* ctx_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->init(ctx, ctx_size);

} // ll_crypto_message_digest_384_init

/**
* Initialize message digest 512.
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
crypto_status_t	ll_crypto_message_digest_512_init(message_digest_ctx_t ctx, size_t* ctx_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->init(ctx, ctx_size);

} // ll_crypto_message_digest_512_init

/**
* Update message digest 224 with the given input data.
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
crypto_status_t	ll_crypto_message_digest_224_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->update(ctx, input, input_size);

} // ll_crypto_message_digest_224_update

/**
* Update message digest 256 with the given input data.
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
crypto_status_t	ll_crypto_message_digest_256_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->update(ctx, input, input_size);

} // ll_crypto_message_digest_256_update

/**
* Update message digest 384 with the given input data.
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
crypto_status_t	ll_crypto_message_digest_384_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->update(ctx, input, input_size);

} // ll_crypto_message_digest_384_update

/**
* Update message digest 512 with the given input data.
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
crypto_status_t	ll_crypto_message_digest_512_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->update(ctx, input, input_size);

} // ll_crypto_message_digest_512_update

/**
* Finalize message digest 224 and write the output hash.
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
crypto_status_t	ll_crypto_message_digest_224_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->finalize(ctx, output, output_size);

} // ll_crypto_message_digest_224_finalize

/**
* Finalize message digest 256 and write the output hash.
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
crypto_status_t	ll_crypto_message_digest_256_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->finalize(ctx, output, output_size);

} // ll_crypto_message_digest_256_finalize

/**
* Finalize message digest 384 and write the output hash.
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
crypto_status_t	ll_crypto_message_digest_384_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->finalize(ctx, output, output_size);

} // ll_crypto_message_digest_384_finalize

/**
* Finalize message digest 512 and write the output hash.
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
crypto_status_t	ll_crypto_message_digest_512_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->finalize(ctx, output, output_size);

} // ll_crypto_message_digest_512_finalize

/**
* Perform a self-test of message digest 224 with the provided scratchpad.
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
crypto_status_t	ll_crypto_message_digest_224_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_digest_224_selftest

/**
* Perform a self-test of message digest 256 with the provided scratchpad.
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
crypto_status_t	ll_crypto_message_digest_256_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_digest_256_selftest

/**
* Perform a self-test of message digest 384 with the provided scratchpad.
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
crypto_status_t	ll_crypto_message_digest_384_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_digest_384_selftest

/**
* Perform a self-test of message digest 512 with the provided scratchpad.
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
crypto_status_t	ll_crypto_message_digest_512_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_digest_vtable_t* message_digest_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (message_digest_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_digest_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_digest_512_selftest
