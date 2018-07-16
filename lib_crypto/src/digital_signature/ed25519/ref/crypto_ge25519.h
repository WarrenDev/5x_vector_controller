
/**
* crypto_ge25519.h
*
* \copyright
* Copyright 2015 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief crypto digital signature ge25519 core.
*
*/

#ifndef CRYPTO_GE25519_H_
#define CRYPTO_GE25519_H_

#include "crypto_fe25519.h"
#include "crypto_sc25519.h"

#if !defined (ALIGN16)
#define ALIGN16
#endif

enum {
	GE25519_OK = 0,
	GE25519_ERROR = -1
};

#define ge25519                      		crypto_sign_ed25519_ref_ge25519
#define ge25519_base                 		crypto_sign_ed25519_ref_ge25519_base
#define ge25519_unpackneg_vartime    		crypto_sign_ed25519_ref_unpackneg_vartime
#define ge25519_pack               			crypto_sign_ed25519_ref_pack
#define ge25519_isneutral_vartime			crypto_sign_ed25519_ref_isneutral_vartime
#define ge25519_double_scalarmult_vartime	crypto_sign_ed25519_ref_double_scalarmult_vartime
#define ge25519_scalarmult_base				crypto_sign_ed25519_ref_scalarmult_base

typedef struct
{
	fe25519 x;
	fe25519 y;
	fe25519 z;
	fe25519 t;
} ge25519;

typedef struct
{
	fe25519 x;
	fe25519 z;
	fe25519 y;
	fe25519 t;
} ge25519_p1p1;

typedef struct
{
	fe25519 x;
	fe25519 y;
	fe25519 z;
} ge25519_p2;

typedef struct
{
	fe25519 x;
	fe25519 y;
} ge25519_aff;

#define ge25519_p3 ge25519


int 	ge25519_unpackneg_vartime (ge25519 *r, const unsigned char p[32]);
void 	ge25519_pack (unsigned char r[32], const ge25519 *p);
int 	ge25519_isneutral_vartime (const ge25519 *p);
void 	ge25519_double_scalarmult_vartime (ge25519 *r, const ge25519 *p1, const sc25519 *s1, const ge25519 *p2, const sc25519 *s2);
void 	ge25519_scalarmult_base (ge25519 *r, const sc25519 *s);


#endif /* CRYPTO_GE25519_H_ */
