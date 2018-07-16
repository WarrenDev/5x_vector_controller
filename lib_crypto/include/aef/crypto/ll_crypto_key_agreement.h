
/**
* ll_crypto_key_agreement.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto key agreement API definition.
*/

#ifndef LL_CRYPTO_KEY_AGREEMENT_H
#define LL_CRYPTO_KEY_AGREEMENT_H

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/key_agreement.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* low-level crypto key_agreement API routines
	*/
	crypto_status_t		ll_crypto_register_key_agreement_curve25519 (void);
	key_agreement_id_t	ll_crypto_key_agreement_curve25519_get_id(void);
	crypto_status_t		ll_crypto_key_agreement_curve25519_create_secret(uint8_t* secret_key, const uint8_t* input_bytes);
	crypto_status_t		ll_crypto_key_agreement_curve25519_create_public(uint8_t* public_key, const uint8_t* secret_key);
	crypto_status_t		ll_crypto_key_agreement_curve25519_create_shared_secret(uint8_t* shared_secret, const uint8_t* peer_public_key, const uint8_t* my_secret_key);
	crypto_status_t		ll_crypto_key_agreement_curve25519_selftest(uint8_t* scratch, size_t* scratch_size);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_KEY_AGREEMENT_H
