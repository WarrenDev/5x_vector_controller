
/**
* ll_crypto_block_cipher.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto block cipher API implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_block_cipher.h>
#include "stddef.h"
#include "../src/block_cipher/aes/ref/crypto_aes_register.h"

/**
* Register the AES 128 ECB crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t
ll_crypto_register_block_cipher_aes_128_ecb(void)
{

	return aes_128_ecb_register();

} // ll_crypto_register_block_cipher_aes_128_ecb

/**
* Register the AES 192 ECB crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t
ll_crypto_register_block_cipher_aes_192_ecb(void)
{

	return aes_192_ecb_register();

} // ll_crypto_register_block_cipher_aes_192_ecb

/**
* Register the AES 256 ECB crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t
ll_crypto_register_block_cipher_aes_256_ecb(void)
{

	return aes_256_ecb_register();

} // ll_crypto_register_block_cipher_aes_256_ecb

/**
* Register the AES 128 CBC crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t
ll_crypto_register_block_cipher_aes_128_cbc(void)
{

	return aes_128_cbc_register();

} // ll_crypto_register_block_cipher_aes_128_cbc

/**
* Register the AES 192 CBC crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t
ll_crypto_register_block_cipher_aes_192_cbc(void)
{

	return aes_192_cbc_register();

} // ll_crypto_register_block_cipher_aes_192_cbc

/**
* Register the AES 256 CBC crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t
ll_crypto_register_block_cipher_aes_256_cbc(void)
{

	return aes_256_cbc_register();

} // ll_crypto_register_block_cipher_aes_256_cbc

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
block_cipher_id_t
ll_crypto_block_cipher_aes_128_ecb_get_id(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->id();

} // ll_crypto_block_cipher_aes_128_ecb_get_id

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
block_cipher_id_t
ll_crypto_block_cipher_aes_192_ecb_get_id(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->id();

} // ll_crypto_block_cipher_aes_192_ecb_get_id

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
block_cipher_id_t
ll_crypto_block_cipher_aes_256_ecb_get_id(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->id();

} // ll_crypto_block_cipher_aes_256_cbc_get_id

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
block_cipher_id_t
ll_crypto_block_cipher_aes_128_cbc_get_id(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->id();

} // ll_crypto_block_cipher_aes_128_cbc_get_id

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
block_cipher_id_t
ll_crypto_block_cipher_aes_192_cbc_get_id(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->id();

} // ll_crypto_block_cipher_aes_192_cbc_get_id

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
block_cipher_id_t
ll_crypto_block_cipher_aes_256_cbc_get_id(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->id();

} // ll_crypto_block_cipher_aes_256_cbc_get_id

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_128_ecb_block_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->block_size();

} // ll_crypto_block_cipher_aes_128_ecb_block_size

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_192_ecb_block_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->block_size();

} // ll_crypto_block_cipher_aes_192_ecb_block_size

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_256_ecb_block_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->block_size();

} // ll_crypto_block_cipher_aes_256_ecb_block_size

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_128_cbc_block_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->block_size();

} // ll_crypto_block_cipher_aes_128_cbc_block_size

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_192_cbc_block_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->block_size();

} // ll_crypto_block_cipher_aes_192_cbc_block_size


/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_256_cbc_block_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->block_size();

} // ll_crypto_block_cipher_aes_256_cbc_block_size

/**
* Get the key size of this cipher.
*
* \returns the key size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_128_ecb_key_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->key_size();

} // ll_crypto_block_cipher_aes_128_ecb_key_size

/**
* Get the key size of this cipher.
*
* \returns the block size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_192_ecb_key_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->key_size();

} // ll_crypto_block_cipher_aes_192_ecb_key_size

/**
* Get the key size of this cipher.
*
* \returns the key size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_256_ecb_key_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->key_size();

} // ll_crypto_block_cipher_aes_256_ecb_key_size

/**
* Get the key size of this cipher.
*
* \returns the key size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_128_cbc_key_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->key_size();

} // ll_crypto_block_cipher_aes_128_cbc_key_size

/**
* Get the key size of this cipher.
*
* \returns the key size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_192_cbc_key_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->key_size();

} // ll_crypto_block_cipher_aes_192_cbc_key_size

/**
* Get the key size of this cipher.
*
* \returns the key size of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_256_cbc_key_size(void)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->key_size();

} // ll_crypto_block_cipher_aes_256_cbc_key_size

/**
* Get the rounds of this cipher.
*
* \returns the rounds of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_128_ecb_rounds(block_cipher_ctx_t ctx)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->rounds(ctx);

} // ll_crypto_block_cipher_aes_128_ecb_rounds

/**
* Get the rounds of this cipher.
*
* \returns the rounds of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_192_ecb_rounds(block_cipher_ctx_t ctx)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->rounds(ctx);

} // ll_crypto_block_cipher_aes_192_ecb_rounds

/**
* Get the rounds of this cipher.
*
* \returns the rounds of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_256_ecb_rounds(block_cipher_ctx_t ctx)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->rounds(ctx);

} // ll_crypto_block_cipher_aes_256_ecb_rounds

/**
* Get the rounds of this cipher.
*
* \returns the rounds of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_128_cbc_rounds(block_cipher_ctx_t ctx)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->rounds(ctx);

} // ll_crypto_block_cipher_aes_128_cbc_rounds

/**
* Get the rounds of this cipher.
*
* \returns the rounds of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_192_cbc_rounds(block_cipher_ctx_t ctx)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->rounds(ctx);

} // ll_crypto_block_cipher_aes_192_cbc_rounds

/**
* Get the rounds of this cipher.
*
* \returns the rounds of this cipher.
*/
size_t
ll_crypto_block_cipher_aes_256_cbc_rounds(block_cipher_ctx_t ctx)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return 0;

	return aes_impl->rounds(ctx);

} // ll_crypto_block_cipher_aes_256_cbc_rounds

