
/**
* certificate_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementation of core certificate library routines.
*/

#include <aef/certificate/certificate_core.h>
#include <string.h>
#include <stdlib.h>
#include "bsp.h"

/**
* Internal routines
*   Routines used only within this code module
*/
static certificate_t*       allocate_certificate_memory   (uint16_t size);
static certificate_t*       reallocate_certificate_memory (certificate_t* certificate, uint16_t size);
static certificate_status_t destroy_certificate_memory    (certificate_t* certificate);
static certificate_field_t* search_certificate_fields     (certificate_t* certificate, certificate_field_type_t field_type);

/**
* cert_create_certificate
*   Create a new certificate by allocating memory for the certificate header.
*
* \param id         Certificate Id (UUID)
*
* \return           Handle to new certificate memory when successful.
*                   NULL if the create failed.
*/
certificate_t*       
cert_create_certificate(const certificate_id_t* id)
{
    certificate_t* certificate = allocate_certificate_memory(sizeof(certificate_hdr_t));

    if (certificate)
    {
        memcpy(&certificate->hdr.id, (const void*)id, sizeof(certificate_id_t));
    }

    return certificate;
}

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
certificate_t*
cert_create_certificate_using_size(const certificate_id_t* id, uint16_t size)
{
    certificate_t* certificate = allocate_certificate_memory(size);

    if (certificate)
    {
        memcpy(&certificate->hdr.id, id, sizeof(certificate_id_t));
    }

    return certificate;
}

/**
* cert_create_certificate_using_buffer
*   Construct a certificate in a pre-allocated buffer.
*
* \param id         Certificate Id (UUID)
* \param buffer     Pre-allocated buffer to use to construct the certificate.
* \param size       size of the pre-allocated buffer in bytes
* \param fields		Number of fields
*
* \return           Handle to new certificate memory when successful.
*                   NULL if the create failed.
*/
certificate_t*
cert_create_certificate_using_buffer(const certificate_id_t* id, void* buffer, uint16_t size, uint16_t fields)
{
    certificate_t* certificate = (certificate_t*)buffer;

    if (certificate)
    {
        memcpy(&certificate->hdr.id, id, sizeof(certificate_id_t));
        certificate->hdr.size = size;
        certificate->hdr.numberOfFields = fields;
    }

    return certificate;
}

/**
* cert_add_field
*   Add a new field to a certificate.  The certificate memory will be resized for the
*   new field, and the new field will be added to the end.
*
* \param certificate    Pointer to a certificate handle
* \param type           Field type to add
* \param value          Pointer to field data
* \param size           Size of field data in bytes
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*                       CERT_GENERAL_FAILURE if unable to add the new field
*                       
*/
certificate_status_t
cert_add_field(certificate_t** certificate, certificate_field_type_t type, const void* value, uint16_t size)
{
    uint16_t fieldSize = CERTIFICATE_FIELD_HEADER_SIZE + size;

    /**
    * Increase the memory footprint of the certificate to make room for the new field
    */
    certificate_t* new_cert_mem = reallocate_certificate_memory(*certificate, fieldSize);
    if (new_cert_mem == NULL)
        return CERT_GENERAL_FAILURE;

    /**
    * Due to reallocation, the certificate may have moved to a different memory location,
    * so update the certificate pointer
    */
    *certificate = new_cert_mem;

    /**
    * Load the new field data structure
    */
    certificate_field_t* field = (certificate_field_t*)((uint8_t*)(*certificate) + ((*certificate)->hdr.size - fieldSize));
    field->hdr.type = type;
    field->hdr.size = size;
    memcpy(field->data, value, size);

    /**
    * Update the certificate field count
    */
    (*certificate)->hdr.numberOfFields++;

    return CERT_STATUS_SUCCESS;
}

/**
* cert_write_field
*    Write field data to a certificate.  
*    The field must exist in the certificate, and the field size must be the same.
*
* \param certificate    Certificate handle
* \param type           Field type to write
* \param value          Pointer to field data to write
* \param size           Size of field data in bytes
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*                       CERT_FIELD_NOT_FOUND if the field is not in the certificate
*                       CERT_ERROR_INVALID_VALUE if the passed in size does not match
*                       the actual size of the field.
*/
certificate_status_t
cert_write_field(certificate_t* certificate, certificate_field_type_t type, const void* value, uint16_t size)
{
    certificate_field_t* field = search_certificate_fields(certificate, type);

    /**
    * Check for error conditions
    */
    if (field == NULL)
        return CERT_FIELD_NOT_FOUND;

    if (field->hdr.size != size)
        return CERT_ERROR_INVALID_VALUE;

    /**
    * Write field data
    */
    memcpy(field->data, value, size);

    return CERT_STATUS_SUCCESS;
}

