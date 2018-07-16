
/**
* crypto_prng_register.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto prng register definitions.
*/

#ifndef CRYPTO_PRNG_REGISTER_H
#define CRYPTO_PRNG_REGISTER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* PRNG crypto registration routines
	*/
	crypto_status_t prng_register(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_PRNG_REGISTER_H
    
