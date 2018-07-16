
/**
* gatt_server.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  GATT Server definitions.
*
*/

#ifndef SRC_SERVICES_BLE_MANAGER_BLE_HCI_GATT_SERVER_H_
#define SRC_SERVICES_BLE_MANAGER_BLE_HCI_GATT_SERVER_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "gatt.h"
#include "gatt_uuid.h"

/*********************************************************************
 * CONSTANTS
 */

/** @defgroup GATT_SERV_MSG_EVENT_DEFINES GATT Server Message IDs
 * @{
 */

#define GATT_CLIENT_CHAR_CFG_UPDATED_EVENT  0x00 //!< Sent when a Client Characteristic Configuration is updated.  This event is sent as an OSAL message defined as gattCharCfgUpdatedEvent_t.

/** @} End GATT_SERV_MSG_EVENT_DEFINES */


/** @defgroup GATT_PROP_BITMAPS_DEFINES GATT Characteristic Properties Bit Fields
 * @{
 */

#define GATT_PROP_BCAST                  0x01 //!< Permits broadcasts of the Characteristic Value
#define GATT_PROP_READ                   0x02 //!< Permits reads of the Characteristic Value
#define GATT_PROP_WRITE_NO_RSP           0x04 //!< Permits writes of the Characteristic Value without response
#define GATT_PROP_WRITE                  0x08 //!< Permits writes of the Characteristic Value with response
#define GATT_PROP_NOTIFY                 0x10 //!< Permits notifications of a Characteristic Value without acknowledgement
#define GATT_PROP_INDICATE               0x20 //!< Permits indications of a Characteristic Value with acknowledgement
#define GATT_PROP_AUTHEN                 0x40 //!< Permits signed writes to the Characteristic Value
#define GATT_PROP_EXTENDED               0x80 //!< Additional characteristic properties are defined in the Characteristic Extended Properties Descriptor

/** @} End GATT_PROP_BITMAPS_DEFINES */

/** @defgroup GATT_EXT_PROP_BITMAPS_DEFINES GATT Characteristic Extended Properties Bit Fields
 * @{
 */

#define GATT_EXT_PROP_RELIABLE_WRITE     0x0001 //!< Permits reliable writes of the Characteristic Value
#define GATT_EXT_PROP_WRITABLE_AUX       0x0002 //!< Permits writes to the characteristic descriptor

/** @} End GATT_EXT_PROP_BITMAPS_DEFINES */

/** @defgroup GATT_CLIENT_CFG_BITMAPS_DEFINES GATT Client Characteristic Configuration Bit Fields
 * @{
 */

#define GATT_CLIENT_CFG_NOTIFY           0x0001 //!< The Characteristic Value shall be notified
#define GATT_CLIENT_CFG_INDICATE         0x0002 //!< The Characteristic Value shall be indicated

/** @} End GATT_CLIENT_CFG_BITMAPS_DEFINES */

/** @defgroup GATT_SERV_CFG_BITMAPS_DEFINES GATT Server Characteristic Configuration Bit Fields
 * @{
 */

#define GATT_SERV_CFG_BCAST              0x0001 //!< The Characteristic Value shall be broadcast when the server is in the broadcast procedure if advertising data resources are available

/** @} End GATT_SERV_CFG_BITMAPS_DEFINES */

#define GATT_CFG_NO_OPERATION            0x0000 // No operation

/** @defgroup GATT_FORMAT_TYPES_DEFINES GATT Characteristic Format Types
 * @{
 */

