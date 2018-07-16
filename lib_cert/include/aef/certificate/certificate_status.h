
/**
* certificate_status.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Certificate status definitions used by the certificate library.
*/

#ifndef CERTIFICATE_STATUS_H
#define CERTIFICATE_STATUS_H

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif


/**
* Certificate status codes.
*/
typedef enum certificate_status
{

    /**
    * The operation completed successfully.
    */
    CERT_STATUS_SUCCESS         = 0x00000000,

    /******** FAILURE CODES ********/

    /**
    * A general failure.
    */
    CERT_GENERAL_FAILURE        = 0x80000000,

    /**
    * Field not found
    */
    CERT_FIELD_NOT_FOUND        = 0x80000001,

    /**
    * Attempt to write to a read-oly certificate
    */
    CERT_ERROR_READONLY         = 0x80000002,

    /**
    * Attempt to write pass the end of the certificate
    */
    CERT_ERROR_WOULD_OVERRUN    = 0x80000003,

    /**
    * Attempt to read past the end of the certificate
    */
    CERT_ERROR_WOULD_TRUNCATE   = 0x80000004,

    /**
    * Invalid field value
    */
    CERT_ERROR_INVALID_VALUE    = 0x80000005,

    /**
    * The highest error number reported
    */
    CERT_ERROR_MAX              = 0x80000010

} certificate_status_t;

/**
* Helper macro which returns true if the given certificate_status_t value indicates
* failure.
*/
#define CERTIFICATE_FAILURE(x) (((x) & CERT_GENERAL_FAILURE) != 0)

/**
* Helper macro which returns true if the given certificate_status_t value indicates
* success.
*/
#define CERTIFICATE_SUCCESS(x) (((x) & CERT_GENERAL_FAILURE) == 0)

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CERTIFICATE_STATUS_H
