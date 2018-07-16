
/**
* ll_crypto_block_cipher.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto block cipher API definition.
*/

#ifndef LL_CRYPTO_BLOCK_CIPHER_H
#define LL_CRYPTO_BLOCK_CIPHER_H

#include <aef/crypto/block_cipher.h>
#include <aef/crypto/crypto_status.h>

#include "../src/block_cipher/aes/ref/crypto_aes_core.h"

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* low-level crypto block cipher API routines
	*/
	crypto_status_t			ll_crypto_register_block_cipher_aes_128_ecb(void);
	crypto_status_t			ll_crypto_register_block_cipher_aes_192_ecb(void);
	crypto_status_t			ll_crypto_register_block_cipher_aes_256_ecb(void);
	crypto_status_t			ll_crypto_register_block_cipher_aes_128_cbc(void);
	crypto_status_t			ll_crypto_register_block_cipher_aes_192_cbc(void);
	crypto_status_t			ll_crypto_register_block_cipher_aes_256_cbc(void);

	block_cipher_id_t		ll_crypto_block_cipher_aes_128_ecb_get_id(void);
	block_cipher_id_t		ll_crypto_block_cipher_aes_192_ecb_get_id(void);
	block_cipher_id_t		ll_crypto_block_cipher_aes_256_ecb_get_id(void);
	block_cipher_id_t		ll_crypto_block_cipher_aes_128_cbc_get_id(void);
	block_cipher_id_t		ll_crypto_block_cipher_aes_192_cbc_get_id(void);
	block_cipher_id_t		ll_crypto_block_cipher_aes_256_cbc_get_id(void);

	size_t					ll_crypto_block_cipher_aes_128_ecb_block_size(void);
	size_t					ll_crypto_block_cipher_aes_192_ecb_block_size(void);
	size_t					ll_crypto_block_cipher_aes_256_ecb_block_size(void);
	size_t					ll_crypto_block_cipher_aes_128_cbc_block_size(void);
	size_t					ll_crypto_block_cipher_aes_192_cbc_block_size(void);
	size_t					ll_crypto_block_cipher_aes_256_cbc_block_size(void);

	size_t					ll_crypto_block_cipher_aes_128_ecb_key_size(void);
	size_t					ll_crypto_block_cipher_aes_192_ecb_key_size(void);
	size_t					ll_crypto_block_cipher_aes_256_ecb_key_size(void);
	size_t					ll_crypto_block_cipher_aes_128_cbc_key_size(void);
	size_t					ll_crypto_block_cipher_aes_192_cbc_key_size(void);
	size_t					ll_crypto_block_cipher_aes_256_cbc_key_size(void);

    size_t					ll_crypto_block_cipher_aes_128_ecb_rounds(block_cipher_ctx_t ctx);
    size_t					ll_crypto_block_cipher_aes_192_ecb_rounds(block_cipher_ctx_t ctx);
    size_t					ll_crypto_block_cipher_aes_256_ecb_rounds(block_cipher_ctx_t ctx);
    size_t					ll_crypto_block_cipher_aes_128_cbc_rounds(block_cipher_ctx_t ctx);
    size_t					ll_crypto_block_cipher_aes_192_cbc_rounds(block_cipher_ctx_t ctx);
    size_t					ll_crypto_block_cipher_aes_256_cbc_rounds(block_cipher_ctx_t ctx);

	crypto_status_t			ll_crypto_block_cipher_aes_128_ecb_init
							(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size);
	crypto_status_t			ll_crypto_block_cipher_aes_192_ecb_init
							(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size);
	crypto_status_t			ll_crypto_block_cipher_aes_256_ecb_init
							(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size);
	crypto_status_t			ll_crypto_block_cipher_aes_128_cbc_init
							(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size);
	crypto_status_t			ll_crypto_block_cipher_aes_192_cbc_init
							(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size);
	crypto_status_t			ll_crypto_block_cipher_aes_256_cbc_init
							(block_cipher_init_t init_param, const uint8_t* key, size_t key_size, block_cipher_ctx_t ctx, size_t* ctx_size);

    crypto_status_t			ll_crypto_block_cipher_aes_128_ecb_set_iv (block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);
    crypto_status_t			ll_crypto_block_cipher_aes_192_ecb_set_iv (block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);
    crypto_status_t			ll_crypto_block_cipher_aes_256_ecb_set_iv (block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);
    crypto_status_t			ll_crypto_block_cipher_aes_128_cbc_set_iv (block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);
    crypto_status_t			ll_crypto_block_cipher_aes_192_cbc_set_iv (block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);
    crypto_status_t			ll_crypto_block_cipher_aes_256_cbc_set_iv (block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);

    crypto_status_t			ll_crypto_block_cipher_aes_128_ecb_encrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_192_ecb_encrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_256_ecb_encrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_128_cbc_encrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_192_cbc_encrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_256_cbc_encrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);

    crypto_status_t			ll_crypto_block_cipher_aes_128_ecb_decrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_192_ecb_decrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_256_ecb_decrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_128_cbc_decrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_192_cbc_decrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);
    crypto_status_t			ll_crypto_block_cipher_aes_256_cbc_decrypt (block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);

    crypto_status_t			ll_crypto_block_cipher_aes_128_ecb_selftest (block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t			ll_crypto_block_cipher_aes_192_ecb_selftest (block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t			ll_crypto_block_cipher_aes_256_ecb_selftest (block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t			ll_crypto_block_cipher_aes_128_cbc_selftest (block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t			ll_crypto_block_cipher_aes_192_cbc_selftest (block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t			ll_crypto_block_cipher_aes_256_cbc_selftest (block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_BLOCK_CIPHER_H
