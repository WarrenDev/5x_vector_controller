
/**
* gatt_server.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  GATT Server implementation.
*
*/

#include "gatt_server.h"
#include "bsp.h"

static uint16_t					m_numAttrs;
static gattAttribute_t*			m_pAttrTable;
static pfnGATTReadAttrCB_t		m_pfnReadAttrCB;
static pfnGATTWriteAttrCB_t		m_pfnWriteAttrCB;
static pfnGATTAuthorizeAttrCB_t	m_pfnAuthorizeAttrCB;

static gattAttribute_t*	find_attr_using_handle ( uint16_t handle );

/**
* gatt_server_initialize
*/
void gatt_server_initialize ( void )
{
	m_numAttrs = 0;
	m_pAttrTable = NULL;
	m_pfnReadAttrCB = NULL;
	m_pfnWriteAttrCB = NULL;
	m_pfnAuthorizeAttrCB = NULL;
} // gatt_server_Initialize

/**
* gatt_server_deinitialize
*/
void gatt_server_deinitialize ( void )
{
	if ( m_pAttrTable )
		free ( m_pAttrTable );
}

uint16_t gatt_server_get_attr_count	( void )
{
	return m_numAttrs;
}

/**
* @brief   Register a service's attribute list and callback functions with
*          the GATT Server.
*
* @param   pService - Pointer to a GATT service definition
* @param   pServiceCBs - Service callback function pointers
*
* @return  SUCCESS: Service registered successfully.<BR>
*          INVALIDPARAMETER: Invalid service field.<BR>
*          FAILURE: Not enough attribute handles available.<BR>
*          bleMemAllocError: Memory allocation error occurred.<BR>
*/
bStatus_t gatt_server_register_service ( gattService_t* pService, const gattServiceCBs_t *pServiceCBs )
{

	////////////////////////////////////////////////////////
	// Set up service callback functions
	if ( pServiceCBs )
	{
		m_pfnReadAttrCB 	 = pServiceCBs->pfnReadAttrCB;
		m_pfnWriteAttrCB 	 = pServiceCBs->pfnWriteAttrCB;
		m_pfnAuthorizeAttrCB = pServiceCBs->pfnAuthorizeAttrCB;
	}

	////////////////////////////////////////////////////////
	// Create the attribute table
	m_numAttrs   = pService->numAttrs;
	m_pAttrTable = (gattAttribute_t*) malloc ( sizeof(gattAttribute_t) * m_numAttrs );
	if ( !m_pAttrTable )
		return bleMemAllocError;

	////////////////////////////////////////////////////////
	// Load the attribute table
	memcpy ( m_pAttrTable, pService->attrs, (sizeof(gattAttribute_t) * m_numAttrs) );

	////////////////////////////////////////////////////////
	// Assign handle values to attributes
	uint16_t handle = 0x0001;
	for ( gattAttribute_t* pAttribute = m_pAttrTable; handle <= m_numAttrs; pAttribute++, handle++ )
	{
		pAttribute->handle = handle;
	}

	return 0;

} // gatt_server_register_service

/**
* @brief   Read attribute value
*
* @param   connHandle		Connection handle
* @param   handle			Attribute handle
* @param   pValue			Pointer to memory to store value
* @param   pLength			Length of value
* @param   offset			Offset
*
* @return  SUCCESS: Service registered successfully.<BR>
*          INVALIDPARAMETER: Invalid service field.<BR>
*          FAILURE: Not enough attribute handles available.<BR>
*          bleMemAllocError: Memory allocation error occurred.<BR>
*/
bStatus_t gatt_server_read_attribute ( uint16_t connHandle, uint16_t handle, uint8_t* pValue, uint8_t* pLength, uint16_t offset )
{
bStatus_t status = ATT_ERR_UNLIKELY;

	////////////////////////////////////////////////////////
	// Locate the attribute in the table
	gattAttribute_t* pAttribute = find_attr_using_handle ( handle );

	////////////////////////////////////////////////////////
	// If we found the attribute, call the profile attribute read callback routine
	if ( pAttribute && m_pfnReadAttrCB )
	{
		status = (*m_pfnReadAttrCB)( connHandle, pAttribute, pValue, pLength, offset, ATT_MTU_SIZE-1 );
	}

	return ( (pAttribute == NULL) ? ATT_ERR_INVALID_HANDLE : status );

} // gatt_server_readAttribute

