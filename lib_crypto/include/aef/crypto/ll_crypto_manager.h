
/**
* ll_crypto_manager.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto sub-system manager definition.
*/

#ifndef LL_CRYPTO_MANAGER_H
#define LL_CRYPTO_MANAGER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* low-level crypto sub-system routines
*/
crypto_status_t  ll_crypto_init		( void );
crypto_status_t  ll_crypto_deinit	( void );

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_MANAGER_H
