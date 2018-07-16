
/**
* digital_signature_id.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported digital signature identifiers.
*
*/

#ifndef  UNIKEY_CRYPTO_DIGITAL_SIGNATURE_ID_HEADER_GUARD
# define UNIKEY_CRYPTO_DIGITAL_SIGNATURE_ID_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Enumeration of supported digital signature identifiers.
	*/
	typedef enum digital_signature_id
	{
		/**
		* ED-25519 (Reference)
		*/
		DIGITAL_SIGNATURE_ED25519 = 0x0300,

        /**
        * ED-25519 (Donna)
        */
        DIGITAL_SIGNATURE_ED25519_DONNA = 0x0301,

	} digital_signature_id_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_DIGITAL_SIGNATURE_ID_HEADER_GUARD