/**
* cert_get_field
*    Retrieve a pointer to a field in a certificate.  
*
* \param certificate    Certificate handle
* \param type           Field type to search for
*
* \return               Pointer to field data if no errors occurred.
*                       NULL if the field was not found.
*/
certificate_field_t*
cert_get_field(certificate_t* certificate, certificate_field_type_t type)
{
    return search_certificate_fields(certificate, type);
}

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
*                       CERT_FIELD_NOT_FOUND if the field is not in the certificate
*/
certificate_status_t
cert_get_field_data(certificate_t* certificate, certificate_field_type_t type, void* value, uint16_t* size)
{
    certificate_field_t* field = search_certificate_fields(certificate, type);

    if (field)
    {
        *size = field->hdr.size;
        if ( value != NULL )
            memcpy(value, field->data, field->hdr.size);
        return CERT_STATUS_SUCCESS;
    }

    return CERT_FIELD_NOT_FOUND;
}

/**
* cert_destroy_certificate
*    Deallocate certificate memory.
*
* \param certificate    Certificate handle
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*
*/
certificate_status_t
cert_destroy_certificate(certificate_t* certificate)
{
    return destroy_certificate_memory (certificate);
}

/*** Internal routines ********************************************************/

/**
* allocate_Certificate_memory (internal)
*    Routine to allocate certificate memory.
*
* \param size           Certificate size to allocate.
*
* \return               Certificate handle if no errors occurred.
*                       NULL if an error occurred.
*
*/
static
certificate_t*
allocate_certificate_memory(uint16_t size)
{
    certificate_t* certificate = (certificate_t*) malloc (size);

    if (certificate)
    {
        memset(certificate, 0, size);
        certificate->hdr.size = size;
    }

    return (certificate);
}

/**
* reallocate_Certificate_memory (internal)
*    Routine to allocate certificate memory.
*
* \param certificate    Certificate handle
* \param size           Size to add to certificate memory
*
* \return               Certificate handle if no errors occurred.
*                       NULL if an error occurred.  If a NULL is returned,
*                       the original certificate exists and is unchanged.
*
*/
static
certificate_t*
reallocate_certificate_memory(certificate_t* certificate, uint16_t size)
{
    certificate_t* cert  = (certificate_t*) realloc (certificate, (certificate->hdr.size + size));

    if (cert)
    {
        uint8_t* newFieldPtr = ((uint8_t*)cert) + cert->hdr.size;
        memset(newFieldPtr, 0, size);
        cert->hdr.size += size;
    }

    return (cert);
}

/**
* destroy_Certificate_memory (internal)
*    Routine to deallocate certificate memory.
*
* \param certificate    Certificate handle
*
* \return               CERT_STATUS_SUCCESS if no errors occurred.
*                       CERT_GENERAL_FAILURE if an error occurred.
*
*/
static
certificate_status_t
destroy_certificate_memory( certificate_t* certificate )
{
    if (certificate)
    {
        free(certificate);
    }

    return CERT_STATUS_SUCCESS;
}

/**
* search_Certificate_fields (internal)
*    Routine to traverse the fields memory of the certificate looking for a specific field.
*
* \param certificate    Certificate handle
* \param field_type     Field type to locate
*
* \return               Pointer to the certificate_field_t structure for the field if no errors occurred.
*                       NULL if the field was not found.
*/
static
certificate_field_t*
search_certificate_fields( certificate_t* certificate, certificate_field_type_t field_type )
{
    if (certificate)
    {
        uint16_t count = certificate->hdr.numberOfFields;
        certificate_field_t* field = certificate->fields;

        while (count > 0)
        {
            if (field->hdr.type == field_type)
                return field;

            field = (certificate_field_t*)((uint8_t*)field + CERTIFICATE_FIELD_HEADER_SIZE + field->hdr.size);
            count--;
        }
    }

    return NULL;
}
