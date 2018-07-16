
/**
* ll_crypto_message_authentication.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto message authentication API definition.
*/

#ifndef LL_CRYPTO_MESSAGE_AUTHENTICATION_H
#define LL_CRYPTO_MESSAGE_AUTHENTICATION_H

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/message_authentication.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* low-level crypto message authentication API routines
	*/
	crypto_status_t				ll_crypto_register_message_authentication_256(void);
	crypto_status_t				ll_crypto_register_message_authentication_384(void);
	crypto_status_t				ll_crypto_register_message_authentication_512(void);

	message_authentication_id_t	ll_crypto_message_authentication_256_get_id(void);
	message_authentication_id_t	ll_crypto_message_authentication_384_get_id(void);
	message_authentication_id_t	ll_crypto_message_authentication_512_get_id(void);

	size_t						ll_crypto_message_authentication_256_block_size(void);
	size_t						ll_crypto_message_authentication_384_block_size(void);
	size_t						ll_crypto_message_authentication_512_block_size(void);

	size_t						ll_crypto_message_authentication_256_digest_size(void);
	size_t						ll_crypto_message_authentication_384_digest_size(void);
	size_t						ll_crypto_message_authentication_512_digest_size(void);

	crypto_status_t				ll_crypto_message_authentication_256_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size);
	crypto_status_t				ll_crypto_message_authentication_384_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size);
	crypto_status_t				ll_crypto_message_authentication_512_init(const uint8_t* key, size_t key_size, message_authentication_ctx_t ctx, size_t* ctx_size);

    crypto_status_t				ll_crypto_message_authentication_256_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size);
    crypto_status_t				ll_crypto_message_authentication_384_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size);
    crypto_status_t				ll_crypto_message_authentication_512_update(message_authentication_ctx_t ctx, const uint8_t* input, size_t input_size);

    crypto_status_t				ll_crypto_message_authentication_256_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size);
    crypto_status_t				ll_crypto_message_authentication_384_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size);
    crypto_status_t				ll_crypto_message_authentication_512_finalize(message_authentication_ctx_t ctx, const uint8_t* key, size_t key_size, uint8_t* output, size_t* output_size);

    crypto_status_t				ll_crypto_message_authentication_256_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t				ll_crypto_message_authentication_384_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);
    crypto_status_t				ll_crypto_message_authentication_512_selftest(message_authentication_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_MESSAGE_AUTHENTICATION_H
