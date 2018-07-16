
/**
* crypto_curve25519_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto key agreement Curve25519 core implementation.
*
*/

/**
* version 20081011                                        
* Matthew Dempsky                                         
* Public domain.                                          
* Derived from public domain code by D. J. Bernstein.     
*/

#include "crypto_curve25519_core.h"
#include <string.h>
#include <stdint.h>

/**
* Internal constants
*/
static const uint8_t
basePoint[32] = { 9, 0, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0 };

static const uint32_t
minusp[32] = { 19, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 128 };

////////////////////////////////////////////////////////////
// Internals

/**
* Internal mainloop scratchpad variables
*/
static uint32_t xzm1[64];
static uint32_t xzm[64];
static uint32_t xzmb[64];
static uint32_t xzm1b[64];
static uint32_t xznb[64];
static uint32_t xzn1b[64];
static uint32_t a0[64];
static uint32_t a1[64];
static uint32_t b0[64];
static uint32_t b1[64];
static uint32_t c1[64];
static uint32_t r[32];
static uint32_t s[32];
static uint32_t t[32];
static uint32_t u[32];

/**
* Internal recip scratchpad variables
*/
static uint32_t z2[32];
static uint32_t z9[32];
static uint32_t z11[32];
static uint32_t z2_5_0[32];
static uint32_t z2_10_0[32];
static uint32_t z2_20_0[32];
static uint32_t z2_50_0[32];
static uint32_t z2_100_0[32];
static uint32_t t0[32];
static uint32_t t1[32];

/**
* Internal routines
*/
static void add				  (uint32_t out[32], const uint32_t a[32], const uint32_t b[32]);
static void sub				  (uint32_t out[32], const uint32_t a[32], const uint32_t b[32]);
static void squeeze			  (uint32_t a[32]);
static void freeze			  (uint32_t a[32]);
static void mult			  (uint32_t out[32], const uint32_t a[32], const uint32_t b[32]);
static void mult121665		  (uint32_t out[32], const uint32_t a[32]);
static void square			  (uint32_t out[32], const uint32_t a[32]);
static void select			  (uint32_t p[64], uint32_t q[64], const uint32_t r[64], const uint32_t s[64], uint32_t b);
static void mainloop		  (uint32_t work[64], const unsigned char e[32]);
static void recip			  (uint32_t out[32], const uint32_t z[32]);
static int 	crypto_scalarmult (unsigned char *q, const unsigned char *n, const unsigned char *p);

////////////////////////////////////////////////////////////
// add
void add(uint32_t out[32], const uint32_t a[32], const uint32_t b[32])
{

	uint32_t j;
	uint32_t u;
	u = 0;
	for (j = 0; j < 31; ++j) { u += a[j] + b[j]; out[j] = u & 255; u >>= 8; }
	u += a[31] + b[31]; out[31] = u;

} // add

////////////////////////////////////////////////////////////
// sub
void sub(uint32_t out[32], const uint32_t a[32], const uint32_t b[32])
{

	uint32_t j;
	uint32_t u;
	u = 218;
	for (j = 0; j < 31; ++j)
	{
		u += a[j] + 65280 - b[j];
		out[j] = u & 255;
		u >>= 8;
	}
	u += a[31] - b[31];
	out[31] = u;

} // sub

////////////////////////////////////////////////////////////
// squeeze
void squeeze(uint32_t a[32])
{

	uint32_t j;
	uint32_t u;
	u = 0;
	for (j = 0; j < 31; ++j) { u += a[j]; a[j] = u & 255; u >>= 8; }
	u += a[31]; a[31] = u & 127;
	u = 19 * (u >> 7);
	for (j = 0; j < 31; ++j) { u += a[j]; a[j] = u & 255; u >>= 8; }
	u += a[31]; a[31] = u;

} // squeeze

////////////////////////////////////////////////////////////
// freeze
void freeze(uint32_t a[32])
{

	uint32_t aorig[32];
	uint32_t j;
	uint32_t negative;

	for (j = 0; j < 32; ++j) aorig[j] = a[j];
	add(a, a, minusp);
	negative = -((a[31] >> 7) & 1);
	for (j = 0; j < 32; ++j) a[j] ^= negative & (aorig[j] ^ a[j]);

} // freeze