/**
* Initialize this block cipher.
*
* \param init_param        The initialization parameter.
* \param key               The key to use for this block cipher.
* \param key_size          The size of the key for this block cipher.
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this cipher.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this cipher.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this cipher.
*/
crypto_status_t	
ll_crypto_block_cipher_aes_128_ecb_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->init(init_param, key, key_size, ctx, ctx_size);

} // ll_cypto_block_cipher_aes_128_init

/**
* Initialize this block cipher.
*
* \param init_param        The initialization parameter.
* \param key               The key to use for this block cipher.
* \param key_size          The size of the key for this block cipher.
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this cipher.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this cipher.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this cipher.
*/
crypto_status_t	
ll_crypto_block_cipher_aes_192_ecb_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->init(init_param, key, key_size, ctx, ctx_size);

} // ll_crypto_block_cipher_aes_192_ecb_init

/**
* Initialize this block cipher.
*
* \param init_param        The initialization parameter.
* \param key               The key to use for this block cipher.
* \param key_size          The size of the key for this block cipher.
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this cipher.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this cipher.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this cipher.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_ecb_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->init(init_param, key, key_size, ctx, ctx_size);

} // ll_crypto_block_cipher_aes_256_ecb_init

/**
* Initialize this block cipher.
*
* \param init_param        The initialization parameter.
* \param key               The key to use for this block cipher.
* \param key_size          The size of the key for this block cipher.
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this cipher.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this cipher.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this cipher.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_cbc_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->init(init_param, key, key_size, ctx, ctx_size);

} // ll_crypto_block_cipher_aes_128_cbc_init

/**
* Initialize this block cipher.
*
* \param init_param        The initialization parameter.
* \param key               The key to use for this block cipher.
* \param key_size          The size of the key for this block cipher.
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this cipher.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this cipher.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this cipher.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_cbc_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->init(init_param, key, key_size, ctx, ctx_size);

} // ll_crypto_block_cipher_aes_192_cbc_init

/**
* Initialize this block cipher.
*
* \param init_param        The initialization parameter.
* \param key               The key to use for this block cipher.
* \param key_size          The size of the key for this block cipher.
* \param ctx               The context to initialize.
* \param ctx_size          The size of the context for this cipher.
*
* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
*
*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
*
*          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
*
*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*              not large enough for holding the context of this cipher.
*              If this status is returned, then ctx_size is updated with
*              the size of the context as required by this cipher.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_cbc_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->init (init_param, key, key_size, ctx, ctx_size );

} // ll_crypto_block_cipher_aes_256_cbc_init

/**
* Set the initialization vector for this cipher, if supported.
*
* \param ctx           The context.
* \param iv            The initialization vector for this cipher.
* \param iv_size       The size of the initialization vector.
*
* \returns CRYPTO_SUCCESS if the IV was successfully set.
*
*          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
*              is invalid.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              accept an initialization vector.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_ecb_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->set_iv ( ctx, iv, iv_size );

} // ll_crypto_block_cipher_aes_128_ecb_set_iv

/**
* Set the initialization vector for this cipher, if supported.
*
* \param ctx           The context.
* \param iv            The initialization vector for this cipher.
* \param iv_size       The size of the initialization vector.
*
* \returns CRYPTO_SUCCESS if the IV was successfully set.
*
*          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
*              is invalid.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              accept an initialization vector.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_ecb_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->set_iv(ctx, iv, iv_size);

} // ll_crypto_block_cipher_aes_192_ecb_set_iv

/**
* Set the initialization vector for this cipher, if supported.
*
* \param ctx           The context.
* \param iv            The initialization vector for this cipher.
* \param iv_size       The size of the initialization vector.
*
* \returns CRYPTO_SUCCESS if the IV was successfully set.
*
*          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
*              is invalid.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              accept an initialization vector.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_ecb_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->set_iv(ctx, iv, iv_size);

} // ll_crypto_block_cipher_aes_256_ecb_set_iv

/**
* Set the initialization vector for this cipher, if supported.
*
* \param ctx           The context.
* \param iv            The initialization vector for this cipher.
* \param iv_size       The size of the initialization vector.
*
* \returns CRYPTO_SUCCESS if the IV was successfully set.
*
*          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
*              is invalid.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              accept an initialization vector.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_cbc_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->set_iv(ctx, iv, iv_size);

} // ll_crypto_block_cipher_aes_128_cbc_set_iv

/**
* Set the initialization vector for this cipher, if supported.
*
* \param ctx           The context.
* \param iv            The initialization vector for this cipher.
* \param iv_size       The size of the initialization vector.
*
* \returns CRYPTO_SUCCESS if the IV was successfully set.
*
*          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
*              is invalid.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              accept an initialization vector.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_cbc_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->set_iv(ctx, iv, iv_size);

} // ll_crypto_block_cipher_aes_192_cbc_set_iv


/**
* Set the initialization vector for this cipher, if supported.
*
* \param ctx           The context.
* \param iv            The initialization vector for this cipher.
* \param iv_size       The size of the initialization vector.
*
* \returns CRYPTO_SUCCESS if the IV was successfully set.
*
*          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
*              is invalid.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              accept an initialization vector.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_cbc_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->set_iv(ctx, iv, iv_size);

} // ll_crypto_block_cipher_aes_256_cbc_set_iv

/**
* Encrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
* \param output        The output buffer to store the encrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support encryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_ecb_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->encrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_128_ecb_encrypt

/**
* Encrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
* \param output        The output buffer to store the encrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support encryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_ecb_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->encrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_192_ecb_encrypt

/**
* Encrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
* \param output        The output buffer to store the encrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support encryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_ecb_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->encrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_256_ecb_encrypt

/**
* Encrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
* \param output        The output buffer to store the encrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support encryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_cbc_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->encrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_128_cbc_encrypt

/**
* Encrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
* \param output        The output buffer to store the encrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support encryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_cbc_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->encrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_192_cbc_encrypt

/**
* Encrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to encrypt.
* \param input_size    The size of the input data to encrypt.
* \param output        The output buffer to store the encrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support encryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_cbc_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->encrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_256_cbc_encrypt

/**
* Decrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to decrypt.
* \param input_size    The size of the input data to decrypt.
* \param output        The output buffer to store the decrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support decryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_ecb_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->decrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_128_ecb_decrypt

/**
* Decrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to decrypt.
* \param input_size    The size of the input data to decrypt.
* \param output        The output buffer to store the decrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support decryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_ecb_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->decrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_192_decrypt

/**
* Decrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to decrypt.
* \param input_size    The size of the input data to decrypt.
* \param output        The output buffer to store the decrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support decryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_ecb_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->decrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_256_ecb_encrypt

/**
* Decrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to decrypt.
* \param input_size    The size of the input data to decrypt.
* \param output        The output buffer to store the decrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support decryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_128_cbc_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->decrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_128_cbc_decrypt

/**
* Decrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to decrypt.
* \param input_size    The size of the input data to decrypt.
* \param output        The output buffer to store the decrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support decryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_192_cbc_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->decrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_192_cbc_decrypt

/**
* Decrypt data using this cipher.
*
* The input and output buffers must be the same size, denoted by
* input_size, and the size must be a multiple of the block size.
*
* Internally, if this cipher supports an IV, the IV is updated after this
* call is completed.
*
* \param ctx           The context.
* \param input         The input data to decrypt.
* \param input_size    The size of the input data to decrypt.
* \param output        The output buffer to store the decrypted data.
*
* \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
*
*          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
*              a multiple of the block size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
*              support decryption.
*/
crypto_status_t
ll_crypto_block_cipher_aes_256_cbc_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->decrypt(ctx, input, input_size, output);

} // ll_crypto_block_cipher_aes_256_cbc_decrypt

