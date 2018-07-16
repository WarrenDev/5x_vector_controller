
/**
* crypto_ed25519_core.c
*
* \copyright
* Copyright 2015 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief crypto digital signature Ed25519 core implementation.
*
*/

#define WINDOWSIZE 1 /* Should be 1,2, or 4 */
#define WINDOWMASK ((1<<WINDOWSIZE)-1)

#include "crypto_ed25519_core.h"
#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/message_digest.h>
#include <string.h>
#include <stdint.h>

extern const ge25519 ge25519_base;

/**
* Context for SHA2 crypto operations
*/
static SHA2_CTX sha2_context;
static size_t   sha2_context_size = sizeof(SHA2_CTX);

/**
* crypto_verify_32
*/
static
int crypto_verify_32(const unsigned char *x, const unsigned char *y)
{
	unsigned int differentbits = 0;
#define F(i) differentbits |= x[i] ^ y[i];
	F(0)
	F(1)
	F(2)
	F(3)
	F(4)
	F(5)
	F(6)
	F(7)
	F(8)
	F(9)
	F(10)
	F(11)
	F(12)
	F(13)
	F(14)
	F(15)
	F(16)
	F(17)
	F(18)
	F(19)
	F(20)
	F(21)
	F(22)
	F(23)
	F(24)
	F(25)
	F(26)
	F(27)
	F(28)
	F(29)
	F(30)
	F(31)
	return (1 & ((differentbits - 1) >> 8)) - 1;
}

/**
* get_hram
*/
static
void get_hram(
	unsigned char *hram,
	const unsigned char *sm,
	const unsigned char *pk,
	unsigned char *playground,
	size_t smlen
	)
{
	size_t i;

	for (i = 0; i < 32; ++i)    playground[i] = sm[i];
	for (i = 32; i < 64; ++i)    playground[i] = pk[i - 32];
	for (i = 64; i < smlen; ++i) playground[i] = sm[i];

	/**
	* Retrieve SHA512 vtable from the crypto registry
	*/
	message_digest_vtable_t* sha512_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (sha512_impl == NULL)
		return;

	size_t sha512_digest_size = SHA512_DIGEST_LENGTH;

	sha512_impl->init     (&sha2_context, &sha2_context_size);
	sha512_impl->update   (&sha2_context, playground, smlen);
	sha512_impl->finalize (&sha2_context, hram, &sha512_digest_size);

}

/**
* crypto_sign_keypair
*/
int crypto_sign_keypair(
	uint8_t *pk,
	uint8_t *sk,
	uint8_t *randombytes
	)
{
	sc25519 scsk;
	ge25519 gepk;
	unsigned char extsk[64];
	int i;

	memcpy(sk, randombytes, 32);

	/**
	* Retrieve SHA512 vtable from the crypto registry
	*/
	message_digest_vtable_t* sha512_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (sha512_impl == NULL)
		return ED25519_ERROR;

	size_t sha512_digest_size = SHA512_DIGEST_LENGTH;

	sha512_impl->init     (&sha2_context, &sha2_context_size);
	sha512_impl->update   (&sha2_context, sk, 32);
	sha512_impl->finalize (&sha2_context, extsk, &sha512_digest_size);

	extsk[0] &= 248;
	extsk[31] &= 127;
	extsk[31] |= 64;

	sc25519_from32bytes(&scsk, extsk);

	ge25519_scalarmult_base(&gepk, &scsk);
	ge25519_pack(pk, &gepk);
	for (i = 0; i<32; i++)
		sk[32 + i] = pk[i];

	return ED25519_OK;
}

/**
* crypto_sign
*/
int crypto_sign(
	uint8_t *sm, size_t *smlen,
	const uint8_t *m, size_t mlen,
	const uint8_t *sk
	)
{
	sc25519 sck, scs, scsk;
	ge25519 ger;
	unsigned char r[32];
	unsigned char s[32];
	unsigned char extsk[64];
	unsigned char hmg[64];
	unsigned char hram[64];
	size_t i;

	/**
	* Retrieve SHA512 vtable from the crypto registry
	*/
	message_digest_vtable_t* sha512_impl = (message_digest_vtable_t*)crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);
	if (sha512_impl == NULL)
		return ED25519_ERROR;

	size_t sha512_digest_size = SHA512_DIGEST_LENGTH;

	sha512_impl->init(&sha2_context, &sha2_context_size);
	sha512_impl->update(&sha2_context, sk, 32);
	sha512_impl->finalize(&sha2_context, extsk, &sha512_digest_size);

	extsk[0] &= 248;
	extsk[31] &= 127;
	extsk[31] |= 64;

	*smlen = mlen + 64;
	for (i = 0; i<mlen; i++)
		sm[64 + i] = m[i];
	for (i = 0; i<32; i++)
		sm[32 + i] = extsk[32 + i];

	/* Generate k as h(extsk[32],...,extsk[63],m) */
	sha512_impl->init(&sha2_context, &sha2_context_size);
	sha512_impl->update(&sha2_context, sm + 32, mlen + 32);
	sha512_impl->finalize(&sha2_context, hmg, &sha512_digest_size);

	/* Computation of R */
	sc25519_from64bytes(&sck, hmg);
	ge25519_scalarmult_base(&ger, &sck);
	ge25519_pack(r, &ger);

	/* Computation of s */
	for (i = 0; i<32; i++)
		sm[i] = r[i];

	get_hram(hram, sm, sk + 32, sm, mlen + 64);

	sc25519_from64bytes(&scs, hram);
	sc25519_from32bytes(&scsk, extsk);
	sc25519_mul(&scs, &scs, &scsk);

	sc25519_add(&scs, &scs, &sck);

	sc25519_to32bytes(s, &scs); /* cat s */
	for (i = 0; i<32; i++)
		sm[32 + i] = s[i];

	return ED25519_OK;

}

/**
* crypto_sign_open
*/
int crypto_sign_open(
	uint8_t *m, size_t *mlen,
	const uint8_t *sm, size_t smlen,
	const uint8_t *pk
	)
{
	int ret;
	unsigned int i;
	unsigned char t2[32];
	ge25519 get1, get2;
	sc25519 schram, scs;
	unsigned char hram[64];

	*mlen = (size_t) - 1;
	if (smlen < 64) return -1;

	if (ge25519_unpackneg_vartime(&get1, pk)) return -1;

	get_hram(hram, sm, pk, m, smlen);

	sc25519_from64bytes(&schram, hram);

	sc25519_from32bytes(&scs, sm + 32);

	ge25519_double_scalarmult_vartime(&get2, &get1, &schram, &ge25519_base, &scs);
	ge25519_pack(t2, &get2);

	ret = crypto_verify_32(sm, t2);

	if (!ret)
	{
		for (i = 0; i<smlen - 64; i++)
			m[i] = sm[i + 64];
		*mlen = smlen - 64;
	}
	else
	{
		for (i = 0; i<smlen - 64; i++)
			m[i] = 0;
	}

	return ret;

}

