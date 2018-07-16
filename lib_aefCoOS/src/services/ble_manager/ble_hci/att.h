
/**
  @headerfile:    att.h

  <!--
  Revised:        $Date: 2013-05-01 13:58:23 -0700 (Wed, 01 May 2013) $
  Revision:       $Revision: 34101 $

  Description:    This file contains Attribute Protocol (ATT) definitions
                  and prototypes.


  Copyright 2009-2013 Texas Instruments Incorporated. All rights reserved.

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
  -->
**************************************************************************************************/

#ifndef ATT_H
#define ATT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "l2cap.h"

/*********************************************************************
 * CONSTANTS
 */

// The Exchanging MTU Size is defined as the maximum size of any packet
// transmitted between a client and a server. A higher layer specification
// defines the default ATT MTU value. The ATT MTU value should be within
// the range 23 to 517 inclusive.
#define ATT_MTU_SIZE                     L2CAP_MTU_SIZE //!< Minimum ATT MTU size
#define ATT_MAX_MTU_SIZE                 517            //!< Maximum ATT MTU size

/** @defgroup ATT_METHOD_DEFINES ATT Methods
 * @{
 */

#define ATT_ERROR_RSP                    0x01 //!< ATT Error Response
#define ATT_EXCHANGE_MTU_REQ             0x02 //!< ATT Exchange MTU Request
#define ATT_EXCHANGE_MTU_RSP             0x03 //!< ATT Exchange MTU Response
#define ATT_FIND_INFO_REQ                0x04 //!< ATT Find Information Request
#define ATT_FIND_INFO_RSP                0x05 //!< ATT Find Information Response
#define ATT_FIND_BY_TYPE_VALUE_REQ       0x06 //!< ATT Find By Type Value Request
#define ATT_FIND_BY_TYPE_VALUE_RSP       0x07 //!< ATT Find By Type Value Response
#define ATT_READ_BY_TYPE_REQ             0x08 //!< ATT Read By Type Request
#define ATT_READ_BY_TYPE_RSP             0x09 //!< ATT Read By Type Response
#define ATT_READ_REQ                     0x0a //!< ATT Read Request
#define ATT_READ_RSP                     0x0b //!< ATT Read Response
#define ATT_READ_BLOB_REQ                0x0c //!< ATT Read Blob Request
#define ATT_READ_BLOB_RSP                0x0d //!< ATT Read Blob Response
#define ATT_READ_MULTI_REQ               0x0e //!< ATT Read Multiple Request
#define ATT_READ_MULTI_RSP               0x0f //!< ATT Read Multiple Response
#define ATT_READ_BY_GRP_TYPE_REQ         0x10 //!< ATT Read By Group Type Request
#define ATT_READ_BY_GRP_TYPE_RSP         0x11 //!< ATT Read By Group Type Response
#define ATT_WRITE_REQ                    0x12 //!< ATT Write Request
#define ATT_WRITE_RSP                    0x13 //!< ATT Write Response
#define ATT_PREPARE_WRITE_REQ            0x16 //!< ATT Prepare Write Request
#define ATT_PREPARE_WRITE_RSP            0x17 //!< ATT Prepare Write Response
#define ATT_EXECUTE_WRITE_REQ            0x18 //!< ATT Execute Write Request
#define ATT_EXECUTE_WRITE_RSP            0x19 //!< ATT Execute Write Response
#define ATT_HANDLE_VALUE_NOTI            0x1b //!< ATT Handle Value Notification
#define ATT_HANDLE_VALUE_IND             0x1d //!< ATT Handle Value Indication
#define ATT_HANDLE_VALUE_CFM             0x1e //!< ATT Handle Value Confirmation

#define ATT_WRITE_CMD                    0x52 //!< ATT Write Command
#define ATT_SIGNED_WRITE_CMD             0xD2 //!< ATT Signed Write Command

/** @} End ATT_METHOD_DEFINES */

