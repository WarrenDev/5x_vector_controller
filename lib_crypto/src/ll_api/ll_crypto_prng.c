

/**
* ll_crypto_prng.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief low-level crypto prng API implementation.
*/

#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_prng.h>
#include "stddef.h"
#include "../src/prng/ref/crypto_prng_register.h"
#include "../src/prng/MK64/mk64_prng_register.h"

/**
* Register the reference PRNG sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_prng (void)
{

	return prng_register();

} // ll_crypto_register_prng

/**
* Register the MK64 PRNG sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if registration was successful.
*
*          CRYPTO_FAILURE_GENERAL if registration failed.
*/
crypto_status_t  ll_crypto_register_mk64_prng (void)
{

	return mk64_prng_register();

} // ll_crypto_register_mk64_prng

/**
* Get the ID of this prng.
*
* \returns the PRNG ID, which can be used to get the string name
*          of this PRNG.
*/
prng_id_t	ll_crypto_prng_get_id (void)
{

	prng_vtable_t* prng_impl = (prng_vtable_t*)crypto_registry_get_vtable(PSEUDO_RNG);
	if (prng_impl == NULL)
		return 0;

	return prng_impl->id();

} // ll_crypto_prng_get_id

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
crypto_status_t ll_crypto_prng_init (uint8_t* entropy, size_t entropySize, uint8_t* personal, size_t personalSize, prng_ctx_t* ctx, size_t* ctx_size)
{

	prng_vtable_t* prng_impl = (prng_vtable_t*)crypto_registry_get_vtable(PSEUDO_RNG);
	if (prng_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return prng_impl->init(entropy, entropySize, personal, personalSize, ctx, ctx_size);

}  // ll_crypto_prng_init

/**
* Reseed a PRNG with the given entropy data.
*
* \param ctx           A pointer the PRNG context memory area.
* \param entropy       A pointer to raw entropy bytes to use in initializing
*                      this PRNG.
* \param entropySize   The size in bytes of the entropy.
*
* \returns              CRYPTO_STATUS_SUCCESS if successful.
*                       CRYPTO_FAILURE_GENERAL if something went wrong.
*/
crypto_status_t ll_crypto_prng_reseed (prng_ctx_t ctx, uint8_t* entropy, size_t entropySize)
{

	prng_vtable_t* prng_impl = (prng_vtable_t*)crypto_registry_get_vtable(PSEUDO_RNG);
	if (prng_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return prng_impl->reseed(ctx, entropy, entropySize);

} // ll_crypto_prng_reseed

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
crypto_status_t ll_crypto_prng_read (prng_ctx_t ctx, uint8_t* buffer, size_t size)
{

	prng_vtable_t* prng_impl = (prng_vtable_t*)crypto_registry_get_vtable(PSEUDO_RNG);
	if (prng_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return prng_impl->read(ctx, buffer, size);

} // ll_crypto_prng_read

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
crypto_status_t ll_crypto_prng_selftest (prng_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{

	prng_vtable_t* prng_impl = (prng_vtable_t*)crypto_registry_get_vtable(PSEUDO_RNG);
	if (prng_impl == NULL)
		return CRYPTO_FAILURE_GENERAL;

	return prng_impl->selftest(ctx, scratch, scratch_size);

} // ll_crypto_prng_selftest

