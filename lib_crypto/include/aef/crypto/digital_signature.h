
/**
* digital_signature.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Generic interface for digital signature support.
*/

#ifndef  UNIKEY_CRYPTO_DIGITAL_SIGNATURE_HEADER_GUARD
# define UNIKEY_CRYPTO_DIGITAL_SIGNATURE_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/digital_signature_id.h>
#include <aef/crypto/message_digest.h>
#include <stddef.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* The Digital Signature Context is an opaque pointer to the context structure used
	* by the message digest during hashing operations.  This structure must be
	* initialized by calling the appropriate initialization function prior to use.
	*/
	typedef void* digital_signature_ctx_t;

	/**
	* The digital signature vtable type, which is used internally for working with
	* specific digital signature implementations.
	*/
	typedef struct digital_signature_vtable
	{
		/**
		* Get the ID of this digital signature digest.
		*
		* \returns the digital signature ID, which can be used to get the string name
		*          of this digest.
		*/
		digital_signature_id_t (*id)();

		/**
		* Get the output size for this digital signature digest.
		*
		* \returns the output size for this digital signature digest.
		*/
		size_t (*digest_size)();

		/**
		* Initialize this digital signature instance.
		*
		* \returns CRYPTO_STATUS_SUCCESS on successful initialization.
		*
		*          CRYPTO_FAILURE_INITIALIZATION for a general init failure,
		*/
		crypto_status_t (*init)();

		/**
		* Sign the key pair.
		*
		*
		* \param pk				Public key
		* \param sk				Secret key
		* \param random_bytes	Random bytes
		*
		* \returns CRYPTO_SUCCESS if this operation was successful.
		*
		*          CRYPTO_FAILURE_GENERAL if something went wrong.
		*/
		crypto_status_t(*sign_keypair)(
			uint8_t* pk, uint8_t* sk, uint8_t* random_bytes);

		/**
		* Sign a message
		*
		*
		* \param sm         Signaure returned
		* \param sm_size    Size of the signature in bytes
		* \param m			Message to sign
		* \param m_size		Size of the message
		* \param sk			Secret key
        * \param pk         Public key
		*
		* \returns CRYPTO_SUCCESS if this operation was successful.
		*
		*          CRYPTO_FAILURE_GENERAL if something went wrong.
		*/
		crypto_status_t (*sign)(
            uint8_t* sm, size_t* sm_size, const uint8_t* m, size_t m_size, const uint8_t* sk, const uint8_t* pk);

		/**
		* Verify a signature
		*
		* \param m          Message
		* \param m_size     Size of the message
		* \param sm         Signature to verify
		* \param sm_size    Size of the signature
		* \param pk         PUblic key
		*
		* \returns CRYPTO_SUCCESS if this operation was successful.
4		*
		*          CRYPTO_FAILURE_GENERAL if something went wrong.
		*/
		crypto_status_t (*sign_open)(
			uint8_t* m, size_t* m_size, const uint8_t* sm, size_t sm_size, const uint8_t* pk);

		/**
		* Perform a self-test of this digital signature digest with the provided scratchpad.
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

	} digital_signature_vtable_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_DIGITAL_SIGNATURE_HEADER_GUARD