////////////////////////////////////////////////////////////
// mult
void mult(uint32_t out[32], const uint32_t a[32], const uint32_t b[32])
{

	uint32_t i;
	uint32_t j;
	uint32_t u;

	for (i = 0; i < 32; ++i)
	{
		u = 0;
		for (j = 0; j <= i; ++j) u += a[j] * b[i - j];
		for (j = i + 1; j < 32; ++j) u += 38 * a[j] * b[i + 32 - j];
		out[i] = u;
	}
	squeeze(out);

} // mult

////////////////////////////////////////////////////////////
// mult121665
void mult121665(uint32_t out[32], const uint32_t a[32])
{

	uint32_t j;
	uint32_t u;

	u = 0;
	for (j = 0; j < 31; ++j) { u += 121665 * a[j]; out[j] = u & 255; u >>= 8; }
	u += 121665 * a[31]; out[31] = u & 127;
	u = 19 * (u >> 7);
	for (j = 0; j < 31; ++j) { u += out[j]; out[j] = u & 255; u >>= 8; }
	u += out[j]; out[j] = u;

} // mult121665

////////////////////////////////////////////////////////////
// square
void square(uint32_t out[32], const uint32_t a[32])
{

	uint32_t i;
	uint32_t j;
	uint32_t u;

	for (i = 0; i < 32; ++i)
	{
		u = 0;
		for (j = 0; j < i - j; ++j) u += a[j] * a[i - j];
		for (j = i + 1; j < i + 32 - j; ++j) u += 38 * a[j] * a[i + 32 - j];
		u *= 2;
		if ((i & 1) == 0)
		{
			u += a[i / 2] * a[i / 2];
			u += 38 * a[i / 2 + 16] * a[i / 2 + 16];
		}
		out[i] = u;
	}
	squeeze(out);

} // square

////////////////////////////////////////////////////////////
// select
void select(uint32_t p[64], uint32_t q[64], const uint32_t r[64], const uint32_t s[64], uint32_t b)
{

	uint32_t j;
	uint32_t t;
	uint32_t bminus1;

	bminus1 = b - 1;
	for (j = 0; j < 64; ++j)
	{
		t = bminus1 & (r[j] ^ s[j]);
		p[j] = s[j] ^ t;
		q[j] = r[j] ^ t;
	}

} // select

////////////////////////////////////////////////////////////
// mainloop
void mainloop(uint32_t work[64], const unsigned char e[32])
{

	uint32_t j;
	uint32_t b;
	int pos;

	for (j = 0; j < 32; ++j) xzm1[j] = work[j];
	xzm1[32] = 1;
	for (j = 33; j < 64; ++j) xzm1[j] = 0;

	xzm[0] = 1;
	for (j = 1; j < 64; ++j) xzm[j] = 0;

	for (pos = 254; pos >= 0; --pos)
	{
		b = e[pos / 8] >> (pos & 7);
		b &= 1;
		select(xzmb, xzm1b, xzm, xzm1, b);
		add(a0, xzmb, xzmb + 32);
		sub(a0 + 32, xzmb, xzmb + 32);
		add(a1, xzm1b, xzm1b + 32);
		sub(a1 + 32, xzm1b, xzm1b + 32);
		square(b0, a0);
		square(b0 + 32, a0 + 32);
		mult(b1, a1, a0 + 32);
		mult(b1 + 32, a1 + 32, a0);
		add(c1, b1, b1 + 32);
		sub(c1 + 32, b1, b1 + 32);
		square(r, c1 + 32);
		sub(s, b0, b0 + 32);
		mult121665(t, s);
		add(u, t, b0);
		mult(xznb, b0, b0 + 32);
		mult(xznb + 32, s, u);
		square(xzn1b, c1);
		mult(xzn1b + 32, r, work);
		select(xzm, xzm1, xznb, xzn1b, b);
	}

	for (j = 0; j < 64; ++j) work[j] = xzm[j];

} // mainloop

