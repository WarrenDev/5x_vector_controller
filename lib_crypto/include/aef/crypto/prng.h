
/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Generic interface for pseudo random number generator support.
*
*/
#ifndef  UNIKEY_CRYPTO_PRNG_HEADER_GUARD
# define UNIKEY_CRYPTO_PRNG_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/prng_id.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* The PRNG Context is an opaque pointer to the context structure used
	* by the pseudo random number generator.  This structure must be
	* initialized by calling the appropriate initialization function prior to use.
	*/
	typedef void* prng_ctx_t;

	/**
	* The prng vtable type, which is used internally for working with
	* specific pseudo random number generator implementations.
	*/
	typedef struct prng_vtable
	{
		/**
		* Get the ID of this pseudo random number generator.
		*
		* \returns the PRNG ID, which can be used to get the string name.
		*/
		prng_id_t (*id)();

        /**
        * Initialize a PRNG with the given entropy data and personalization data.
        * NOTE: Initialize must only be called once.  Use PRNG_reseed to reseed the
        * PRNG with new entropy data.
        *
        * \param entropy       A pointer to raw entropy bytes to use in initializing
        *                      this PRNG.
        * \param entropySize   The size in bytes of the entropy.
        * \param personal      A pointer to personalization data to use in initializing this
        *                      PRNG.
        * \param personalSize  The size in bytes of the personalization data.
		* \param ctx		   A pointer to the PRNG context memory area pointer.
		* \param ctxSize	   The size in bytes of the ctx.
        *
        * \returns CRYPTO_STATUS_SUCCESS if initialization was successful.
		*
        *          CRYPTO_FAILURE_GENERAL if something went wrong.
		*
		*          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
		*              not large enough for holding the context of this digest.
		*              If this status is returned, then ctx_size is updated with
		*              the size of the context as required by this authentication digest.
		*/
		crypto_status_t (*init)(
			uint8_t* entropy, size_t entropySize, uint8_t* personal, size_t personalSize, prng_ctx_t* ctx, size_t* ctx_size);

        /**
        * Reseed a PRNG with the given entropy data.
        *
        * \param ctx		   A pointer to the PRNG context memory area.
        * \param entropy       A pointer to raw entropy bytes to use in initializing
        *                      this PRNG.
        * \param entropySize   The size in bytes of the entropy.
        *
        * \returns              CRYPTO_STATUS_SUCCESS if successful.
        *                       CRYPTO_FAILURE_GENERAL if something went wrong.
        */
		crypto_status_t (*reseed)(
            prng_ctx_t ctx, uint8_t* entropy, size_t entropySize);

        /**
        * Read a stream of pseudorandom bytes from the PRNG.
        *
        * \param ctx		   A pointer to the PRNG context memory area.
        * \param buffer        The buffer to read into.
        * \param size          The number of bytes to read.
        *
        * \returns              CRYPTO_STATUS_SUCCESS if successful.
        *                       CRYPTO_FAILURE_GENERAL if something went wrong.
        */
		crypto_status_t (*read)(
            prng_ctx_t ctx, uint8_t* buffer, size_t size);

		/**
		* Perform a self-test of this PRNG with the provided scratchpad.
		*
        * \param ctx		   A pointer to the PRNG context memory area.
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
		crypto_status_t (*selftest)(
            prng_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);

	} prng_vtable_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_PRNG_HEADER_GUARD
