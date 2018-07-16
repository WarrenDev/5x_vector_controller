
/**
* crypto_ed25519_core.h
*
* \copyright
* Copyright 2015 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief crypto digital signature Ed25519 core definitions.
*
*/

#ifndef CRYPTO_ED25519_CORE_H_
#define CRYPTO_ED25519_CORE_H_

#include <stdint.h>
#include "../src/message_digest/sha2/ref/crypto_sha2_core.h"
#include "crypto_ge25519.h"

#if !defined (ALIGN16)
#define ALIGN16
#endif

enum {
	ED25519_OK = 0,
	ED25519_ERROR = -1
};

	int crypto_sign_keypair(
		uint8_t *pk,
		uint8_t *sk,
		uint8_t *randombytes
		);

	int crypto_sign(
		uint8_t *sm, size_t *smlen,
		const uint8_t *m, size_t mlen,
		const uint8_t *sk
		);

	int crypto_sign_open(
		uint8_t *m, size_t *mlen,
		const uint8_t *sm, size_t smlen,
		const uint8_t *pk
		);


#endif /* CRYPTO_ED25519_CORE_H_ */
