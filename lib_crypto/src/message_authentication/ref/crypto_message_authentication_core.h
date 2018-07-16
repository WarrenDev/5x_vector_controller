
/**
* crypto_message_authentication_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto message authentication core definitions.
*/

#ifndef _CRYPTO_MESSAGE_AUTHENTICATION_CORE_H
#define _CRYPTO_MESSAGE_AUTHENTICATION_CORE_H

#include <aef/crypto/crypto_status.h>
#include <stddef.h>
#include <stdint.h>
#include "../src/message_digest/sha2/ref/crypto_sha2_core.h"
#include "../src/message_digest/sha2/ref/crypto_sha2_register.h"

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/*** Message Authentication Context Structure *******************************/
typedef struct _MESSAGE_AUTHENTICATION_CTX
{
    SHA2_CTX	sha2_context;
    size_t      sha2_context_size;
} MESSAGE_AUTHENTICATION_CTX;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif
