
/**
* mk64_prng_core.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64 crypto PRNG core implementation.
*/

#include "mk64_prng_core.h"

/**
* Initialize a PRNG with the given entropy data and personalization data.
* NOTE: Initialize must only be called once.  Use PRNG_reseed to reseed the
* PRNG with new entropy data.
*
* \param entropy       A pointer to raw entropy bytes to use in initializing
*                      this PRNG.
* \param entropySize   The size in bytes of the entropy.
* \param personal      The personalization data to use in initializing this
*                      PRNG.
* \param personalSize  The size in bytes of the personalization data.
* \param ctx           A pointer the PRNG context memory area.
* \param ctx_size	   The size in bytes of the context.
*
* \returns              CRYPTO_STATUS_SUCCESS if initialization was successful.
*                       CRYPTO_FAILURE_GENERAL if something went wrong.
*						CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
*							not large enough for holding the context of this digest.
*							If this status is returned, then ctx_size is updated with
*							the size of the context as required by this authentication digest.
*/
crypto_status_t mk64_prng_core_init (uint8_t* entropy, size_t entropySize, uint8_t* personal, size_t personalSize, prng_ctx_t ctx, size_t* ctx_size)
{
	if ( ctx != NULL )
	{
		mk64_prng_ctx_t* mk64_prng_ctx = (mk64_prng_ctx_t*)ctx;
		RNGA_Init(mk64_prng_ctx->base);
		if ( entropySize >= sizeof(uint32_t) )
		{
			uint32_t initialSeed;
			memcpy ( &initialSeed, entropy, sizeof(uint32_t) );
			RNGA_Seed(mk64_prng_ctx->base, initialSeed);
		}
		return CRYPTO_STATUS_SUCCESS;
	}
	return CRYPTO_FAILURE_GENERAL;
}

/**
* Reseed a PRNG with the given entropy data.
*
* \param ctx           A pointer the PRNG context memory area.
* \param entropy       A pointer to raw entropy bytes to use in initializing
*                      this PRNG.
* \param entropySize   The size in bytes of the entropy.
*
* \returns	CRYPTO_STATUS_SUCCESS if successful.
*           CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t mk64_prng_core_reseed (prng_ctx_t ctx, uint8_t* entropy, size_t entropySize)
{
	if ( ctx != NULL )
	{
		mk64_prng_ctx_t* mk64_prng_ctx = (mk64_prng_ctx_t*)ctx;
		if ( entropySize >= sizeof(uint32_t) )
		{
			uint32_t newSeed;
			memcpy ( &newSeed, entropy, sizeof(uint32_t) );
			RNGA_Seed(mk64_prng_ctx->base, newSeed);
		}
		return CRYPTO_STATUS_SUCCESS;
	}
	return CRYPTO_FAILURE_GENERAL;
}

/**
* Read a stream of pseudorandom bytes from the PRNG.
*
* \param ctx           A pointer the PRNG context memory area.
* \param buffer        The buffer to read into.
* \param size          The number of bytes to read.
*
* \returns              CRYPTO_STATUS_SUCCESS if successful.
*                       CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t mk64_prng_core_read (prng_ctx_t ctx, uint8_t* buffer, size_t size)
{
	if ( ctx != NULL )
	{
		mk64_prng_ctx_t* mk64_prng_ctx = (mk64_prng_ctx_t*)ctx;
		if ( RNGA_GetRandomData(mk64_prng_ctx->base, buffer, size ) == kStatus_Success )
		{
			return CRYPTO_STATUS_SUCCESS;
		}
	}
	return CRYPTO_FAILURE_GENERAL;
}

/**
* Perform a self-test of this prng with the provided scratchpad.
*
* \param ctx           A pointer the PRNG context memory area.
* \param scratch       The scratchpad to use for self-testing.
* \param scratch_size  The size of the scratchpad.
*
* \returns CRYPTO_STATUS_SUCCESS on a successful self-test.
*
*          CRYPTO_FAILURE_INSUFFICIENT_SIZE if the scratchpad is not
*              large enough for a self-test.  The required size will be
*              written to scratch_size.
*
*          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this implementation
*              does not support self testing.
*
*          CRYPTO_FAILURE_SELF_TEST if the self test fails.
*/
crypto_status_t mk64_prng_core_selftest (prng_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{
	return CRYPTO_FAILURE_UNSUPPORTED_OPERATION;
}

