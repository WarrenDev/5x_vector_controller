
/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Generic interface for key agreement support.
*
*/
#ifndef  UNIKEY_CRYPTO_KEY_AGREEMENT_HEADER_GUARD
# define UNIKEY_CRYPTO_KEY_AGREEMENT_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/key_agreement_id.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* The Key Agreement Context is an opaque pointer to the context structure used
	* by the message digest during hashing operations.  This structure must be
	* initialized by calling the appropriate initialization function prior to use.
	*/
	typedef void* key_agreement_ctx_t;

	/**
	* The key agreement vtable type, which is used internally for working with
	* specific key agreement implementations.
	*/
	typedef struct key_agreement_vtable
	{
		/**
		* Get the ID of this key agreement digest.
		*
		* \returns the key agreement ID, which can be used to get the string name
		*          of this digest.
		*/
		key_agreement_id_t (*id)();

		/**
		* create secret.
		*
		*
		* \param secret_key		
		* \param input_bytes	
		*
		* \returns CRYPTO_SUCCESS if this operation was successful.
		*
		*          CRYPTO_FAILURE_GENERAL if something went wrong.
		*/
		crypto_status_t (*create_secret)(
			uint8_t* secret_key, const uint8_t* input_bytes);

		/**
		* Create public
		*
		*
		* \param public_key
		* \param secret_key
		*
		* \returns CRYPTO_SUCCESS if this operation was successful.
		*
		*          CRYPTO_FAILURE_GENERAL if something went wrong.
		*/
		crypto_status_t (*create_public)(
			uint8_t* public_key, const uint8_t* secret_key);

		/**
		* Cerate shared secret
		*
		* \param shared_secret
		* \param peer_public_key
		* \param my_secret_key
		*
		* \returns CRYPTO_SUCCESS if this operation was successful.
		*
		*          CRYPTO_FAILURE_GENERAL if something went wrong.
		*/
		crypto_status_t (*create_shared_secret)(
			uint8_t* shared_secret, const uint8_t* peer_public_key, const uint8_t* my_secret_key);

		/**
		* Perform a self-test of this key agreement digest with the provided scratchpad.
		*
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
			uint8_t* scratch, size_t* scratch_size);

	} key_agreement_vtable_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_KEY_AGREEMENT_HEADER_GUARD