#define GATT_FORMAT_BOOL                 0x01 //!< Unsigned 1 bit; 0 = false, 1 = true
#define GATT_FORMAT_2BIT                 0x02 //!< Unsigned 2 bit integer
#define GATT_FORMAT_NIBBLE               0x03 //!< Unsigned 4 bit integer
#define GATT_FORMAT_UINT8                0x04 //!< Unsigned 8 bit integer
#define GATT_FORMAT_UINT12               0x05 //!< Unsigned 12 bit integer
#define GATT_FORMAT_UINT16               0x06 //!< Unsigned 16 bit integer
#define GATT_FORMAT_UINT24               0x07 //!< Unsigned 24 bit integer
#define GATT_FORMAT_UINT32               0x08 //!< Unsigned 32 bit integer
#define GATT_FORMAT_UINT48               0x09 //!< Unsigned 48 bit integer
#define GATT_FORMAT_UINT64               0x0a //!< Unsigned 64 bit integer
#define GATT_FORMAT_UINT128              0x0b //!< Unsigned 128 bit integer
#define GATT_FORMAT_SINT8                0x0c //!< Signed 8 bit integer
#define GATT_FORMAT_SINT12               0x0d //!< Signed 12 bit integer
#define GATT_FORMAT_SINT16               0x0e //!< Signed 16 bit integer
#define GATT_FORMAT_SINT24               0x0f //!< Signed 24 bit integer
#define GATT_FORMAT_SINT32               0x10 //!< Signed 32 bit integer
#define GATT_FORMAT_SINT48               0x11 //!< Signed 48 bit integer
#define GATT_FORMAT_SINT64               0x12 //!< Signed 64 bit integer
#define GATT_FORMAT_SINT128              0x13 //!< Signed 128 bit integer
#define GATT_FORMAT_FLOAT32              0x14 //!< IEEE-754 32 bit floating point
#define GATT_FORMAT_FLOAT64              0x15 //!< IEEE-754 64 bit floating point
#define GATT_FORMAT_SFLOAT               0x16 //!< IEEE-11073 16 bit SFLOAT
#define GATT_FORMAT_FLOAT                0x17 //!< IEEE-11073 32 bit FLOAT
#define GATT_FORMAT_DUINT16              0x18 //!< IEEE-20601 format
#define GATT_FORMAT_UTF8S                0x19 //!< UTF-8 string
#define GATT_FORMAT_UTF16S               0x1a //!< UTF-16 string
#define GATT_FORMAT_STRUCT               0x1b //!< Opaque structure

/** @} End GATT_FORMAT_TYPES_DEFINES */

/** @defgroup GATT_NS_TYPES_DEFINES GATT Namespace Types
 * @{
 */

#define GATT_NS_NONE                     0x00 //!< No namespace
#define GATT_NS_BT_SIG                   0x01 //!< Bluetooth SIG namespace

/** @} End GATT_NS_TYPES_DEFINES */

/** @defgroup GATT_NS_BT_DESC_DEFINES GATT Bluetooth Namespace Descriptions
 * @{
 */

#define GATT_NS_BT_DESC_UNKNOWN          0x0000 //!< The description is unknown

/** @} End GATT_NS_BT_DESC_DEFINES */

// All profile services bit fields
#define GATT_ALL_SERVICES                0xFFFFFFFF

// GATT Services bit fields
#define GATT_SERVICE                     0x00000001

#if defined ( TESTMODES )
  // GATT Test Modes
  #define GATT_TESTMODE_OFF              0 // Test mode off
  #define GATT_TESTMODE_NO_RSP           1 // Ignore incoming request
  #define GATT_TESTMODE_PREPARE_WRITE    2 // Forward Prepare Write Request right away
  #define GATT_TESTMODE_MAX_MTU_SIZE     3 // Use Max ATT MTU size with Exchange MTU Rsp
  #define GATT_TESTMODE_CORRUPT_PW_DATA  4 // Corrupt incoming Prepare Write Request data
#endif

// GATT Server Parameters
#define GATT_PARAM_NUM_PREPARE_WRITES    0 // RW  uint8

/*********************************************************************
 * VARIABLES
 */

/*********************************************************************
 * MACROS
 */

// The number of attribute records in a given attribute table
#define GATT_NUM_ATTRS( attrs )          ( sizeof( attrs ) / sizeof( gattAttribute_t ) )

// The handle of a service is the handle of the first attribute
#define GATT_SERVICE_HANDLE( attrs )     ( (attrs)[0].handle )

// The handle of the first included service (i = 1) is the value of the second attribute
#define GATT_INCLUDED_HANDLE( attrs, i ) ( *((uint16 *)((attrs)[(i)].pValue)) )

/*********************************************************************
 * TYPEDEFS
 */

/**
 * @defgroup GATT_SERV_APP_CB_API GATT Server App Callback API Functions
 *
 * @{
 */

