
/**
* certificate_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used by the certificate core implementation.
*/

#ifndef CERTIFICATE_CORE_H
#define CERTIFICATE_CORE_H

#include <aef/certificate/certificate_definitions.h>
#include <aef/certificate/certificate_status.h>
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
* cert_create_certificate
*   Create a new certificate by allocating memory for the certificate header.
*
* \param id         Certificate Id (UUID)
*
* \return           Handle to new certificate memory when successful.
*                   NULL if the create failed.
*/
certificate_t* cert_create_certificate(const certificate_id_t* id);

/**
* cert_create_certificate_using_size
*   Create a new certificate by allocating memory for the certificate header and body.
*
* \param id         Certificate Id (UUID)
* \param size       size of the certificate body in bytes to allocate
*
* \return           Handle to new certificate memory when successful.
*                   NULL if the create failed.
*/
certificate_t* cert_create_certificate_using_size(const certificate_id_t* id, uint16_t size);

/**
* cert_create_certificate_using_buffer
*   Construct a certificate in a pre-allocated buffer.
*
* \param id         Certificate Id (UUID)
* \param buffer     Pre-allocated buffer to use to construct the certificate.
* \param size       Size of the pre-allocated buffer in bytes
* \param fields		Number of fields
*
* \return           Handle to new certificate memory when successful.
*                   NULL if the create failed.
*/
certificate_t* cert_create_certificate_using_buffer(const certificate_id_t* id, void* buffer, uint16_t size, uint16_t fields);

/**
* cert_add_field
*   Add a new field to a certificate
*
* \param certificate    Pointer to a certificate handle
* \param type           Field type to add
* \param value          Pointer to field data
* \param size           Size of field data in bytes
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*
*/
certificate_status_t cert_add_field(certificate_t** certificate, certificate_field_type_t type, const void* value, uint16_t size);

/**
* cert_write_field
*    Write field data to a certificate.
*    The field must exist in the certificate, and the field size must be the same.
*
* \param certificate    Certificate handle
* \param type           Field type to write
* \param value          Pointer to field data
* \param size           Size of field data in bytes
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*
*/
certificate_status_t cert_write_field(certificate_t* certificate, certificate_field_type_t type, const void* value, uint16_t size);

/**
* cert_get_field
*    Retrieve a pointer to a field in a certificate.
*
* \param certificate    Certificate handle
* \param type           Field type to search for
*
* \return               Pointer to field data if no errors occurred.
*                       NULL if the field was not found.
*
*/
certificate_field_t* cert_get_field(certificate_t* certificate, certificate_field_type_t type);

/**
* cert_get_field_Data
*    Retrieve field data from a certificate.
*    The field must exist in the certificate, and the field size must be the same.
*
* \param certificate    Certificate handle
* \param type           Field type to search for
* \param value          Pointer to memory area to hold the field data
* \param size           Pointer to variable to hold the Size of field data in bytes
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*
*/
certificate_status_t cert_get_field_data(certificate_t* certificate, certificate_field_type_t type, void* value, uint16_t* size);

/**
* cert_destroy_certificate
*    Deallocate certificate memory.
*
* \param certificate    Certificate handle
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*
*/
certificate_status_t cert_destroy_certificate(certificate_t* certificate);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CERTIFICATE_CORE_H

