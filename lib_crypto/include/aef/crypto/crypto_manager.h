
/**
* crypto_manager.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions use by the crypto manager.
*/

#ifndef CRYPTO_MANAGER_H
#define CRYPTO_MANAGER_H

#include "aef/crypto/crypto_status.h"

# ifdef   __cplusplus
extern "C" {
# endif

crypto_status_t  crypto_manager_init	( void );
crypto_status_t  crypto_manager_deinit	( void );

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_MANAGER_H