/*** Opcode fields: bitmasks ***/
// Method (bits 5-0)
#define ATT_METHOD_BITS                  0x3f

// Command Flag (bit 6)
#define ATT_CMD_FLAG_BIT                 0x40

// Authentication Signature Flag (bit 7)
#define ATT_AUTHEN_SIG_FLAG_BIT          0x80

// Size of 16-bit Bluetooth UUID
#define ATT_BT_UUID_SIZE                 2

// Size of 128-bit UUID
#define ATT_UUID_SIZE                    16

// ATT Response or Confirmation timeout
#define ATT_MSG_TIMEOUT                  30

// Authentication Signature status for received PDU; it's TRUE or FALSE for PDU to be sent
#define ATT_SIG_NOT_INCLUDED             0x00 // Signature not included
#define ATT_SIG_VALID                    0x01 // Included signature valid
#define ATT_SIG_INVALID                  0x02 // Included signature not valid

/*********************************************************************
 * Error Response: Error Code
 */

/** @defgroup ATT_ERR_CODE_DEFINES ATT Error Codes
 * @{
 */

#define ATT_ERR_INVALID_HANDLE           0x01 //!< Attribute handle value given was not valid on this attribute server
#define ATT_ERR_READ_NOT_PERMITTED       0x02 //!< Attribute cannot be read
#define ATT_ERR_WRITE_NOT_PERMITTED      0x03 //!< Attribute cannot be written
#define ATT_ERR_INVALID_PDU              0x04 //!< The attribute PDU was invalid
#define ATT_ERR_INSUFFICIENT_AUTHEN      0x05 //!< The attribute requires authentication before it can be read or written
#define ATT_ERR_UNSUPPORTED_REQ          0x06 //!< Attribute server doesn't support the request received from the attribute client
#define ATT_ERR_INVALID_OFFSET           0x07 //!< Offset specified was past the end of the attribute
#define ATT_ERR_INSUFFICIENT_AUTHOR      0x08 //!< The attribute requires an authorization before it can be read or written
#define ATT_ERR_PREPARE_QUEUE_FULL       0x09 //!< Too many prepare writes have been queued
#define ATT_ERR_ATTR_NOT_FOUND           0x0a //!< No attribute found within the given attribute handle range
#define ATT_ERR_ATTR_NOT_LONG            0x0b //!< Attribute cannot be read or written using the Read Blob Request or Prepare Write Request
#define ATT_ERR_INSUFFICIENT_KEY_SIZE    0x0c //!< The Encryption Key Size used for encrypting this link is insufficient
#define ATT_ERR_INVALID_VALUE_SIZE       0x0d //!< The attribute value length is invalid for the operation
#define ATT_ERR_UNLIKELY                 0x0e //!< The attribute request that was requested has encountered an error that was very unlikely, and therefore could not be completed as requested
#define ATT_ERR_INSUFFICIENT_ENCRYPT     0x0f //!< The attribute requires encryption before it can be read or written
#define ATT_ERR_UNSUPPORTED_GRP_TYPE     0x10 //!< The attribute type is not a supported grouping attribute as defined by a higher layer specification
#define ATT_ERR_INSUFFICIENT_RESOURCES   0x11 //!< Insufficient Resources to complete the request

/*** Reserved for future use: 0x12 - 0x7F ***/

/*** Application error code defined by a higher layer specification: 0x80-0x9F ***/

#define ATT_ERR_INVALID_VALUE            0x80 //!< The attribute value is invalid for the operation

/** @} End ATT_ERR_CODE_DEFINES */

/*********************************************************************
 * @defgroup ATT_VENDOR_SPECIFIC_EVENT_DEFINES
 */
