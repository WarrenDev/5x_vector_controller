
/**
* database_service_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Logger service core definitions.
*
*/

#ifndef SRC_SERVICES_DATABASE_DATABASE_SERVICE_CORE_H_
#define SRC_SERVICES_DATABASE_DATABASE_SERVICE_CORE_H_

#include <aef/embedded/service/database/database_service.h>
#include <aef/embedded/service/service_interface.h>
#include <aef/embedded/service/service_ioctl.h>
#include <stddef.h>
#include <stdint.h>
#include "CoOS.h"
#include "string.h"

# ifdef   __cplusplus
extern "C" {
# endif

#define DATABASE_VERSION					1	// Database version
#define DATABASE_SUPERBLOCK_COUNT			8	// Number of Superblocks in the database
#define DATABASE_UUID_SIZE					16	// UUID size

#define INVALID_BLOCKREF					0xFFFF

//The record management bytes include all bytes up to and including the IV.
//There are only two bytes after the IV -- the encrypted bytes -- which must be
//part of the encrypted payload and the AES-CBC-MAC payload as per proper
//cryptographic use of these algorithms.  Therefore, this size should be
//sizeof(EntryHeader) - sizeof(EntryHeader::encBytes)
#define RECORD_MANAGEMENT_BYTES_SIZE (sizeof(EntryHeader_t) - sizeof(EntryHeader_t::encBytes))

//The PROTECTED region covers all of the record EXCEPT the footer and the first two bytes
#define HEADER_PROTECTED_SIZE ((sizeof(DatabaseHeader_t) - RECORD_MANAGEMENT_BYTES_SIZE) - sizeof(EntryFooter_t))
#define HEADER_PROTECTED_OFFSET RECORD_MANAGEMENT_BYTES_SIZE

//The PROTECTED region covers all of the session EXCEPT the footer and the first two bytes
#define SESSION_PROTECTED_SIZE ((sizeof(SessionHeader_t) - RECORD_MANAGEMENT_BYTES_SIZE) - sizeof(EntryFooter_t))
#define SESSION_PROTECTED_OFFSET RECORD_MANAGEMENT_BYTES_SIZE

//The PROTECTED region covers all of the eKey EXCEPT the footer and the first two bytes
#define EKEY_PROTECTED_SIZE ((sizeof(EKeyEntry_t) - RECORD_MANAGEMENT_BYTES_SIZE) - sizeof(EntryFooter_t))
#define EKEY_PROTECTED_OFFSET RECORD_MANAGEMENT_BYTES_SIZE

/**
 * Return codes for Device Database methods.
 */
enum DatabaseAPIStatus
{
    ///Success
    DATABASE_API_SUCCESS                    = 0x00000000,

    ///Generic status message.  All status messages have the 0x10 bit set.
    DATABASE_API_STATUS                     = 0x10000000,

    ///The status is successful, but a background operation is pending.
    DATABASE_API_STATUS_PENDING             = 0x10000001,

    ///General Error.  Not recoverable.
    DATABASE_API_ERROR                      = 0x80000000,

    ///No valid strategy for garbage collection
    DATABASE_API_ERROR_NO_GC_STRATEGY       = 0x80000001,

    ///Generic Recoverable Error.
    DATABASE_API_RECOVERABLE_ERROR          = 0xC0000000,

    ///The database is currently busy.
    DATABASE_API_ERROR_BUSY                 = 0xC0000001,

    ///The database is currently full.
    DATABASE_API_ERROR_FULL                 = 0xC0000002,

    ///The entry was not found in the database for update.
    DATABASE_API_ERROR_NOT_FOUND            = 0xC0000003,

    ///This is an invalid error.  Should never occur.
    DATABASE_API_ERROR_INVALID              = 0xFFFFFFFF
};

////////////////////////////////////////////////////////////
///Generic interrogation functions for error codes from the database API.
#define DATABASE_STATUS(x) (((x) & DATABASE_API_STATUS) == DATABASE_API_STATUS)
#define DATABASE_ERROR(x) (((x) & DATABASE_API_ERROR) == DATABASE_API_ERROR)
#define DATABASE_ERROR_RECOVERABLE(x) (((x) & DATABASE_API_RECOVERABLE_ERROR) == DATABASE_API_RECOVERABLE_ERROR)
#define DATABASE_ERROR_NOT_RECOVERABLE(x) !(((x) & DATABASE_API_RECOVERABLE_ERROR) == DATABASE_API_RECOVERABLE_ERROR)

typedef uint16_t	DBCURSOR;

/**
* Initialize the database service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_init (service_ctx_t* ctx);

/**
* De-initialize the database service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_deinit (service_ctx_t* ctx);

/**
* Send a command to a system service instance.
*
* \param    ctx					Pointer to the service context
* \param    code				I/O control code to perform
* \param    input_buffer		Pointer to the input buffer
* \param    input_size			Input buffer size
* \param    output_buffer		Pointer to the output buffer
* \param    output_size			Output buffer size
* \param    bytes_transferred	Pointer to the number of bytes read or written
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
service_status_t
database_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICES_DATABASE_DATABASE_SERVICE_CORE_H_ */