/**
* @brief   Write attribute value
*
* @param   connHandle		Connection handle
* @param   handle			Attribute handle
* @param   pValue			Pointer to value to write
* @param   pLength			Length of value
* @param   offset			Offset
*
* @return  SUCCESS: Service registered successfully.<BR>
*          INVALIDPARAMETER: Invalid service field.<BR>
*          FAILURE: Not enough attribute handles available.<BR>
*          bleMemAllocError: Memory allocation error occurred.<BR>
*/
bStatus_t gatt_server_write_attribute ( uint16_t connHandle, uint16_t handle, uint8_t* pValue, uint8_t length, uint16_t offset )
{
bStatus_t status = ATT_ERR_UNLIKELY;

	////////////////////////////////////////////////////////
	// Locate the attribute in the table
	gattAttribute_t* pAttribute = find_attr_using_handle ( handle );

	////////////////////////////////////////////////////////
	// If we found the attribute, call the profile attribute write callback routine
	if ( pAttribute && m_pfnWriteAttrCB )
	{
		status = (*m_pfnWriteAttrCB)( connHandle, pAttribute, pValue, length, offset );
	}

	return ( (pAttribute == NULL) ? ATT_ERR_INVALID_HANDLE : status );

} // gatt_server_writeAttribute

/**
* @brief   Find attribute using type
*
* @param   type				Attribute type
* @param   pValue			Pointer to the value
*
* @return  Pointer to a gattAttribute_t structure if successful or NULL
*/
gattAttribute_t* gatt_server_find_attr_by_type ( uint16_t type, uint8_t* pValue )
{

	uint16_t index = 0;
	for ( gattAttribute_t* pAttribute = m_pAttrTable; index < m_numAttrs; pAttribute++, index++ )
	{
        if ( *((uint16_t*)pAttribute->type.uuid) == type )
			if ( memcmp (pAttribute->pValue, pValue, pAttribute->type.len ) == 0 )
				return pAttribute;
	}

	return NULL;

}

/**
* @brief   Get attribute descriptor list using type
*
* @param   type				Characteristic type
* @param   startHandle		Starting handle value
* @param   pCharDesc		Characteristic descriptor
* @param   maxLength		Maximum length
* @param   pSize			Pointer to size
*
* @return  SUCCESS: Service registered successfully.<BR>
*          INVALIDPARAMETER: Invalid service field.<BR>
*          FAILURE: Not enough attribute handles available.<BR>
*          bleMemAllocError: Memory allocation error occurred.<BR>
*/
gattAttribute_t* gatt_server_get_char_desc_list_by_type ( uint16_t type, uint16_t startHandle, attCharDesc_t* pCharDesc, uint8_t maxLength, uint8_t* pSize )
{
	gattAttribute_t* pAttribute = find_attr_using_handle ( startHandle );

	////////////////////////////////////////////////////////
	// Found the primary service attribute
	if ( pAttribute )
	{
		gattAttribute_t* pAttr = pAttribute;
		uint16_t count = 0;

		////////////////////////////////////////////////////
		// Move to the next attribute in the service and populate
		// the characteristic list with characteristics in the service.
		pAttribute++;
		while ( (count < maxLength) && (*((uint16_t*)pAttribute->type.uuid) != GATT_PRIMARY_SERVICE_UUID) )
		{
			if ( *((uint16_t*)pAttribute->type.uuid) == type )
			{
				pAttribute++;			// Move to the attribute defining the characteristic

				pCharDesc->type       = type;
				pCharDesc->uuid       = *((uint16_t*)pAttribute->type.uuid);
				pCharDesc->properties = pAttribute->permissions;
				pCharDesc->handle     = pAttribute->handle;

				pCharDesc++;			// Move to the next item in the list
				count++;				// Increment list entry count
			}
			pAttribute++;
		}

		if ( pSize )
			*pSize = count;

		return pAttr;

	}

	return NULL;

}

/**
* @brief   Retrieve the attribute count
*
* @param   None
*
* @return  Number of attributes in the table
*/
gattAttribute_t* find_attr_using_handle ( uint16_t handle )
{

	uint16_t index = 0;
	for ( gattAttribute_t* pAttribute = m_pAttrTable; index < m_numAttrs; pAttribute++, index++ )
	{
		if ( pAttribute->handle == handle )
			return pAttribute;
	}

	return NULL;

} // find_attr_using_handle