#define	ATT_ERROR_RESPONSE_EVENT			0x0501
#define ATT_EXCHANGE_MTU_REQ_EVENT			0x0502
#define ATT_EXCHANGE_MTU_RSP_EVENT			0x0503
#define ATT_FIND_INFO_REQ_EVENT				0x0504
#define ATT_FIND_INFO_RSP_EVENT				0x0505
#define ATT_FIND_BY_TYPE_VALUE_REQ_EVENT	0x0506
#define ATT_FIND_BY_TYPE_VALUE_RSP_EVENT	0x0507
#define	ATT_READ_BY_TYPE_REQ_EVENT			0x0508
#define ATT_READ_BY_TYPE_RSP_EVENT			0x0509
#define ATT_READ_REQ_EVENT					0x050A
#define ATT_READ_RSP_EVENT					0x050B
#define ATT_READ_BLOB_REQ_EVENT				0x050C
#define ATT_READ_BLOB_RSP_EVENT				0x050D
#define ATT_READ_MULTIPLE_REQ_EVENT			0x050E
#define ATT_READ_MULTIPLE_RSP_EVENT			0x050F
#define ATT_READ_BY_GRP_TYPE_REQ_EVENT		0x0510
#define ATT_READ_BY_GRP_TYPE_RSP_EVENT		0x0511
#define ATT_WRITE_REQ_EVENT					0x0512
#define ATT_WRITE_RSP_EVENT					0x0513
#define ATT_PREPARE_WRITE_REQ_EVENT			0x0516
#define ATT_PREPARE_WRITE_RSP_EVENT			0x0517
#define ATT_EXECUTE_WRITE_REQ_EVENT			0x0518
#define ATT_EXECUTE_WRITE_RSP_EVENT			0x0519
#define ATT_HANDLE_VALUE_NOTIFICATION_EVENT	0x051B
#define ATT_HANDLE_VALUE_INDICATION_EVENT	0x051D
#define ATT_HANDLE_VALUE_CONFIRMATION_EVENT	0x051E

/*********************************************************************
 * Find Information Response: UUID Format
 */
  // Handle and 16-bit Bluetooth UUID
#define ATT_HANDLE_BT_UUID_TYPE          0x01

  // Handle and 128-bit UUID
#define ATT_HANDLE_UUID_TYPE             0x02

// Maximum number of handle and 16-bit UUID pairs in a single Find Info Response
#define ATT_MAX_NUM_HANDLE_BT_UUID       ( ( ATT_MTU_SIZE - 2 ) / ( 2 + ATT_BT_UUID_SIZE ) )

// Maximum number of handle and 128-bit UUID pairs in a single Find Info Response
#define ATT_MAX_NUM_HANDLE_UUID          ( ( ATT_MTU_SIZE - 2 ) / ( 2 + ATT_UUID_SIZE ) )

/*********************************************************************
 * Find By Type Value Response: Handles Infomation (Found Attribute Handle and Group End Handle)
 */
  // Maximum number of handles info in a single Find By Type Value Response
#define ATT_MAX_NUM_HANDLES_INFO         ( ( ATT_MTU_SIZE - 1 ) / 4 )

/*********************************************************************
 * Read Multiple Request: Handles
 */
  // Maximum number of handles in a single Read Multiple Request
#define ATT_MAX_NUM_HANDLES              ( ( ATT_MTU_SIZE - 1 ) / 2 )

  // Minimum number of handles in a single Read Multiple Request
#define ATT_MIN_NUM_HANDLES              2

/*********************************************************************
 * Execute Write Request: Flags
 */
  // Cancel all prepared writes
#define ATT_CANCEL_PREPARED_WRITES       0x00

  // Immediately write all pending prepared values
#define ATT_WRITE_PREPARED_VALUES        0x01

#if defined ( TESTMODES )
  // ATT Test Modes
  #define ATT_TESTMODE_OFF               0 // Test mode off
  #define ATT_TESTMODE_UNAUTHEN_SIG      1 // Do not authenticate incoming signature
#endif

/*********************************************************************
 * Size of mandatory fields of ATT requests
 */
// Length of Read By Type Request's fixed fields: First handle number (2) + Last handle number (2)
#define READ_BY_TYPE_REQ_FIXED_SIZE        4

