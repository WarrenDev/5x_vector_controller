/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
 *
 * \brief List of supported message digest identifiers.
 */
#ifndef  UNIKEY_CRYPTO_MESSAGE_DIGEST_ID_HEADER_GUARD
# define UNIKEY_CRYPTO_MESSAGE_DIGEST_ID_HEADER_GUARD

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
typedef enum message_digest_id
{
	/**
	 * SHA-224
	 */
	MESSAGE_DIGEST_SHA2_224									=	0x0100,

    /**
     * SHA-256
     */
    MESSAGE_DIGEST_SHA2_256                                 =   0x0101,

    /**
     * SHA-384
     */
    MESSAGE_DIGEST_SHA2_384                                 =   0x0102,

    /**
     * SHA-512
     */
    MESSAGE_DIGEST_SHA2_512                                 =   0x0103,

    /**
     * SHA-512/224
     */
    MESSAGE_DIGEST_SHA2_512_224                             =   0x0104,

    /**
     * SHA-512/256
     */
    MESSAGE_DIGEST_SHA2_512_256                             =   0x0105,

    /**
     * SHA3-224
     */
    MESSAGE_DIGEST_SHA3_224                                 =   0x0108,

    /**
     * SHA3-256
     */
    MESSAGE_DIGEST_SHA3_256                                 =   0x0109,

    /**
     * SHA3-384
     */
    MESSAGE_DIGEST_SHA3_384                                 =   0x010A,

    /**
     * SHA3-512
     */
    MESSAGE_DIGEST_SHA3_512                                 =   0x010B,

} message_digest_id_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_MESSAGE_DIGEST_ID_HEADER_GUARD
