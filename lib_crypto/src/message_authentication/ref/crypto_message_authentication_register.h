
/**
* crypto_message_authentication_register.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto message authenticaiton register definitions.
*/

#ifndef CRYPTO_MESSAGE_AUTHENTICATION_REGISTER_H
#define CRYPTO_MESSAGE_AUTHENTICATION_REGISTER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* Message authentication crypto registration routines
	*/
	crypto_status_t message_authentication_256_register(void);
	crypto_status_t message_authentication_384_register(void);
	crypto_status_t message_authentication_512_register(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_MESSAGE_AUTHENTICATION_REGISTER_H
