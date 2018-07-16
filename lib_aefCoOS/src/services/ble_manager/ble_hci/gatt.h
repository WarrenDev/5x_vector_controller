
/**
  @headerfile:       gatt.h
  $Date: 2009-06-29 16:20:52 -0700 (Mon, 29 Jun 2009) $
  $Revision: 20240 $

  @mainpage BLE GATT API

  Description:    This file contains Generic Attribute Profile (GATT)
                  definitions and prototypes.<BR><BR>

  \image html HighLevelGATT.PNG

  \htmlinclude GATTDesc.html

  Copyright 2009-2011 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*/

#ifndef GATT_H
#define GATT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "att.h"

/*********************************************************************
 * CONSTANTS
 */

/** @defgroup GATT_PERMIT_BITMAPS_DEFINES GATT Attribute Access Permissions Bit Fields
 * @{
 */
#define GATT_PERMIT_READ                 0x01 //!< Attribute is Readable
#define GATT_PERMIT_WRITE                0x02 //!< Attribute is Writable
#define GATT_PERMIT_AUTHEN_READ          0x04 //!< Read requires Authentication
#define GATT_PERMIT_AUTHEN_WRITE         0x08 //!< Write requires Authentication
#define GATT_PERMIT_AUTHOR_READ          0x10 //!< Read requires Authorization
#define GATT_PERMIT_AUTHOR_WRITE         0x20 //!< Write requires Authorization

/** @} End GATT_PERMIT_BITMAPS_DEFINES */


/** @defgroup GATT_NUM_PREPARE_WRITES_DEFINES GATT Maximum Number of Prepare Writes
 * @{
 */

#if !defined( GATT_MAX_NUM_PREPARE_WRITES )
  #define GATT_MAX_NUM_PREPARE_WRITES      5 //!< GATT Maximum number of attributes that Attribute Server can prepare for writing per Attribute Client
#endif

/** @} End GATT_NUM_PREPARE_WRITES_DEFINES */


/** @defgroup GATT_ENCRYPT_KEY_SIZE_DEFINES GATT Encryption Key Size
 * @{
 */

#define GATT_ENCRYPT_KEY_SIZE            16 //!< GATT Encryption Key Size used for encrypting a link

/** @} End GATT_ENCRYPT_KEY_SIZE_DEFINES */


/** @defgroup GATT_MAX_ATTR_SIZE_DEFINES GATT Maximum Attribute Value Length
 * @{
 */

#define GATT_MAX_ATTR_SIZE               512 //!< GATT Maximum length of an attribute value

/** @} End GATT_MAX_ATTR_SIZE_DEFINES */

// GATT Maximum number of connections (including loopback)
#define GATT_MAX_NUM_CONN                ( MAX_NUM_LL_CONN + 1 )

 // GATT Base Method
#define GATT_BASE_METHOD                 0x40

// Attribute handle defintions
#define GATT_INVALID_HANDLE              0x0000 // Invalid attribute handle
#define GATT_MIN_HANDLE                  0x0001 // Minimum attribute handle
#define GATT_MAX_HANDLE                  0xFFFF // Maximum attribute handle

/*********************************************************************
 * VARIABLES
 */

/*********************************************************************
 * MACROS
 */

// Attribute Access Permissions
#define gattPermitRead( a )              ( (a) & GATT_PERMIT_READ )
#define gattPermitWrite( a )             ( (a) & GATT_PERMIT_WRITE )
#define gattPermitAuthenRead( a )        ( (a) & GATT_PERMIT_AUTHEN_READ )
#define gattPermitAuthenWrite( a )       ( (a) & GATT_PERMIT_AUTHEN_WRITE )
#define gattPermitAuthorRead( a )        ( (a) & GATT_PERMIT_AUTHOR_READ )
#define gattPermitAuthorWrite( a )       ( (a) & GATT_PERMIT_AUTHOR_WRITE )

// Check for different UUID types
#define gattPrimaryServiceType( t )      ( ATT_CompareUUID( primaryServiceUUID, ATT_BT_UUID_SIZE, \
                                                            (t).uuid, (t).len ) )
#define gattSecondaryServiceType( t )    ( ATT_CompareUUID( secondaryServiceUUID, ATT_BT_UUID_SIZE, \
                                                            (t).uuid, (t).len ) )
#define gattCharacterType( t )           ( ATT_CompareUUID( characterUUID, ATT_BT_UUID_SIZE, \
                                                            (t).uuid, (t).len ) )
#define gattIncludeType( t )             ( ATT_CompareUUID( includeUUID, ATT_BT_UUID_SIZE, \
                                                            (t).uuid, (t).len ) )
#define gattServiceType( t )             ( gattPrimaryServiceType( (t) ) || \
                                           gattSecondaryServiceType( (t) ) )

/*********************************************************************
 * TYPEDEFS
 */

/**
 * GATT Read By Type Request format.
 */
struct _gattReadByTypeReq_def
{
  uint8_t discCharsByUUID;  //!< Whether this is a GATT Discover Characteristics by UUID sub-procedure
  attReadByTypeReq_t req; //!< Read By Type Request
} __attribute__((__packed__));
typedef struct _gattReadByTypeReq_def gattReadByTypeReq_t;

/**
 * GATT Prepare Write Request format.
 */
struct _gattPrepareWriteReq_def
{
  uint16_t handle; //!< Handle of the attribute to be written (must be first field)
  uint16_t offset; //!< Offset of the first octet to be written
  uint8_t len;     //!< Length of value
  uint8_t *pValue; //!< Part of the value of the attribute to be written (must be allocated)
} __attribute__((__packed__));
typedef struct _gattPrepareWriteReq_def gattPrepareWriteReq_t;

