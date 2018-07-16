
/**
* ll_crypto_message_authentication.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto message authentication API implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_message_authentication.h>
#include "stddef.h"
#include "../src/message_authentication/ref/crypto_message_authentication_register.h"

/**
* Register the crypto message_authentication 256 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_message_authentication_256(void)
{

	return message_authentication_256_register();

} // ll_crypto_register_message_authentication_256

/**
* Register the crypto message authentication 384 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_message_authentication_384(void)
{

	return message_authentication_384_register();

} // ll_crypto_register_message_authentication_384

/**
* Register the crypto message authentication 512 sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_message_authentication_512(void)
{

	return message_authentication_512_register();

} // ll_crypto_register_message_authentication_512


/**
* Get the ID of message authentication 256.
*
* \returns the message authentication ID, which can be used to get the string name
*          of this message authentication.
*/
message_authentication_id_t	ll_crypto_message_authentication_256_get_id(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->id();

} // ll_crypto_message_authentication_256_get_id

/**
* Get the ID of message authentication 384.
*
* \returns the message authentication ID, which can be used to get the string name
*          of this message authentication.
*/
message_authentication_id_t	ll_crypto_message_authentication_384_get_id(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->id();

} // ll_crypto_message_authentication_384_get_id

/**
* Get the ID of message authentication 512.
*
* \returns the message authentication ID, which can be used to get the string name
*          of this message authentication.
*/
message_authentication_id_t	ll_crypto_message_authentication_512_get_id(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->id();

} // ll_crypto_message_authentication_512_get_id

/**
* Get the input block size of message authentication 256.
*
* \returns the input block size of this message authentication.
*/
size_t	ll_crypto_message_authentication_256_block_size(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->block_size();

} // ll_crypto_message_authentication_256_block_size

/**
* Get the input block size of message authentication 384.
*
* \returns the input block size of this message authentication.
*/
size_t	ll_crypto_message_authentication_384_block_size(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->block_size();

} // ll_crypto_message_authentication_384_block_size

/**
* Get the input block size of message authentication 512.
*
* \returns the input block size of this message authentication.
*/
size_t	ll_crypto_message_authentication_512_block_size(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->block_size();

} // ll_crypto_message_authentication_512_block_size

/**
* Get the digest size of message authentication 256.
*
* \returns the digest size of this message authentication.
*/
size_t	ll_crypto_message_authentication_256_digest_size(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->digest_size();

} // ll_crypto_message_authentication_256_digest_size

/**
* Get the digest size of message authentication 384.
*
* \returns the digest size of this message authentication.
*/
size_t	ll_crypto_message_authentication_384_digest_size(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->digest_size();

} // ll_crypto_message_authentication_384_digest_size

/**
* Get the digest size of message authentication 512.
*
* \returns the digest size of this message authentication.
*/
size_t	ll_crypto_message_authentication_512_digest_size(void)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return 0;

	return message_auth_impl->digest_size();

} // ll_crypto_message_authentication_512_digest_size


/**
* Initialize message authentication 256.
*
* \param key           The key to use for this message authentication.
* \param key_size      The size of the key for this message authentication.
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
crypto_status_t
ll_crypto_message_authentication_256_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->init(key, key_size, ctx, ctx_size);

} // ll_crypto_message_authentication_256_init

/**
* Initialize message authentication 384.
*
* \param key           The key to use for this message authentication.
* \param key_size      The size of the key for this message authentication.
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
crypto_status_t
ll_crypto_message_authentication_384_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->init(key, key_size, ctx, ctx_size);

} // ll_crypto_message_authentication_384_init

/**
* Initialize message authentication 512.
*
* \param key           The key to use for this message authentication.
* \param key_size      The size of the key for this message authentication.
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
crypto_status_t
ll_crypto_message_authentication_512_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->init(key, key_size, ctx, ctx_size);

} // ll_crypto_message_authentication_512_init

/**
* Update message authentication 256 with the given input data.
*
* The internal state of this message authentication digest is updated with the given
* input data.
*
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_message_authentication_256_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->update(ctx, input, input_size);

} // ll_crypto_message_authentication_256_update

/**
* Update message authentication 384 with the given input data.
*
* The internal state of this message authentication digest is updated with the given
* input data.
*
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_message_authentication_384_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->update(ctx, input, input_size);

} // ll_crypto_message_authentication_384_update

/**
* Update message authentication 512 with the given input data.
*
* The internal state of this message authentication digest is updated with the given
* input data.
*
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
*
* \returns CRYPTO_SUCCESS if this operation was successful.
*
*          CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t
ll_crypto_message_authentication_512_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->update(ctx, input, input_size);

} // ll_crypto_message_authentication_512_update

/**
* Finalize message authentication 256 and write the output hash.
*
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
crypto_status_t
ll_crypto_message_authentication_256_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->finalize(ctx, key, key_size, output, output_size);

} // ll_crypto_message_authentication_256_finalize

/**
* Finalize message authentication 384 and write the output hash.
*
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
crypto_status_t
ll_crypto_message_authentication_384_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->finalize(ctx, key, key_size, output, output_size);

} // ll_crypto_message_authentication_384_finalize

/**
* Finalize message authentication 512 and write the output hash.
*
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
crypto_status_t
ll_crypto_message_authentication_512_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->finalize(ctx, key, key_size, output, output_size);

} // ll_crypto_message_authentication_512_finalize

/**
* Perform a self-test of message authentication 256 with the provided scratchpad.
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
ll_crypto_message_authentication_256_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_authentication_256_selftest

/**
* Perform a self-test of message authentication 384 with the provided scratchpad.
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
ll_crypto_message_authentication_384_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_authentication_384_selftest

/**
* Perform a self-test of message authentication 512 with the provided scratchpad.
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
ll_crypto_message_authentication_512_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	message_authentication_vtable_t* message_auth_impl = (message_authentication_vtable_t*)crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);
	if (message_auth_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return message_auth_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_message_authentication_512_selftest
