
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
#ifndef  UNIKEY_CRYPTO_PRNG_ID_HEADER_GUARD
# define UNIKEY_CRYPTO_PRNG_ID_HEADER_GUARD

#include <aef/crypto/crypto_status.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Enumeration of supported pseudo random number generator identifiers.
	*/
	typedef enum prng_id
	{
		/**
		* Pseudo random number generator
		*/
		PSEUDO_RNG = 0x0500,

	} prng_id_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_PRNG_ID_HEADER_GUARD
