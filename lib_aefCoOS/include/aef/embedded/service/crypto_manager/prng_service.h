
/**
* prng_service.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the crypto manager service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_CRYPTO_MANAGER_PRNG_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_CRYPTO_MANAGER_PRNG_SERVICE_H_

#include <aef/crypto/prng.h>

/**
* PRNG initialization parameter structure definition
*/
typedef struct prng_init_parms_def
{
	uint8_t*	entropy;
	size_t 		entropySize;
	uint8_t* 	personal;
	size_t 		personalSize;
	prng_ctx_t* ctx;
	size_t* 	ctx_size;
} prng_init_parms_t;

/**
* PRNG ID parameter structure definition
*/
typedef struct prng_id_parms_def
{
    prng_ctx_t	ctx;
    uint32_t    id;
} prng_id_parms_t;

/**
* PRNG reseed parameter structure definition
*/
typedef struct prng_reseed_parms_def
{
    prng_ctx_t	ctx;
	uint8_t* 	entropy;
	size_t 		entropySize;
} prng_reseed_parms_t;

/**
* PRNG read parameter structure definition
*/
typedef struct prng_read_parms_def
{
    prng_ctx_t	ctx;
    uint8_t* 	buffer;
    size_t 		size;
} prng_read_parms_t;

/**
* PRNG selftest parameter structure definition
*/
typedef struct prng_selftest_parms_def
{
    prng_ctx_t	ctx;
    uint8_t* 	scratch;
    size_t* 	scratch_size;
} prng_selftest_parms_t;

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_CRYPTO_MANAGER_PRNG_SERVICE_H_ */
