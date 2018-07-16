
/**
* certificate_definitions.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used by the certificate library.
*/

#ifndef CERTIFICATE_DEFINITIONS_H
#define CERTIFICATE_DEFINITIONS_H

#include <stdint.h>
#include "aef/cutils/guid.h"

# ifdef   __cplusplus
extern "C" {
# endif

#ifndef UUID_SIZE
#define UUID_SIZE       16
#endif

/**
* Certificate type GUID identifiers
*/
typedef GUID certificate_id_t;

extern const certificate_id_t CERT_TYPE_INVALID_GUID;
extern const certificate_id_t CERT_TYPE_ENTITY_GUID;
extern const certificate_id_t CERT_TYPE_PRIVATE_GUID;
extern const certificate_id_t CERT_TYPE_ASSIGNMENT_GUID;
extern const certificate_id_t CERT_TYPE_HARDWARE_GUID;

/**
* Certificate field types
*/
typedef enum certificate_field_type
{
    CERT_FIELD_TYPE_INVALID     	= 0x0000,
    CERT_FIELD_TYPE_CERT_ID     	= 0x0001,
    CERT_FIELD_TYPE_REALM_ID    	= 0x0002,
    CERT_FIELD_TYPE_ASSIGNER    	= 0x0003,
    CERT_FIELD_TYPE_ASSIGNEE    	= 0x0004,
    CERT_FIELD_TYPE_SIGNER      	= 0x0005,
    CERT_FIELD_TYPE_SIGNATURE   	= 0x0006,
    CERT_FIELD_TYPE_CRYPTO_TYPE 	= 0x0007,
	CERT_FIELD_TYPE_CODE_VERSION 	= 0x0008,
	CERT_FIELD_TYPE_SERIAL_NUMBER	= 0x0009,
	CERT_FIELD_TYPE_MAC_ADDR		= 0x000A,
	CERT_FIELD_TYPE_GATEWAY_ADDR	= 0x000B,
	CERT_FIELD_TYPE_DNS_ADDR		= 0x000C,
	CERT_FILED_TYPE_NET_MASK		= 0x000D,
	CERT_FIELD_TYPE_CA_CERT			= 0x000E,
	CERT_FIELD_TYPE_PRIVATE_KEY		= 0x000F,
	CERT_FIELD_TYPE_USER_DEFINED	= 0xFFF0,
} certificate_field_type_t;

/**
* Certificate field header structure definition
*/
struct certificate_field_hdr_def
{
    uint16_t    type;                           // Field type
    uint16_t    size;                           // Field size in bytes
} __attribute__((__packed__));
typedef struct certificate_field_hdr_def certificate_field_hdr_t;

/**
* Certificate field structure definition
*/
struct certificate_field_def
{
    certificate_field_hdr_t hdr;                // certificate field header
    uint8_t                 data[1];            // Field data
} __attribute__((__packed__)); 
typedef struct certificate_field_def certificate_field_t;

/**
* Certificate header structure definition
*/
struct certificate_hdr_def
{
    certificate_id_t    id;                     // Certificate id UUID
    uint16_t            size;                   // Certificate size in bytes
    uint16_t            numberOfFields;         // Number of fields 
} __attribute__((__packed__)); 
typedef struct certificate_hdr_def certificate_hdr_t;

/**
* Certificate header structure definition
*/
struct certificate_def
{
    certificate_hdr_t   hdr;                    // Certificate header
    certificate_field_t fields[1];              // Certificate fields
} __attribute__((__packed__)); 
typedef struct certificate_def certificate_t;

#define CERTIFICATE_FIELD_HEADER_SIZE   sizeof(certificate_field_hdr_t)
#define CERTIFICATE_HEADER_SIZE         sizeof(certificate_hdr_t)

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CERTIFICATE_DEFINITIONS_H