// Length of Prepare Write Request's fixed size: Attribute Handle (2) + Value Offset (2)
#define PREPARE_WRITE_REQ_FIXED_SIZE       4

/*********************************************************************
 * VARIABLES
 */
extern const uint8_t btBaseUUID[ATT_UUID_SIZE];

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/**
 * ATT event header format.
 */
struct _attEventHdr_def
{
  uint16_t opCode;                  		//!< ATT type of event.
  uint8_t status;							//!< status
  uint16_t connectionHandle;				//!< Connection handle
  uint8_t  pduLen;							//!< PDU Length
} __attribute__((__packed__));
typedef struct _attEventHdr_def attEventHdr_t;

/**
 * Attribute Protocol PDU format.
 */
struct _attPacket_def
{
  uint8_t sig;      //!< Authentication Signature status (not included (0), valid (1), invalid (2))
  uint8_t cmd;      //!< Command Flag
  uint8_t method;   //!< Method
  uint16_t len;     //!< Length of Attribute Parameters
  uint8_t *pParams; //!< Attribute Parameters
} __attribute__((__packed__));
typedef struct _attPacket_def attPacket_t;

/**
 * Attribute Type format (2 or 16 octet UUID).
 */
struct _attAttrType_def
{
  uint8_t len;                 //!< Length of UUID
  uint8_t uuid[ATT_UUID_SIZE]; //!< 16 or 128 bit UUID
} __attribute__((__packed__));
typedef struct _attAttrType_def attAttrType_t;

/**
 * Attribute Type format (2-octet Bluetooth UUID).
 */
struct _attAttrBtType_def
{
  uint8_t len;                    //!< Length of UUID
  uint8_t uuid[ATT_BT_UUID_SIZE]; //!< 16 bit UUID
} __attribute__((__packed__));
typedef struct _attAttrBtType_def attAttrBtType_t;

/**
 * Error Response format.
 */
struct _attErrorRsp_def
{
  attEventHdr_t hdr;
  uint8_t reqOpcode; //!< Request that generated this error response
  uint16_t handle;   //!< Attribute handle that generated error response
  uint8_t errCode;   //!< Reason why the request has generated error response
} __attribute__((__packed__));
typedef struct _attErrorRsp_def attErrorRsp_t;

/**
 * Exchange MTU Request format.
 */
struct _attExchangeMTUReq_def
{
  uint16_t clientRxMTU; //!< Client receive MTU size
} __attribute__((__packed__));
typedef struct _attExchangeMTUReq_def attExchangeMTUReq_t;

/**
 * Exchange MTU Response format.
 */
struct _attExchangeMTURsp_def
{
  attEventHdr_t hdr;	//!< Event header
  uint16_t serverRxMTU; //!< Server receive MTU size
} __attribute__((__packed__));
typedef struct _attExchangeMTURsp_def attExchangeMTURsp_t;

/**
 * Find Information Request format.
 */
struct _attFindInfoReq_def
{
  uint16_t startHandle;       //!< First requested handle number (must be first field)
  uint16_t endHandle;         //!< Last requested handle number
} __attribute__((__packed__));
typedef struct _attFindInfoReq_def attFindInfoReq_t;

/**
 * Handle and its 16-bit Bluetooth UUIDs.
 */
struct _attHandleBtUUID_def
{
  uint16_t handle;                //!< Handle
  uint8_t uuid[ATT_BT_UUID_SIZE]; //!< 2-octet Bluetooth UUID
} __attribute__((__packed__));
typedef struct _attHandleBtUUID_def attHandleBtUUID_t;

/**
 * Handle and its 128-bit UUID.
 */
struct _attHandleUUID_def
{
  uint16_t handle;             //!< Handle
  uint8_t uuid[ATT_UUID_SIZE]; //!< 16-octect UUID
} __attribute__((__packed__));
typedef struct _attHandleUUID_def attHandleUUID_t;

/**
 * Info data format for Find Information Response (handle-UUID pair).
 */