/**
 * @brief   Callback function prototype to read an attribute value.
 *
 * @param   connHandle - connection request was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be read (to be returned)
 * @param   pLen - length of data (to be returned)
 * @param   offset - offset of the first octet to be read
 * @param   maxLen - maximum length of data to be read
 *
 * @return  SUCCESS: Read was successfully.<BR>
 *          Error, otherwise: ref ATT_ERR_CODE_DEFINES.<BR>
 */
typedef bStatus_t (*pfnGATTReadAttrCB_t)( uint16_t connHandle, gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint8_t *pLen, uint16_t offset,
                                          uint8_t maxLen );

/**
 * @brief   Callback function prototype to write an attribute value.
 *
 * @param   connHandle - connection request was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   pLen - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  SUCCESS: Write was successfully.<BR>
 *          Error, otherwise: ref ATT_ERR_CODE_DEFINES.<BR>
 */
typedef bStatus_t (*pfnGATTWriteAttrCB_t)( uint16_t connHandle, gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint8_t len, uint16_t offset );

/**
 * @brief   Callback function prototype to authorize a Read or Write operation
 *          on a given attribute.
 *
 * @param   connHandle - connection request was received on
 * @param   pAttr - pointer to attribute
 * @param   opcode - request opcode (ATT_READ_REQ or ATT_WRITE_REQ)
 *
 * @return  SUCCESS: Operation authorized.<BR>
 *          ATT_ERR_INSUFFICIENT_AUTHOR: Authorization required.<BR>
 */
typedef bStatus_t (*pfnGATTAuthorizeAttrCB_t)( uint16_t connHandle, gattAttribute_t *pAttr,
                                               uint8_t opcode );
/**
 * @}
 */

/**
 * GATT Structure for Characteristic Presentation Format Value.
 */
typedef struct
{
  uint8_t format;    //!< Format of the value of this characteristic
  uint8_t exponent;  //!< A sign integer which represents the exponent of an integer
  uint16_t unit;     //!< Unit of this attribute as defined in the data dictionary
  uint8_t nameSpace; //!< Name space of the description
  uint16_t desc;     //!< Description of this attribute as defined in a higher layer profile
} gattCharFormat_t;

/**
 * GATT Structure for Client Characteristic Configuration.
 */
typedef struct
{
  uint16_t connHandle; //!< Client connection handle
  uint8_t  value;      //!< Characteristic configuration value for this client
} gattCharCfg_t;

/**
 * GATT Structure for service callback functions - must be setup by the application
 * and used when RegisterService() is called.
 */
typedef struct
{
  pfnGATTReadAttrCB_t pfnReadAttrCB;           //!< Read callback function pointer
  pfnGATTWriteAttrCB_t pfnWriteAttrCB;         //!< Write callback function pointer
  pfnGATTAuthorizeAttrCB_t pfnAuthorizeAttrCB; //!< Authorization callback function pointer
} gattServiceCBs_t;

void gatt_server_initialize ( void );
void gatt_server_deinitialize ( void );

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
bStatus_t gatt_server_register_service ( gattService_t* pService,  const gattServiceCBs_t *pServiceCBs );

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
bStatus_t gatt_server_read_attribute ( uint16_t connHandle, uint16_t handle, uint8_t* pValue, uint8_t* pLength, uint16_t offset );

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
bStatus_t gatt_server_write_attribute ( uint16_t connHandle, uint16_t handle, uint8_t* pValue, uint8_t length, uint16_t offset );

/**
* @brief   Find attribute using type
*
* @param   type				Attribute type
* @param   pValue			Pointer to the value
*
* @return  Pointer to a gattAttribute_t structure if successful or NULL
*/
gattAttribute_t* gatt_server_find_attr_by_type ( uint16_t type, uint8_t* pValue );

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
gattAttribute_t* gatt_server_get_char_desc_list_by_type ( uint16_t type, uint16_t startHandle, attCharDesc_t* pCharDesc, uint8_t maxLength, uint8_t* pSize );

/**
* @brief   Retrieve the attribute count
*
* @param   None
*
* @return  Number of attributes in the table
*/
uint16_t gatt_server_get_attr_count ( void );

#endif /* SRC_SERVICES_BLE_MANAGER_BLE_HCI_GATT_SERVER_H_ */
