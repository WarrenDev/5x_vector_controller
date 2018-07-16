
/**
* crypto_sha2_register.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto SHA2 register definitions.
*/

#ifndef CRYPTO_SHA2_REGISTER_H
#define CRYPTO_SHA2_REGISTER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* SHA2 crypto registration routines
	*/
	crypto_status_t sha2_224_register(void);
	crypto_status_t sha2_256_register(void);
	crypto_status_t sha2_384_register(void);
	crypto_status_t sha2_512_register(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_SHA2_REGISTER_H