union _attFindInfo_def
{
  attHandleBtUUID_t btPair[ATT_MAX_NUM_HANDLE_BT_UUID]; //!< A list of 1 or more handles with their 16-bit Bluetooth UUIDs
  attHandleUUID_t   pair[ATT_MAX_NUM_HANDLE_UUID];      //!< A list of 1 or more handles with their 128-bit UUIDs
} __attribute__((__packed__));
typedef union _attFindInfo_def attFindInfo_t;

/**
 * Find Information Response format.
 */
struct _attFindInfoRsp_def
{
  uint8_t numInfo;      //!< Number of attribute handle-UUID pairs found
  uint8_t format;       //!< Format of information data
  attFindInfo_t info; //!< Information data whose format is determined by format field
} __attribute__((__packed__));
typedef struct _attFindInfoRsp_def attFindInfoRsp_t;

/**
 * Find By Type Value Request format.
 */
struct _attFindByTypeValueReq_def
{
  attEventHdr_t hdr;
  uint16_t startHandle;          //!< First requested handle number (must be first field)
  uint16_t endHandle;            //!< Last requested handle number
  uint16_t type;				 //!< UUID
  uint8_t value[ATT_MTU_SIZE-7]; //!< Attribute value to find
} __attribute__((__packed__));
typedef struct _attFindByTypeValueReq_def attFindByTypeValueReq_t;

/**
 * Handles Infomation format.
 */
struct _attHandlesInfo_def
{
  uint16_t handle;       //!< Found attribute handle
  uint16_t grpEndHandle; //!< Group end handle
} __attribute__((__packed__));
typedef struct _attHandlesInfo_def attHandlesInfo_t;

/**
 * Find By Type Value Response format.
 */
struct _attFindByTypeValueRsp_def
{
  attEventHdr_t hdr;
  attHandlesInfo_t handlesInfo[ATT_MAX_NUM_HANDLES_INFO]; //!< List of 1 or more handles information
} __attribute__((__packed__));
typedef struct _attFindByTypeValueRsp_def attFindByTypeValueRsp_t;

/**
 * Read By Type Request format.
 */
struct _attReadByTypeReq_def
{
  attEventHdr_t hdr;
  uint16_t startHandle; //!< First requested handle number (must be first field)
  uint16_t endHandle;   //!< Last requested handle number
  uint16_t type;		//!< Requested type
  uint8_t  uuid[16];	//!< Type uuid (2 or 16 octet UUID)
} __attribute__((__packed__));
typedef struct _attReadByTypeReq_def attReadByTypeReq_t;

/**
 * Characteristic declaration format
 */
struct _attCharDesc_def
{
	uint16_t type;			//!< Attribute type (0x2803-UUID for "Characteristic")
	uint8_t  properties;	//!< Characteristic properties
	uint16_t handle;		//!< Characteristic value attribute handle
	uint16_t uuid;			//!< Characteristic UUID
} __attribute__((__packed__));
typedef struct _attCharDesc_def attCharDesc_t;

/**
 * Read By Type Response format.
 */
struct _attReadByTypeRsp_def
{
  attEventHdr_t hdr;
  uint8_t len;                      //!< Size of each attribute handle-value pair
  uint8_t dataList[ATT_MTU_SIZE-2]; //!< List of 1 or more attribute handle-value pairs
} __attribute__((__packed__));
typedef struct _attReadByTypeRsp_def attReadByTypeRsp_t;

/**
 * Read Request format.
 */
struct _attReadReq_def
{
  attEventHdr_t hdr; // AEW
  uint16_t handle; //!< Handle of the attribute to be read (must be first field)
} __attribute__((__packed__));
typedef struct _attReadReq_def attReadReq_t;

/**
 * Read Response format.
 */
struct _attReadRsp_def
{
  attEventHdr_t hdr;
  uint8_t value[ATT_MTU_SIZE-1]; //!< Value of the attribute with the handle given
} __attribute__((__packed__));
typedef struct _attReadRsp_def attReadRsp_t;

