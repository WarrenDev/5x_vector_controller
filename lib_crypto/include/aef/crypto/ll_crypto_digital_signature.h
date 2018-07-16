
/**
* ll_crypto_digital_signature.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto digital signature API definition.
*/

#ifndef LL_CRYPTO_DIGITAL_SIGNATURE_H
#define LL_CRYPTO_DIGITAL_SIGNATURE_H

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/digital_signature.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* low-level crypto digital signature API routines
	*/
	crypto_status_t			ll_crypto_register_digital_signature_ed25519(void);
	digital_signature_id_t	ll_crypto_digital_signature_ed25519_get_id(void);
	size_t					ll_crypto_digital_signature_ed25519_get_digest_size(void);
	crypto_status_t			ll_crypto_digital_signature_ed25519_init(void);
	crypto_status_t			ll_crypto_digital_signature_ed25519_sign_keypair(uint8_t* pk, uint8_t* sk, uint8_t* random_bytes);
	crypto_status_t			ll_crypto_digital_signature_ed25519_sign(uint8_t* sm, size_t* sm_size, const uint8_t* m, size_t m_size, const uint8_t* sk, const uint8_t* pk);
	crypto_status_t			ll_crypto_digital_signature_ed25519_sign_open(uint8_t* m, size_t* m_size, const uint8_t* sm, size_t sm_size, const uint8_t* pk);
	crypto_status_t			ll_crypto_digital_signature_ed25519_selftest(uint8_t* scratch, size_t* scratch_size);

	crypto_status_t			ll_crypto_register_digital_signature_ed25519_donna(void);
	digital_signature_id_t	ll_crypto_digital_signature_ed25519_donna_get_id(void);
	size_t 					ll_crypto_digital_signature_ed25519_donna_get_digest_size(void);
	crypto_status_t 		ll_crypto_digital_signature_ed25519_donna_init(void);
	crypto_status_t 		ll_crypto_digital_signature_ed25519_donna_sign_keypair(uint8_t* pk, uint8_t* sk, uint8_t* random_bytes);
	crypto_status_t			ll_crypto_digital_signature_ed25519_donna_sign(uint8_t* sm, size_t* sm_size, const uint8_t* m, size_t m_size, const uint8_t* sk, const uint8_t* pk);
	crypto_status_t			ll_crypto_digital_signature_ed25519_donna_sign_open(uint8_t* m, size_t* m_size, const uint8_t* sm, size_t sm_size, const uint8_t* pk);
	crypto_status_t			ll_crypto_digital_signature_ed25519_donna_selftest(uint8_t* scratch, size_t* scratch_size);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_DIGITAL_SIGNATURE_H
