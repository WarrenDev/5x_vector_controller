
/**
* crypto_aes_register.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto AES register definitions.
*/

#ifndef CRYPTO_AES_REGISTER_H
#define CRYPTO_AES_REGISTER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* AES crypto registration routines
*/
crypto_status_t aes_128_ecb_register	(void);
crypto_status_t aes_192_ecb_register	(void);
crypto_status_t aes_256_ecb_register	(void);
crypto_status_t aes_128_cbc_register	(void);
crypto_status_t aes_192_cbc_register	(void);
crypto_status_t aes_256_cbc_register	(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_AES_REGISTER_H