/**
 * Read Blob Req format.
 */
struct _attReadBlobReq_def
{
  attEventHdr_t hdr;  // AEW
  uint16_t handle; //!< Handle of the attribute to be read (must be first field)
  uint16_t offset; //!< Offset of the first octet to be read
} __attribute__((__packed__));
typedef struct _attReadBlobReq_def attReadBlobReq_t;

/**
 * Read Blob Response format.
 */
struct _attReadBlobRsp_def
{
  attEventHdr_t hdr;
  uint8_t value[ATT_MTU_SIZE-1]; //!< Part of the value of the attribute with the handle given
} __attribute__((__packed__));
typedef struct _attReadBlobRsp_def attReadBlobRsp_t;

/**
 * Read Multiple Request format.
 */
struct _attReadMultiReq_def
{
  uint16_t handle[ATT_MAX_NUM_HANDLES]; //!< Set of two or more attribute handles (must be first field)
  uint8_t numHandles;                   //!< Number of attribute handles
} __attribute__((__packed__));
typedef struct _attReadMultiReq_def attReadMultiReq_t;

/**
 * Read Multiple Response format.
 */
struct _attReadMultiRsp_def
{
  attEventHdr_t hdr;
  uint8_t values[ATT_MTU_SIZE-1]; //!< Set of two or more values
} __attribute__((__packed__));
typedef struct _attReadMultiRsp_def attReadMultiRsp_t;

/**
 * Read By Group Type Request format.
 */
struct _attReadByGrpTypeReq_def
{
  uint16_t startHandle; //!< First requested handle number (must be first field)
  uint16_t endHandle;   //!< Last requested handle number
  attAttrType_t type; //!< Requested group type (2 or 16 octet UUID)
} __attribute__((__packed__));
typedef struct _attReadByGrpTypeReq_def attReadByGrpTypeReq_t;

/**
 * Read By Group Type Response format.
 */
struct _attReadByGrpTypeRsp_def
{
  attEventHdr_t hdr;
  uint8_t len;                      //!< Length of each attribute handle, end group handle and value set
  uint8_t dataList[ATT_MTU_SIZE-2]; //!< List of 1 or more attribute handle, end group handle and value
} __attribute__((__packed__));
typedef struct _attReadByGrpTypeRsp_def attReadByGrpTypeRsp_t;

/**
 * ATT Write Request Packet
 *
 * This event packet is received when a Write Request
 * has been received for the given handle and data.
 *
 */
 struct _attWriteReq_def
 {
  attEventHdr_t hdr;
  uint8_t  signature;				//!< Authentication signature status (net included (0), valid (1), invalid (2))
  uint8_t  command;					//!< Command flag
  uint16_t handle;					//!< Handle of the attribute to be written
  uint8_t  value[ATT_MTU_SIZE-3]; 	//!< Value of the attribute to be written
 } __attribute__((__packed__));
 typedef struct _attWriteReq_def attWriteReq_t;

/**
 * Prepare Write Request format.
 */
struct _attPrepareWriteReq_def
{
  uint16_t handle;               //!< Handle of the attribute to be written (must be first field)
  uint16_t offset;               //!< Offset of the first octet to be written
  uint8_t len;                   //!< Length of value
  uint8_t value[ATT_MTU_SIZE-5]; //!< Part of the value of the attribute to be written
} __attribute__((__packed__));
typedef struct _attPrepareWriteReq_def attPrepareWriteReq_t;

/**
 * Prepare Write Response format.
 */
struct _attPrepareWriteRsp_def
{
  attEventHdr_t hdr;
  uint16_t handle;               //!< Handle of the attribute that has been read
  uint16_t offset;               //!< Offset of the first octet to be written
  uint8_t value[ATT_MTU_SIZE-5]; //!< Part of the value of the attribute to be written
} __attribute__((__packed__));
typedef struct _attPrepareWriteRsp_def attPrepareWriteRsp_t;

