
/**
* message_digest.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Generic interface for message digests and secure hashes.
*/
#ifndef  UNIKEY_CRYPTO_MESSAGE_DIGEST_HEADER_GUARD
# define UNIKEY_CRYPTO_MESSAGE_DIGEST_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/message_digest_id.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
 * The Message Digest Context is an opaque pointer to the context structure used
 * by the message digest during hashing operations.  This structure must be
 * initialized by calling the appropriate initialization function prior to use.
 */
typedef void* message_digest_ctx_t;

/**
 * The message digest vtable type, which is used internally for working with
 * specific message digests.
 */
typedef struct message_digest_vtable
{
    /**
     * Get the ID of this message digest.
     *
     * \returns the message digest ID, which can be used to get the string name
     *          of this digest.
     */
    message_digest_id_t (*id)();

    /**
     * Get the input block size of this message digest.
     *
     * \returns the input block size of this message digest.
     */
    size_t (*block_size)();

    /**
     * Get the output size for this message digest.
     *
     * \returns the output size for this message digest.
     */
    size_t (*digest_size)();

    /**
     * Initialize this message digest.
     *
     * \param ctx               The context to initialize.
     * \param ctx_size          The size of the context for this digest.
     *
     * \returns CRYPTO_STATUS_SUCCESS on successful initialization.
     *
     *          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
     *
     *          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
     *              not large enough for holding the context of this digest.
     *              If this status is returned, then ctx_size is updated with
     *              the size of the context as required by this digest.
     */
    crypto_status_t (*init)(
		message_digest_ctx_t ctx, size_t* ctx_size);

    /**
     * Update this message digest with the given input data.
     *
     * The internal state of this message digest is updated with the given
     * input data.
     *
     * \param ctx           The context.
     * \param input         The input data to encrypt.
     * \param input_size    The size of the input data to encrypt.
     *
     * \returns CRYPTO_SUCCESS if this operation was successful.
     *
     *          CRYPTO_FAILURE_GENERAL if something went wrong.
     */
    crypto_status_t (*update)(
        message_digest_ctx_t ctx, const uint8_t* input, size_t input_size);

    /**
     * Finalize this message digest and write the output hash.
     *
     * \param ctx           The context.
     * \param output        The output buffer to store the message digest.
     * \param output_size   The size of the output buffer.
     *
     * \returns CRYPTO_SUCCESS if finalization completed successfully.
     *
     *          CRYPTO_FAILURE_INSUFFICIENT_SIZE if the output buffer is
     *              insufficient to perform this operation.  The required
     *              output size is stored in the output_size parameter.
     */
    crypto_status_t (*finalize)(
        message_digest_ctx_t ctx, uint8_t* output, size_t* output_size);

    /**
     * Perform a self-test of this message digest with the provided scratchpad.
     *
     * \param ctx               The context.
     * \param scratch           The scratchpad to use for self-testing.
     * \param scratch_size      The size of the scratchpad.
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
        message_digest_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);

} message_digest_vtable_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_MESSAGE_DIGEST_HEADER_GUARD