/**
 * GATT Write Long Request format. Do not change the order of the members.
 */
struct _gattWriteLongReq_def
{
  uint8_t reliable;            //!< Whether reliable writes requested (always FALSE for Write Long)
  gattPrepareWriteReq_t req; //!< GATT Prepare Write Request
  uint16_t lastOffset;         //!< Offset of last Prepare Write Request sent
} __attribute__((__packed__));
typedef struct _gattWriteLongReq_def gattWriteLongReq_t;

/**
 * GATT Reliable Writes Request format. Do not change the order of the members.
 */
struct _gattReliableWritesReq_def
{
  uint8_t reliable;              //!< Whether reliable writes requested (always TRUE for Reliable Writes)
  attPrepareWriteReq_t *pReqs; //!< Arrary of Prepare Write Requests (must be allocated)
  uint8_t numReqs;               //!< Number of Prepare Write Requests
  uint8_t index;                 //!< Index of last Prepare Write Request sent
  uint8_t flags;                 //!< 0x00 - cancel all prepared writes.
                               //!< 0x01 - immediately write all pending prepared values.
} __attribute__((__packed__));
typedef struct _gattReliableWritesReq_def gattReliableWritesReq_t;

/**
 * GATT Message format. It's a union of all attribute protocol/profile messages
 * used between the attribute protocol/profile and upper layer application.
 */
union _gattMsg_def
{
  // Request messages
  attExchangeMTUReq_t exchangeMTUReq;              //!< ATT Exchange MTU Request
  attFindInfoReq_t findInfoReq;                    //!< ATT Find Information Request
  attFindByTypeValueReq_t findByTypeValueReq;      //!< ATT Find By Type Vaue Request
  attReadByTypeReq_t readByTypeReq;                //!< ATT Read By Type Request
  attReadReq_t readReq;                            //!< ATT Read Request
  attReadBlobReq_t readBlobReq;                    //!< ATT Read Blob Request
  attReadMultiReq_t readMultiReq;                  //!< ATT Read Multiple Request
  attReadByGrpTypeReq_t readByGrpTypeReq;          //!< ATT Read By Group Type Request
  attWriteReq_t writeReq;                          //!< ATT Write Request
  attPrepareWriteReq_t prepareWriteReq;            //!< ATT Prepare Write Request
  attExecuteWriteReq_t executeWriteReq;            //!< ATT Execute Write Request
  gattReadByTypeReq_t gattReadByTypeReq;           //!< GATT Read By Type Request
  gattWriteLongReq_t gattWriteLongReq;             //!< GATT Long Write Request
  gattReliableWritesReq_t gattReliableWritesReq;   //!< GATT Reliable Writes Request

  // Response messages
  attErrorRsp_t errorRsp;                          //!< ATT Error Response
  attExchangeMTURsp_t exchangeMTURsp;              //!< ATT Exchange MTU Response
  attFindInfoRsp_t findInfoRsp;                    //!< ATT Find Information Response
  attFindByTypeValueRsp_t findByTypeValueRsp;      //!< ATT Find By Type Vaue Response
  attReadByTypeRsp_t readByTypeRsp;                //!< ATT Read By Type Response
  attReadRsp_t readRsp;                            //!< ATT Read Response
  attReadBlobRsp_t readBlobRsp;                    //!< ATT Read Blob Response
  attReadMultiRsp_t readMultiRsp;                  //!< ATT Read Multiple Response
  attReadByGrpTypeRsp_t readByGrpTypeRsp;          //!< ATT Read By Group Type Response
  attPrepareWriteRsp_t prepareWriteRsp;            //!< ATT Prepare Write Response

  // Indication and Notification messages
  attHandleValueNoti_t handleValueNoti;            //!< ATT Handle Value Notification
  attHandleValueInd_t handleValueInd;              //!< ATT Handle Value Indication
} __attribute__((__packed__));
typedef union _gattMsg_def gattMsg_t;

/**
 * GATT OSAL GATT_MSG_EVENT message format. This message is used to forward an
 * incoming attribute protocol/profile message up to upper layer application.
 */
struct _gattMsgEvent_def
{
//  _event_hdr_t hdr; //!< GATT_MSG_EVENT and status
  uint16_t connHandle;    //!< Connection message was received on
  uint8_t method;         //!< Type of message
  gattMsg_t msg;        //!< Attribute protocol/profile message
} __attribute__((__packed__));
typedef struct _gattMsgEvent_def gattMsgEvent_t;

/**
 * GATT Attribute Type format.
 */
struct _gattAttrType_def
{
  uint8_t len;         //!< Length of UUID
  const uint8_t *uuid; //!< Pointer to UUID
} __attribute__((__packed__));
typedef struct _gattAttrType_def gattAttrType_t;

/**
 * GATT Attribute format.
 */
struct _attAttribute_def
{
  gattAttrType_t type; 		//!< Attribute type (2 or 16 octet UUIDs)
  uint8_t permissions;   	//!< Attribute permissions
  uint16_t handle;       	//!< Attribute handle - assigned internally by attribute server
  uint8_t* const pValue; 	//!< Attribute value - encoding of the octet array is defined in
                       	   //!< the applicable profile. The maximum length of an attribute
                       	   //!< value shall be 512 octets.
} __attribute__((__packed__));
typedef struct _attAttribute_def gattAttribute_t;

/**
 * GATT Service format.
 */
struct _gattService_def
{
  uint16_t numAttrs; //!< Number of attributes in attrs

  /** Array of attribute records.
   *  NOTE: The list must start with a Service attribute followed by
   *        all attributes associated with this Service attribute.
   */
  gattAttribute_t *attrs;
} __attribute__((__packed__));
typedef struct _gattService_def gattService_t;

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GATT_H */