/**
 * Execute Write Request format.
 */
struct _attExecuteWriteReq_def
{
  uint8_t flags; //!< 0x00 - cancel all prepared writes.
               //!< 0x01 - immediately write all pending prepared values.
} __attribute__((__packed__));
typedef struct _attExecuteWriteReq_def attExecuteWriteReq_t;

/**
 * Handle Value Notification format.
 */
struct _attHandleValueNoti_def
{
  uint16_t handle;               //!< Handle of the attribute that has been changed (must be first field)
  uint8_t len;                   //!< Length of value
  uint8_t value[ATT_MTU_SIZE-3]; //!< New value of the attribute
} __attribute__((__packed__));
typedef struct _attHandleValueNoti_def attHandleValueNoti_t;

/**
 * Handle Value Indication format.
 */
struct _attHandleValueInd_def
{
  uint16_t handle;               //!< Handle of the attribute that has been changed (must be first field)
  uint8_t len;                   //!< Length of value
  uint8_t value[ATT_MTU_SIZE-3]; //!< New value of the attribute
} __attribute__((__packed__));
typedef struct _attHandleValueInd_def attHandleValueInd_t;

/**
 * AddAttribute response
 */
struct _attAddAttributeRsp_def
{
	attEventHdr_t hdr;
	attHandlesInfo_t handleRange;
} __attribute__((__packed__));
typedef struct _attAddAttributeRsp_def attAddAttributeRsp_t;

/**
 * ATT Message format. It's a union of all attribute protocol messages used
 * between the attribute protocol and upper layer profile/application.
 */
union _attMsg_def
{
  // Request messages
  attExchangeMTUReq_t exchangeMTUReq;         //!< ATT Exchange MTU Request
  attFindInfoReq_t findInfoReq;               //!< ATT Find Information Request
  attFindByTypeValueReq_t findByTypeValueReq; //!< ATT Find By Type Vaue Request
  attReadByTypeReq_t readByTypeReq;           //!< ATT Read By Type Request
  attReadReq_t readReq;                       //!< ATT Read Request
  attReadBlobReq_t readBlobReq;               //!< ATT Read Blob Request
  attReadMultiReq_t readMultiReq;             //!< ATT Read Multiple Request
  attReadByGrpTypeReq_t readByGrpTypeReq;     //!< ATT Read By Group Type Request
  attWriteReq_t writeReq;                     //!< ATT Write Request
  attPrepareWriteReq_t prepareWriteReq;       //!< ATT Prepare Write Request
  attExecuteWriteReq_t executeWriteReq;       //!< ATT Execute Write Request

  // Response messages
  attEventHdr_t hdr;						  //!< ATT Header
  attErrorRsp_t errorRsp;                     //!< ATT Error Response
  attExchangeMTURsp_t exchangeMTURsp;         //!< ATT Exchange MTU Response
  attFindInfoRsp_t findInfoRsp;               //!< ATT Find Information Response
  attFindByTypeValueRsp_t findByTypeValueRsp; //!< ATT Find By Type Value Response
  attReadByTypeRsp_t readByTypeRsp;           //!< ATT Read By Type Response
  attReadRsp_t readRsp;                       //!< ATT Read Response
  attReadBlobRsp_t readBlobRsp;               //!< ATT Read Blob Response
  attReadMultiRsp_t readMultiRsp;             //!< ATT Read Multiple Response
  attReadByGrpTypeRsp_t readByGrpTypeRsp;     //!< ATT Read By Group Type Response
  attPrepareWriteRsp_t prepareWriteRsp;       //!< ATT Prepare Write Response

  // Indication and Notification messages
  attHandleValueNoti_t handleValueNoti;       //!< ATT Handle Value Notification
  attHandleValueInd_t handleValueInd;         //!< ATT Handle Value Indication
} __attribute__((__packed__));
typedef union _attMsg_def attMsg_t;

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ATT_H */
