/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
 *
 * \brief List of supported block cipher identifiers.
 */
#ifndef  UNIKEY_CRYPTO_BLOCK_CIPHER_ID_HEADER_GUARD
# define UNIKEY_CRYPTO_BLOCK_CIPHER_ID_HEADER_GUARD

#include <aef/crypto/block_cipher_id.h>
#include <aef/crypto/crypto_status.h>
#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
 * Enumeration of supported block cipher identifiers.
 */
typedef enum block_cipher_id
{
    /**
     * AES-128-ECB
     */
    BLOCK_CIPHER_ID_AES_128_ECB                             =   0x0001,

    /**
     * AES-128-CBC
     */
    BLOCK_CIPHER_ID_AES_128_CBC                             =   0x0002,

    /**
     * AES-192-ECB
     */
    BLOCK_CIPHER_ID_AES_192_ECB                             =   0x0011,

    /**
     * AES-192-CBC
     */
    BLOCK_CIPHER_ID_AES_192_CBC                             =   0x0012,

    /**
     * AES-256-ECB
     */
    BLOCK_CIPHER_ID_AES_256_ECB                             =   0x0021,

    /**
     * AES-256-CBC
     */
    BLOCK_CIPHER_ID_AES_256_CBC                             =   0x0022,

    /**
     * AES-256-2X-ECB - Double-round variant
     */
    BLOCK_CIPHER_ID_AES_256_2X_ECB                          =   0x0031,

    /**
     * AES-256-2X-CBC - Double-round variant
     */
    BLOCK_CIPHER_ID_AES_256_2X_CBC                          =   0x0032,

    /**
     * AES-256-3X-ECB - Triple-round variant
     */
    BLOCK_CIPHER_ID_AES_256_3X_ECB                          =   0x0041,

    /**
     * AES-256-3X-CBC - Triple-round variant
     */
    BLOCK_CIPHER_ID_AES_256_3X_CBC                          =   0x0042,

} block_cipher_id_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_BLOCK_CIPHER_ID_HEADER_GUARD