////////////////////////////////////////////////////////////
// recip
void recip(uint32_t out[32], const uint32_t z[32])
{

	int i;

	/* 2 */ square(z2, z);
	/* 4 */ square(t1, z2);
	/* 8 */ square(t0, t1);
	/* 9 */ mult(z9, t0, z);
	/* 11 */ mult(z11, z9, z2);
	/* 22 */ square(t0, z11);
	/* 2^5 - 2^0 = 31 */ mult(z2_5_0, t0, z9);

	/* 2^6 - 2^1 */ square(t0, z2_5_0);
	/* 2^7 - 2^2 */ square(t1, t0);
	/* 2^8 - 2^3 */ square(t0, t1);
	/* 2^9 - 2^4 */ square(t1, t0);
	/* 2^10 - 2^5 */ square(t0, t1);
	/* 2^10 - 2^0 */ mult(z2_10_0, t0, z2_5_0);

	/* 2^11 - 2^1 */ square(t0, z2_10_0);
	/* 2^12 - 2^2 */ square(t1, t0);
	/* 2^20 - 2^10 */ for (i = 2; i < 10; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^20 - 2^0 */ mult(z2_20_0, t1, z2_10_0);

	/* 2^21 - 2^1 */ square(t0, z2_20_0);
	/* 2^22 - 2^2 */ square(t1, t0);
	/* 2^40 - 2^20 */ for (i = 2; i < 20; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^40 - 2^0 */ mult(t0, t1, z2_20_0);

	/* 2^41 - 2^1 */ square(t1, t0);
	/* 2^42 - 2^2 */ square(t0, t1);
	/* 2^50 - 2^10 */ for (i = 2; i < 10; i += 2) { square(t1, t0); square(t0, t1); }
	/* 2^50 - 2^0 */ mult(z2_50_0, t0, z2_10_0);

	/* 2^51 - 2^1 */ square(t0, z2_50_0);
	/* 2^52 - 2^2 */ square(t1, t0);
	/* 2^100 - 2^50 */ for (i = 2; i < 50; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^100 - 2^0 */ mult(z2_100_0, t1, z2_50_0);

	/* 2^101 - 2^1 */ square(t1, z2_100_0);
	/* 2^102 - 2^2 */ square(t0, t1);
	/* 2^200 - 2^100 */ for (i = 2; i < 100; i += 2) { square(t1, t0); square(t0, t1); }
	/* 2^200 - 2^0 */ mult(t1, t0, z2_100_0);

	/* 2^201 - 2^1 */ square(t0, t1);
	/* 2^202 - 2^2 */ square(t1, t0);
	/* 2^250 - 2^50 */ for (i = 2; i < 50; i += 2) { square(t0, t1); square(t1, t0); }
	/* 2^250 - 2^0 */ mult(t0, t1, z2_50_0);

	/* 2^251 - 2^1 */ square(t1, t0);
	/* 2^252 - 2^2 */ square(t0, t1);
	/* 2^253 - 2^3 */ square(t1, t0);
	/* 2^254 - 2^4 */ square(t0, t1);
	/* 2^255 - 2^5 */ square(t1, t0);
	/* 2^255 - 21 */ mult(out, t1, z11);

} // recip

////////////////////////////////////////////////////////////
// crypto_scalarmult
int crypto_scalarmult(unsigned char *q,
	const unsigned char *n,
	const unsigned char *p)
{

	uint32_t work[96];
	unsigned char e[32];
	uint32_t i;

	for (i = 0; i < 32; ++i) e[i] = n[i];
	e[0] &= 248;
	e[31] &= 127;
	e[31] |= 64;
	for (i = 0; i < 32; ++i) work[i] = p[i];
	mainloop(work, e);
	recip(work + 32, work + 32);
	mult(work + 64, work, work + 32);
	freeze(work + 64);
	for (i = 0; i < 32; ++i) q[i] = work[64 + i];

	return CURVE25519_OK;

} // scalarmult

////////////////////////////////////////////////////////////
// curve25519_create_secret
//   Generate a secret key given 32 bytes of random data
int curve25519_create_secret(
	unsigned char* secretKey,
	const unsigned char* inputBytes)
{

	memcpy(secretKey, inputBytes, 32);
	secretKey[0] &= 248;
	secretKey[31] &= 127;
	secretKey[31] |= 64;

	return CURVE25519_OK;

} // curve25519_create_secret

////////////////////////////////////////////////////////////
// curve25519_create_public
//   Generate a public key from a secret key
int curve25519_create_public(
	unsigned char* publicKey,
	const unsigned char* secretKey)
{

	return
		crypto_scalarmult(
		publicKey, secretKey, basePoint);

} // curve25519_create_public

////////////////////////////////////////////////////////////
// curve25519_create_shared_secret
//  Generate a shared secret key from a peer's public key, my secret key, and a nonce value
int curve25519_create_shared_secret(
	unsigned char* sharedSecret,
	const unsigned char* peerPublicKey,
	const unsigned char* mySecretKey)
{

	return
		crypto_scalarmult(
		sharedSecret, mySecretKey, peerPublicKey);

} // curve25519_create_shared_secret

