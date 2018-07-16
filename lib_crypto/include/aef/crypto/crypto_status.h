/**
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
 *
 * \brief Crypto status codes.
 */
#ifndef  UNIKEY_CRYPTO_CRYPTO_STATUS_HEADER_GUARD
# define UNIKEY_CRYPTO_CRYPTO_STATUS_HEADER_GUARD

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
 * Crypto status codes.  These codes represent various success and failure
 * codes that can arise when using the UniKey crypto library.
 *
 * Regular status codes have the high bit of the 32-bit word unset.
 * Failure codes have the high bit set.
 */
typedef enum crypto_status
{
    /******** STATUS CODES ********/

    /**
     * The operation completed successfully.
     */
    CRYPTO_STATUS_SUCCESS                                   =   0x00000000,

    /******** FAILURE CODES ********/

    /**
     * A general failure.
     */
    CRYPTO_FAILURE_GENERAL                                  =   0x80000000,

    /**
     * The operation requested is not supported.
     */
    CRYPTO_FAILURE_UNSUPPORTED_OPERATION                    =   0x80000001,

    /**
     * The size of the data block provided for this operation was not
     * sufficient.
     */
    CRYPTO_FAILURE_INSUFFICIENT_SIZE                        =   0x80000002,

	/**
	* Incorrect mode for this operation
	*/
	CRYPTO_FAILURE_INCORRECT_MODE							=	0x80000003,

	/**
     * The requested self test failed.
     */
    CRYPTO_FAILURE_SELF_TEST                                =   0x80800000,

    /**
     * General failure during initialization.
     */
    CRYPTO_FAILURE_INITIALIZATION                           =   0x81000001,

    /**
     * The key size is not valid.
     */
    CRYPTO_FAILURE_INVALID_KEY_SIZE                         =   0x81000002,

    /**
     * The context buffer size is insufficient for initalization.
     */
    CRYPTO_FAILURE_INSUFFICIENT_CONTEXT_SIZE                =   0x81000003,

    /**
     * The initialization vector size is not valid.
     */
    CRYPTO_FAILURE_INVALID_IV_SIZE                          =   0x81000004,

    /**
     * The size provided to this method must be a multiple of the block size.
     */
    CRYPTO_FAILURE_INVALID_BLOCK_MULTIPLE                   =   0x81000005,

} crypto_status_t;

/**
 * Helper macro which returns true if the given crypto_status_t value indicates
 * failure.
 */
#define CRYPTO_FAILURE(x) (((x) & 0x80000000) != 0)

/**
 * Helper macro which returns true if the given crypto_status_t value indicates
 * success.
 */
#define CRYPTO_SUCCESS(x) (((x) & 0x80000000) == 0)

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_CRYPTO_CRYPTO_STATUS_HEADER_GUARD
