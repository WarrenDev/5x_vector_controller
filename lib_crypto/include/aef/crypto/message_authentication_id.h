
/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported message authentication identifiers.
*/
#ifndef  UNIKEY_CRYPTO_MESSAGE_AUTHENTICATION_ID_HEADER_GUARD
# define UNIKEY_CRYPTO_MESSAGE_AUTHENTICATION_ID_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <aef/crypto/message_digest_id.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Enumeration of supported message authentication identifiers.
	*/
	typedef enum message_authentication_id
	{
		/**
		* SHA2-224
		*/
		MESSAGE_AUTHENTICATION_224 = 0x0200,

		/**
		* SHA2-256
		*/
		MESSAGE_AUTHENTICATION_256 = 0x0201,

		/**
		* SHA2-384
		*/
		MESSAGE_AUTHENTICATION_384 = 0x0202,

		/**
		* SHA2-512
		*/
		MESSAGE_AUTHENTICATION_512 = 0x0203,

	} message_authentication_id_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_MESSAGE_AUTHENTICATION_ID_HEADER_GUARD
