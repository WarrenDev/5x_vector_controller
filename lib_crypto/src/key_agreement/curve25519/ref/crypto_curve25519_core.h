
/**
* crypto_curve25519_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto key agreement Curve25519 core.
*
*/

#ifndef  UNIKEY_CRYPTO_CURVE25519_CORE_HEADER_GUARD
# define UNIKEY_CRYPTO_CURVE25519_CORE_HEADER_GUARD

#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

#if !defined (ALIGN16)
#define ALIGN16
#endif

	enum {
		CURVE25519_OK = 0,
		CURVE25519_ERROR = -1
	};

	////////////////////////////////////////////////////////
	// Generate a secret key given 32 bytes of random data
	int curve25519_create_secret(
		unsigned char* secretKey,
		const unsigned char* inputBytes);

	////////////////////////////////////////////////////////
	// Generate a public key from a secret key
	int curve25519_create_public(
		unsigned char* publicKey,
		const unsigned char* secretKey);

	////////////////////////////////////////////////////////
	// Generate a shared secret key from a peer's public key, my secret key, and a nonce value
	int curve25519_create_shared_secret(
		unsigned char* sharedSecret,
		const unsigned char* peerPublicKey,
		const unsigned char* mySecretKey);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_CURVE25519_CORE_HEADER_GUARD
