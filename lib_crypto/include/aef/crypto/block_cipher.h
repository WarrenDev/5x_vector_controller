/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
 *
 * \brief Generic interface for a block cipher.
 */
#ifndef  UNIKEY_CRYPTO_BLOCK_CIPHER_HEADER_GUARD
# define UNIKEY_CRYPTO_BLOCK_CIPHER_HEADER_GUARD

#include <aef/crypto/block_cipher_id.h>
#include <aef/crypto/crypto_status.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
 * Enumeration for initializing a block cipher.
 */
typedef enum block_cipher_init
{

	/**
	* Initialize this block cipher for decryption.
	*/
	BLOCK_CIPHER_INIT_DECRYPT,

	/**
     * Initialize this block cipher for encryption.
     */
    BLOCK_CIPHER_INIT_ENCRYPT,

} block_cipher_init_t;

/**
 * The Block Cipher Context is an opaque pointer to the context structure used
 * by the block cipher during encryption operations.  This structure must be
 * initialized by calling the appropriate initialization function prior to use.
 */
typedef void* block_cipher_ctx_t;

/**
 * The block cipher vtable type, which is used internally for working with
 * specific block ciphers.
 */
typedef struct block_cipher_vtable
{
    /**
     * Get the ID of this block cipher.
     *
     * \returns the block cipher ID, which can be used to get the string name
     *          of this cipher.
     */
    block_cipher_id_t (*id)();

    /**
     * Get the block size of this cipher.
     *
     * \returns the block size of this cipher.
     */
    size_t (*block_size)();

    /**
     * Get the key size of this cipher.
     *
     * \returns the key size of this cipher.
     */
    size_t (*key_size)();

    /**
     * Get the number of rounds for this cipher.
     *
     * \returns the number of rounds for this cipher.
     */
    size_t (*rounds)(block_cipher_ctx_t ctx);

    /**
     * Initialize this block cipher.
     *
     * \param init_param        The initialization parameter.
     * \param key               The key to use for this block cipher.
     * \param key_size          The size of the key for this block cipher.
     * \param ctx               The context to initialize.
     * \param ctx_size          The size of the context for this cipher.
     *
     * \returns CRYPTO_STATUS_SUCCESS on successful initialization.
     *
     *          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
     *
     *          CRYPTO_FAILURE_INVALID_KEY_SIZE if the key size is invalid,
     *
     *          CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE if the context size is
     *              not large enough for holding the context of this cipher.
     *              If this status is returned, then ctx_size is updated with
     *              the size of the context as required by this cipher.
     */
    crypto_status_t (*init)(
        block_cipher_init_t init_param, const uint8_t* key, size_t key_size,
        block_cipher_ctx_t ctx, size_t* ctx_size);

    /**
     * Set the initialization vector for this cipher, if supported.
     *
     * \param ctx           The context.
     * \param iv            The initialization vector for this cipher.
     * \param iv_size       The size of the initialization vector.
     *
     * \returns CRYPTO_SUCCESS if the IV was successfully set.
     *
     *          CRYPTO_FAILURE_INVALID_IV_SIZE if the initialization vector size
     *              is invalid.
     *
     *          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
     *              accept an initialization vector.
     */
    crypto_status_t (*set_iv)(
        block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size);

    /**
     * Encrypt data using this cipher.
     *
     * The input and output buffers must be the same size, denoted by
     * input_size, and the size must be a multiple of the block size.
     *
     * Internally, if this cipher supports an IV, the IV is updated after this
     * call is completed.
     *
     * \param ctx           The context.
     * \param input         The input data to encrypt.
     * \param input_size    The size of the input data to encrypt.
     * \param output        The output buffer to store the encrypted data.
     *
     * \returns CRYPTO_SUCCESS if the blocks were successfully encrypted.
     *
     *          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
     *              a multiple of the block size.
     *
     *          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
     *              support encryption.
     */
    crypto_status_t (*encrypt)(
        block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);

    /**
     * Decrypt data using this cipher.
     *
     * The input and output buffers must be the same size, denoted by
     * input_size, and the size must be a multiple of the block size.
     *
     * Internally, if this cipher supports an IV, the IV is updated after this
     * call is completed.
     *
     * \param ctx           The context.
     * \param input         The input data to decrypt.
     * \param input_size    The size of the input data to decrypt.
     * \param output        The output buffer to store the decrypted data.
     *
     * \returns CRYPTO_SUCCESS if the blocks were successfully decrypted.
     *
     *          CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE if the input_size was not
     *              a multiple of the block size.
     *
     *          CRYPTO_FAILURE_UNSUPPORTED_OPERATION if this cipher does not
     *              support decryption.
     */
    crypto_status_t (*decrypt)(
        block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output);

    /**
     * Perform a self-test of this block cipher with the provided scratchpad.
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
        block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size);

} block_cipher_vtable_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_BLOCK_CIPHER_HEADER_GUARD
