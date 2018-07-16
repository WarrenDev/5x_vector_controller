
/**
* ll_crypto_message_digest.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto message digest API definition.
*/

#ifndef LL_CRYPTO_MESSAGE_DIGEST_H
#define LL_CRYPTO_MESSAGE_DIGEST_H

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/message_digest.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* low-level crypto message digest API routines
	*/
	crypto_status_t		ll_crypto_register_sha224(void);
	crypto_status_t		ll_crypto_register_sha256(void);
	crypto_status_t		ll_crypto_register_sha384(void);
	crypto_status_t		ll_crypto_register_sha512(void);

	message_digest_id_t	ll_crypto_message_digest_224_get_id(void);
	message_digest_id_t	ll_crypto_message_digest_256_get_id(void);
	message_digest_id_t	ll_crypto_message_digest_384_get_id(void);
	message_digest_id_t	ll_crypto_message_digest_512_get_id(void);

	size_t				ll_crypto_message_digest_224_block_size(void);
	size_t				ll_crypto_message_digest_256_block_size(void);
	size_t				ll_crypto_message_digest_384_block_size(void);
	size_t				ll_crypto_message_digest_512_block_size(void);

	size_t				ll_crypto_message_digest_224_digest_size(void);
	size_t				ll_crypto_message_digest_256_digest_size(void);
	size_t				ll_crypto_message_digest_384_digest_size(void);
	size_t				ll_crypto_message_digest_512_digest_size(void);

	crypto_status_t		ll_crypto_message_digest_224_init(message_digest_ctx_t ctx, size_t* ctx_size);
	crypto_status_t		ll_crypto_message_digest_256_init(message_digest_ctx_t ctx, size_t* ctx_size);
	crypto_status_t		ll_crypto_message_digest_384_init(message_digest_ctx_t ctx, size_t* ctx_size);
	crypto_status_t		ll_crypto_message_digest_512_init(message_digest_ctx_t ctx, size_t* ctx_size);

    crypto_status_t		ll_crypto_message_digest_224_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size);
    crypto_status_t		ll_crypto_message_digest_256_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size);
    crypto_status_t		ll_crypto_message_digest_384_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size);
    crypto_status_t		ll_crypto_message_digest_512_update(message_digest_ctx_t ctx, const uint8_t* input, size_t input_size);

    crypto_status_t		ll_crypto_message_digest_224_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size);
    crypto_status_t		ll_crypto_message_digest_256_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size);
    crypto_status_t		ll_crypto_message_digest_384_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size);
    crypto_status_t		ll_crypto_message_digest_512_finalize(message_digest_ctx_t ctx, uint8_t* output, size_t* output_size);

    crypto_status_t		ll_crypto_message_digest_224_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t		ll_crypto_message_digest_256_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t		ll_crypto_message_digest_384_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t		ll_crypto_message_digest_512_selftest(message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_MESSAGE_DIGEST_H
