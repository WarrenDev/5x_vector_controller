
/**
* crypto_key_agreement_register.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto key agreement register definitions.
*/

#ifndef CRYPTO_KEY_AGREEMENT_REGISTER_H
#define CRYPTO_KEY_AGREEMENT_REGISTER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* Key agreement crypto registration routines
	*/
	crypto_status_t key_agreement_curve25519_register(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_KEY_AGREEMENT_REGISTER_H
