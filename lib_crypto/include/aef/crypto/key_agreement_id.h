
/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported key agreement identifiers.
*
*/
#ifndef  UNIKEY_CRYPTO_KEY_AGREEMENT_ID_HEADER_GUARD
# define UNIKEY_CRYPTO_KEY_AGREEMENT_ID_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Enumeration of supported key agreement identifiers.
	*/
	typedef enum key_agreement_id
	{
		/**
		* Curve-25519
		*/
		KEY_AGREEMENT_CURVE25519 = 0x0400,

	} key_agreement_id_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_KEY_AGREEMENT_ID_HEADER_GUARD
