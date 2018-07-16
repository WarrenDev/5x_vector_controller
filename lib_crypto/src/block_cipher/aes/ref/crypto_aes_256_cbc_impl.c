
/**
* crypto_aes_256_cbc_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto AES 256 CBC implementation.
*/

#include <aef/crypto/block_cipher.h>
#include "string.h"
#include "crypto_aes_core.h"

#define	IMPL_CIPHER_ID			    BLOCK_CIPHER_ID_AES_256_CBC
#define IMPL_BLOCK_SIZE			    (AES_BLOCK_SIZE * 8)
#define IMPL_KEY_SIZE			    AES_BITSIZE_256
#define CIPHER_CONTEXT_VALID(_ctx)	((_ctx) != NULL)

/**
* Get the ID of this block cipher.
*
* \returns the block cipher ID, which can be used to get the string name
*          of this cipher.
*/
static
block_cipher_id_t aes_256_cbc_get_cipher_id(void)
{
	return IMPL_CIPHER_ID;
}

/**
* Get the block size of this cipher.
*
* \returns the block size of this cipher.
*/
static
size_t aes_256_cbc_get_block_size(void)
{

	return IMPL_BLOCK_SIZE;
}

/**
* Get the key size of this cipher.
*
* \returns the key size of this cipher.
*/
static
size_t aes_256_cbc_get_key_size(void)
{
	return IMPL_KEY_SIZE;
}

/**
* Get the number of rounds for this cipher.
*
* \returns the number of rounds for this cipher.
*/
static
size_t aes_256_cbc_get_rounds(block_cipher_ctx_t ctx)
{
    aes_ctx* _ctx = (aes_ctx*)ctx;
    return ((CIPHER_CONTEXT_VALID(_ctx)) ? _ctx->Nr : 0);
}

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
static
crypto_status_t aes_256_cbc_init(block_cipher_init_t init_param, const uint8_t* key, size_t key_size,
block_cipher_ctx_t ctx, size_t* ctx_size)
{

    aes_ctx* _ctx = (aes_ctx*)ctx;
    
    /**
	* Validate parameters
	*/
	if (*ctx_size < sizeof(aes_ctx))
	{
		*ctx_size = sizeof(aes_ctx);
		return CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE;
	}

	if ((key_size != IMPL_KEY_SIZE) || (key == NULL))
		return CRYPTO_FAILURE_INVALID_KEY_SIZE;

	/**
	* Initialize the context
	*/
	_ctx->mode = 0;
	memset(_ctx->reg, 0, sizeof(_ctx->reg));

	/**
	* Set the key
	*/
	aes_core_set_key(_ctx, (unsigned char*)key, key_size, init_param);

	return CRYPTO_STATUS_SUCCESS;

}

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
static
crypto_status_t aes_256_cbc_set_iv(block_cipher_ctx_t ctx, const uint8_t* iv, size_t iv_size)
{

    aes_ctx* _ctx = (aes_ctx*)ctx;
    
    if (iv_size != AES_BLOCK_SIZE)
		return CRYPTO_FAILURE_INVALID_IV_SIZE;

	/**
	* Initialize the IV and indicate that it has been initialized
	*/
	_ctx->mode = AES_MODE_CBC;
	memcpy(_ctx->reg, iv, iv_size);

	return CRYPTO_STATUS_SUCCESS;

}

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
*
*		   CRYPTO_FAILURE_INCORRECT_MODE if CBC mode is not selected and IV not set.
*/
static
crypto_status_t aes_256_cbc_encrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	int8_t   i;
	uint8_t  temp_block[AES_BLOCK_SIZE];
    aes_ctx* _ctx = (aes_ctx*)ctx;
    uint8_t* iv = (uint8_t*)_ctx->reg;

	if (_ctx->mode != AES_MODE_CBC)
		return CRYPTO_FAILURE_INCORRECT_MODE;

	while (input_size > 0)
	{
		memcpy(temp_block, input, AES_BLOCK_SIZE);

		/**
		* XOR block with IV for CBC
		*/
		for (i = 0; i < AES_BLOCK_SIZE; i++)
			temp_block[i] ^= iv[i];

		aes_core_encrypt(_ctx, (unsigned char*)temp_block, (unsigned char*)output);

		/**
		* Store IV for the next block
		*/
		memcpy(iv, output, AES_BLOCK_SIZE);

		input += AES_BLOCK_SIZE;
		output += AES_BLOCK_SIZE;
		input_size--;
	}

	return CRYPTO_STATUS_SUCCESS;
}

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
*
*		   CRYPTO_FAILURE_INCORRECT_MODE if CBC mode is not selected and IV not set.
*/
static
crypto_status_t aes_256_cbc_decrypt(block_cipher_ctx_t ctx, const uint8_t* input, size_t input_size, uint8_t* output)
{

	int8_t   i;
	uint8_t  temp_block[AES_BLOCK_SIZE];
    aes_ctx* _ctx = (aes_ctx*)ctx;
    uint8_t* iv = (uint8_t*)_ctx->reg;

	if (_ctx->mode != AES_MODE_CBC)
		return CRYPTO_FAILURE_INCORRECT_MODE;

	while (input_size > 0)
	{
		memcpy(temp_block, input, AES_BLOCK_SIZE);
		aes_core_decrypt(_ctx, (unsigned char*)temp_block, (unsigned char*)output);

		/**
		* XOR block with IV for CBC
		*/
		for (i = 0; i < AES_BLOCK_SIZE; i++)
			output[i] ^= iv[i];

		/**
		* Store IV for the next block
		*/
		memcpy(iv, temp_block, AES_BLOCK_SIZE);

		input += AES_BLOCK_SIZE;
		output += AES_BLOCK_SIZE;
		input_size--;
	}

	return CRYPTO_STATUS_SUCCESS;
}

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
static
crypto_status_t aes_256_cbc_selftest(block_cipher_ctx_t ctx, uint8_t* scratch, size_t* scratch_size)
{
	return CRYPTO_STATUS_SUCCESS;
}

/**
* The block cipher vtable for this cipher
*/
const block_cipher_vtable_t aes_256_cbc_vtable =
{
	aes_256_cbc_get_cipher_id,
	aes_256_cbc_get_block_size,
	aes_256_cbc_get_key_size,
	aes_256_cbc_get_rounds,
	aes_256_cbc_init,
	aes_256_cbc_set_iv,
	aes_256_cbc_encrypt,
	aes_256_cbc_decrypt,
	aes_256_cbc_selftest
};

