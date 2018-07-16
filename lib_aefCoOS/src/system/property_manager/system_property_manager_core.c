
/**
* system_property_manager_core.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Certificate-based system property Manager core definitions.
*
*/

#include "system_property_manager_core.h"
#include "bsp.h"

/**
* Hardware certificate definition
* We are defining a hardware certificate in system memory.
* The property manager will initialize the certificate with configuration values.
* This is a temporary measure.  It will be replaced with reading the hardware certificate from flash memory.
*/
struct hardware_certificate_def
{
	certificate_hdr_t cert_hdr;
	certificate_field_hdr_t mac_addr_hdr;
	uint8_t mac_addr[6];
	certificate_field_hdr_t serial_number_hdr;
	uint8_t serial_number[4];
} __attribute__((__packed__));
typedef struct hardware_certificate_def hardware_certificate_t;

static hardware_certificate_t hardware_cert =
{
		.mac_addr_hdr.type = CERT_FIELD_TYPE_MAC_ADDR,
		.mac_addr_hdr.size = 6,
		.serial_number_hdr.type = CERT_FIELD_TYPE_SERIAL_NUMBER,
		.serial_number_hdr.size = sizeof(uint32_t)
};
static certificate_t* cert;

static uint8_t mac_addr[6] = { 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x01 };
static uint32_t serial_number = 1000000000;

/**
* Initialize the property manager.
*
* \param    none
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to initialize the property manager
*/
system_status_t property_manager_core_init (void)
{
	cert = cert_create_certificate_using_buffer(&CERT_TYPE_HARDWARE_GUID, &hardware_cert, sizeof(hardware_certificate_t), 2);
	cert_write_field (cert, CERT_FIELD_TYPE_MAC_ADDR, mac_addr, 6);
	cert_write_field (cert, CERT_FIELD_TYPE_SERIAL_NUMBER, &serial_number, sizeof(uint32_t));

	return SYSTEM_STATUS_SUCCESS;
}

/**
* Get an existing property instance from the property manager.  This is a blocking call.
*
* \param    property_id			Property identifier (certificate field type)
* \param	length				Pointer to size of the property value in bytes
* \param 	value				Property value
*
* \returns SYSTEM_STATUS_SUCCESS on success.
*          SYSTEM_FAILURE_GENERAL if the property doesn't exist
*/
system_status_t property_manager_core_getproperty (certificate_field_type_t property_id, uint16_t* length, void* value)
{
	return cert_get_field_data ( cert, property_id, value, length );
}

/**
* Add a new property.  This is a blocking call.
*
* \param    property_id			Property identifier (certificate field type)
* \param	length				Size of the property value in bytes
* \param 	value				Property value
*
* \returns SYSTEM_STATUS_SUCCESS on success.
*          SYSTEM_FAILURE_GENERAL if unable to add the new property
*/
system_status_t property_manager_core_addproperty (certificate_field_type_t property_id, uint16_t length, const void* value)
{
	return cert_add_field ( &cert, property_id, value, length );
}

/**
* Update an existing property.
*
* \param    property_id			Property identifier (certificate field type)
* \param	length				Size of the property value in bytes
* \param 	value				Property value
*
* \returns SYSTEM_STATUS_SUCCESS on success.
*          SYSTEM_FAILURE_GENERAL if unable to update the property
*/
system_status_t property_manager_core_updateproperty (certificate_field_type_t property_id, uint16_t length, const void* value)
{
	return cert_write_field ( cert, property_id, value, length );
}

/**
* Delete an existing property.
*
* \param    property_id			Property identifier (certificate field type)
*
* \returns SERVICE_STATUS_SUCCESS on success.
*          SERVICE_FAILURE_GENERAL if unable to delete the property
*/
system_status_t property_manager_core_deleteproperty (certificate_field_type_t property_id)
{
	return SYSTEM_FAILURE_UNSUPPORTED_OPERATION;
}

/**
* Add a permission controller to the property manager.
*
* \param    Pointer to a permission controller
*
* \returns SERVICE_STATUS_SUCCESS on successful.
*          SERVICE_FAILURE_GENERAL if unable to add the new permission controller
*/
system_status_t property_manager_core_setpermission (const void* permission_controller)
{
	return SYSTEM_STATUS_SUCCESS;
}