/**
* Perform a self-test of this block cipher with the provided scratchpad.
*
* \param ctx           The context.
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
ll_crypto_block_cipher_aes_128_ecb_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_block_cipher_aes_128_ecb_selftest

/**
* Perform a self-test of this block cipher with the provided scratchpad.
*
* \param ctx           The context.
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
ll_crypto_block_cipher_aes_192_ecb_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_block_cipher_aes_192_ecb_selftest

/**
* Perform a self-test of this block cipher with the provided scratchpad.
*
* \param ctx           The context.
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
ll_crypto_block_cipher_aes_256_ecb_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_block_cipher_aes_256_ecb_selftest

/**
* Perform a self-test of this block cipher with the provided scratchpad.
*
* \param ctx           The context.
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
ll_crypto_block_cipher_aes_128_cbc_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_block_cipher_aes_128_cbc_selftest

/**
* Perform a self-test of this block cipher with the provided scratchpad.
*
* \param ctx           The context.
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
ll_crypto_block_cipher_aes_192_cbc_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_block_cipher_aes_192_cbc_selftest

/**
* Perform a self-test of this block cipher with the provided scratchpad.
*
* \param ctx           The context.
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
ll_crypto_block_cipher_aes_256_cbc_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	block_cipher_vtable_t* aes_impl = (block_cipher_vtable_t*)crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);
	if (aes_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return aes_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_block_cipher_aes_256_cbc_selftest

