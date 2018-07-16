
/**
* crypto_prng_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto PRNG core definitions.
*/

#ifndef _CRYPTO_PRNG_CORE_H
#define _CRYPTO_PRNG_CORE_H

#include <aef/crypto/crypto_status.h>
#include <stddef.h>
#include <stdint.h>
#include "../src/block_cipher/aes/ref/crypto_aes_core.h"
#include "../src/block_cipher/aes/ref/crypto_aes_register.h"
#include "../src/message_digest/sha2/ref/crypto_sha2_core.h"
#include "../src/message_digest/sha2/ref/crypto_sha2_register.h"

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#define PRNG_COUNTER_BITS		128		// Counter size in bits (must be >= 8 and even)
#define BLOCK_SIZE				32		// Key and hash result size
#define CIPHER_BLOCK_SIZE		16		// Cipher block size

/*** PRNG Context Structure *******************************/
typedef struct _PRNG_CTX 
{
	aes_ctx		aes_context;
    size_t      aes_context_size;
	SHA2_CTX	sha2_context;
    size_t      sha2_context_size;
	uint8_t		counter[PRNG_COUNTER_BITS / 8];
	uint8_t		result[CIPHER_BLOCK_SIZE];
	uint8_t		key[BLOCK_SIZE];
	uint32_t	reseed_count;
	uint32_t	last_reseed_time;
} PRNG_CTX;

void			increment_counter		(uint8_t counter[]);
uint8_t			is_counter_zero			(uint8_t counter[]);
crypto_status_t twoLeastSignificantBytes(uint8_t result[], long value);
int				prng_ceil				(int value, int divisor);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif
