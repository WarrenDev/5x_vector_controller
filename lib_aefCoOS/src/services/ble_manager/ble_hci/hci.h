
/*******************************************************************************
  Filename:       hci.h
  Revised:        $Date: 2013-05-14 12:23:59 -0700 (Tue, 14 May 2013) $
  Revision:       $Revision: 34279 $

  Description:    This file contains the Host Controller Interface (HCI) API.
                  It provides the defines, types, and functions for all
                  supported Bluetooth Low Energy (BLE) commands.

                  All Bluetooth and BLE commands are based on:
                  Bluetooth Core Specification, V4.0.0, Vol. 2, Part E.

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
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

*******************************************************************************

  04/17/2014 AEW Ported to the GNU C/C++ ARM environment

*******************************************************************************/

#ifndef HCI_H
#define HCI_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "ll.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*
** HCI Status
**
** Per the Bluetooth Core Specification, V4.0.0, Vol. 2, Part D.
*/
#define HCI_SUCCESS                                                 0x00
#define HCI_ERROR_CODE_UNKNOWN_HCI_CMD                              0x01
#define HCI_ERROR_CODE_UNKNOWN_CONN_ID                              0x02
#define HCI_ERROR_CODE_HW_FAILURE                                   0x03
#define HCI_ERROR_CODE_PAGE_TIMEOUT                                 0x04
#define HCI_ERROR_CODE_AUTH_FAILURE                                 0x05
#define HCI_ERROR_CODE_PIN_KEY_MISSING                              0x06
#define HCI_ERROR_CODE_MEM_CAP_EXCEEDED                             0x07
#define HCI_ERROR_CODE_CONN_TIMEOUT                                 0x08
#define HCI_ERROR_CODE_CONN_LIMIT_EXCEEDED                          0x09
#define HCI_ERROR_CODE_SYNCH_CONN_LIMIT_EXCEEDED                    0x0A
#define HCI_ERROR_CODE_ACL_CONN_ALREADY_EXISTS                      0x0B
#define HCI_ERROR_CODE_CMD_DISALLOWED                               0x0C
#define HCI_ERROR_CODE_CONN_REJ_LIMITED_RESOURCES                   0x0D
#define HCI_ERROR_CODE_CONN_REJECTED_SECURITY_REASONS               0x0E
#define HCI_ERROR_CODE_CONN_REJECTED_UNACCEPTABLE_BDADDR            0x0F
#define HCI_ERROR_CODE_CONN_ACCEPT_TIMEOUT_EXCEEDED                 0x10
#define HCI_ERROR_CODE_UNSUPPORTED_FEATURE_PARAM_VALUE              0x11
#define HCI_ERROR_CODE_INVALID_HCI_CMD_PARAMS                       0x12
#define HCI_ERROR_CODE_REMOTE_USER_TERM_CONN                        0x13
#define HCI_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES        0x14
#define HCI_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_POWER_OFF            0x15
#define HCI_ERROR_CODE_CONN_TERM_BY_LOCAL_HOST                      0x16
#define HCI_ERROR_CODE_REPEATED_ATTEMPTS                            0x17
#define HCI_ERROR_CODE_PAIRING_NOT_ALLOWED                          0x18
#define HCI_ERROR_CODE_UNKNOWN_LMP_PDU                              0x19
#define HCI_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE                   0x1A
#define HCI_ERROR_CODE_SCO_OFFSET_REJ                               0x1B
#define HCI_ERROR_CODE_SCO_INTERVAL_REJ                             0x1C
#define HCI_ERROR_CODE_SCO_AIR_MODE_REJ                             0x1D
#define HCI_ERROR_CODE_INVALID_LMP_PARAMS                           0x1E
#define HCI_ERROR_CODE_UNSPECIFIED_ERROR                            0x1F
#define HCI_ERROR_CODE_UNSUPPORTED_LMP_PARAM_VAL                    0x20
#define HCI_ERROR_CODE_ROLE_CHANGE_NOT_ALLOWED                      0x21
#define HCI_ERROR_CODE_LMP_LL_RESP_TIMEOUT                          0x22
#define HCI_ERROR_CODE_LMP_ERR_TRANSACTION_COLLISION                0x23
#define HCI_ERROR_CODE_LMP_PDU_NOT_ALLOWED                          0x24
#define HCI_ERROR_CODE_ENCRYPT_MODE_NOT_ACCEPTABLE                  0x25
#define HCI_ERROR_CODE_LINK_KEY_CAN_NOT_BE_CHANGED                  0x26
#define HCI_ERROR_CODE_REQ_QOS_NOT_SUPPORTED                        0x27
#define HCI_ERROR_CODE_INSTANT_PASSED                               0x28
#define HCI_ERROR_CODE_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED          0x29
#define HCI_ERROR_CODE_DIFFERENT_TRANSACTION_COLLISION              0x2A
#define HCI_ERROR_CODE_RESERVED1                                    0x2B
#define HCI_ERROR_CODE_QOS_UNACCEPTABLE_PARAM                       0x2C
#define HCI_ERROR_CODE_QOS_REJ                                      0x2D
#define HCI_ERROR_CODE_CHAN_ASSESSMENT_NOT_SUPPORTED                0x2E
#define HCI_ERROR_CODE_INSUFFICIENT_SECURITY                        0x2F
#define HCI_ERROR_CODE_PARAM_OUT_OF_MANDATORY_RANGE                 0x30
#define HCI_ERROR_CODE_RESERVED2                                    0x31
#define HCI_ERROR_CODE_ROLE_SWITCH_PENDING                          0x32
#define HCI_ERROR_CODE_RESERVED3                                    0x33
#define HCI_ERROR_CODE_RESERVED_SLOT_VIOLATION                      0x34
#define HCI_ERROR_CODE_ROLE_SWITCH_FAILED                           0x35
#define HCI_ERROR_CODE_EXTENDED_INQUIRY_RESP_TOO_LARGE              0x36
#define HCI_ERROR_CODE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST			0x37
#define HCI_ERROR_CODE_HOST_BUSY_PAIRING                            0x38
#define HCI_ERROR_CODE_CONN_REJ_NO_SUITABLE_CHAN_FOUND              0x39
#define HCI_ERROR_CODE_CONTROLLER_BUSY                              0x3A
#define HCI_ERROR_CODE_UNACCEPTABLE_CONN_INTERVAL                   0x3B
#define HCI_ERROR_CODE_DIRECTED_ADV_TIMEOUT                         0x3C
#define HCI_ERROR_CODE_CONN_TERM_MIC_FAILURE                        0x3D
#define HCI_ERROR_CODE_CONN_FAILED_TO_ESTABLISH                     0x3E
#define HCI_ERROR_CODE_MAC_CONN_FAILED                              0x3F

/*
** Max Buffers Supported
*/
#define HCI_MAX_NUM_DATA_BUFFERS                       LL_MAX_NUM_DATA_BUFFERS
#define HCI_MAX_NUM_CMD_BUFFERS                        LL_MAX_NUM_CMD_BUFFERS

/*
** HCI Command API Parameters
*/

// Send Data Packet Boundary Flags
#define FIRST_PKT_HOST_TO_CTRL                         LL_DATA_FIRST_PKT_HOST_TO_CTRL
#define CONTINUING_PKT                                 LL_DATA_CONTINUATION_PKT
#define FIRST_PKT_CTRL_TO_HOST                         LL_DATA_FIRST_PKT_CTRL_TO_HOST

// Receive Data Packet
#define HCI_RSSI_NOT_AVAILABLE                         LL_RSSI_NOT_AVAILABLE

// Disconnect Reasons
#define HCI_DISCONNECT_AUTH_FAILURE                    HCI_ERROR_CODE_AUTH_FAILURE
#define HCI_DISCONNECT_REMOTE_USER_TERM                HCI_ERROR_CODE_REMOTE_USER_TERM_CONN
#define HCI_DISCONNECT_REMOTE_DEV_LOW_RESOURCES        HCI_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES
#define HCI_DISCONNECT_REMOTE_DEV_POWER_OFF            HCI_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_POWER_OFF
#define HCI_DISCONNECT_UNSUPPORTED_REMOTE_FEATURE      HCI_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE
#define HCI_DISCONNECT_KEY_PAIRING_NOT_SUPPORTED       HCI_ERROR_CODE_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED
#define HCI_DISCONNECT_UNACCEPTABLE_CONN_INTERVAL      HCI_ERROR_CODE_UNACCEPTABLE_CONN_INTERVAL

// Tx Power Types
#define HCI_READ_CURRENT_TX_POWER_LEVEL                LL_READ_CURRENT_TX_POWER_LEVEL
#define HCI_READ_MAX_TX_POWER_LEVEL                    LL_READ_MAX_TX_POWER_LEVEL

// Host Flow Control
#define HCI_CTRL_TO_HOST_FLOW_CTRL_OFF                 0
#define HCI_CTRL_TO_HOST_FLOW_CTRL_ACL_ON_SYNCH_OFF    1
#define HCI_CTRL_TO_HOST_FLOW_CTRL_ACL_OFF_SYNCH_ON    2
#define HCI_CTRL_TO_HOST_FLOW_CTRL_ACL_ON_SYNCH_ON     3

// Device Address Type
#define HCI_PUBLIC_DEVICE_ADDRESS                      LL_DEV_ADDR_TYPE_PUBLIC
#define HCI_RANDOM_DEVICE_ADDRESS                      LL_DEV_ADDR_TYPE_RANDOM

// Advertiser Events
#define HCI_CONNECTABLE_UNDIRECTED_ADV                 LL_ADV_CONNECTABLE_UNDIRECTED_EVT
#define HCI_CONNECTABLE_DIRECTED_ADV                   LL_ADV_CONNECTABLE_DIRECTED_EVT
#define HCI_SCANNABLE_UNDIRECTED                       LL_ADV_SCANNABLE_UNDIRECTED_EVT
#define HCI_NONCONNECTABLE_UNDIRECTED_ADV              LL_ADV_NONCONNECTABLE_UNDIRECTED_EVT
#define HCI_CONNECTABLE_DIRECTED_LDC_ADV               LL_ADV_CONNECTABLE_DIRECTED_LDC_EVT

// Advertiser Channels
#define HCI_ADV_CHAN_37                                LL_ADV_CHAN_37
#define HCI_ADV_CHAN_38                                LL_ADV_CHAN_38
#define HCI_ADV_CHAN_39                                LL_ADV_CHAN_39
#define HCI_ADV_CHAN_ALL                               (LL_ADV_CHAN_37 | LL_ADV_CHAN_38 | LL_ADV_CHAN_39)

// Advertiser White List Policy
#define HCI_ADV_WL_POLICY_ANY_REQ                      LL_ADV_WL_POLICY_ANY_REQ
#define HCI_ADV_WL_POLICY_WL_SCAN_REQ                  LL_ADV_WL_POLICY_WL_SCAN_REQ
#define HCI_ADV_WL_POLICY_WL_CONNECT_REQ               LL_ADV_WL_POLICY_WL_CONNECT_REQ
#define HCI_ADV_WL_POLICY_WL_ALL_REQ                   LL_ADV_WL_POLICY_WL_ALL_REQ

// Advertiser Commands
#define HCI_ENABLE_ADV                                 LL_ADV_MODE_ON
#define HCI_DISABLE_ADV                                LL_ADV_MODE_OFF

// Scan Types
#define HCI_SCAN_PASSIVE                               LL_SCAN_PASSIVE
#define HCI_SCAN_ACTIVE                                LL_SCAN_ACTIVE

// Scan White List Policy
#define HCI_SCAN_WL_POLICY_ANY_ADV_PKTS                LL_SCAN_WL_POLICY_ANY_ADV_PKTS
#define HCI_SCAN_WL_POLICY_USE_WHITE_LIST              LL_SCAN_WL_POLICY_USE_WHITE_LIST

// Scan Filtering
#define HCI_FILTER_REPORTS_DISABLE                     LL_FILTER_REPORTS_DISABLE
#define HCI_FILTER_REPORTS_ENABLE                      LL_FILTER_REPORTS_ENABLE

// Scan Commands
#define HCI_SCAN_STOP                                  LL_SCAN_STOP
#define HCI_SCAN_START                                 LL_SCAN_START

// Initiator White List Policy
#define HCI_INIT_WL_POLICY_USE_PEER_ADDR               LL_INIT_WL_POLICY_USE_PEER_ADDR
#define HCI_INIT_WL_POLICY_USE_WHITE_LIST              LL_INIT_WL_POLICY_USE_WHITE_LIST

// Encryption Related
#define HCI_ENCRYPTION_OFF                             LL_ENCRYPTION_OFF
#define HCI_ENCRYPTION_ON                              LL_ENCRYPTION_ON

// Direct Test Mode
#define HCI_DTM_NUMBER_RF_CHANS                        LL_DIRECT_TEST_NUM_RF_CHANS
#define HCI_DIRECT_TEST_MAX_PAYLOAD_LEN                LL_DIRECT_TEST_MAX_PAYLOAD_LEN
//
#define HCI_DIRECT_TEST_PAYLOAD_PRBS9                  LL_DIRECT_TEST_PAYLOAD_PRBS9
#define HCI_DIRECT_TEST_PAYLOAD_0x0F                   LL_DIRECT_TEST_PAYLOAD_0x0F
#define HCI_DIRECT_TEST_PAYLOAD_0x55                   LL_DIRECT_TEST_PAYLOAD_0x55
#define HCI_DIRECT_TEST_PAYLOAD_PRBS15                 LL_DIRECT_TEST_PAYLOAD_PRBS15
#define HCI_DIRECT_TEST_PAYLOAD_0xFF                   LL_DIRECT_TEST_PAYLOAD_0xFF
#define HCI_DIRECT_TEST_PAYLOAD_0x00                   LL_DIRECT_TEST_PAYLOAD_0x00
#define HCI_DIRECT_TEST_PAYLOAD_0xF0                   LL_DIRECT_TEST_PAYLOAD_0xF0
#define HCI_DIRECT_TEST_PAYLOAD_0xAA                   LL_DIRECT_TEST_PAYLOAD_0xAA

// Vendor Specific
#define HCI_EXT_RX_GAIN_STD                            LL_EXT_RX_GAIN_STD
#define HCI_EXT_RX_GAIN_HIGH                           LL_EXT_RX_GAIN_HIGH
//
#define HCI_EXT_TX_POWER_MINUS_23_DBM                  LL_EXT_TX_POWER_MINUS_23_DBM
#define HCI_EXT_TX_POWER_MINUS_6_DBM                   LL_EXT_TX_POWER_MINUS_6_DBM
#define HCI_EXT_TX_POWER_0_DBM                         LL_EXT_TX_POWER_0_DBM
#define HCI_EXT_TX_POWER_4_DBM                         LL_EXT_TX_POWER_4_DBM
//
#define HCI_EXT_ENABLE_ONE_PKT_PER_EVT                 LL_EXT_ENABLE_ONE_PKT_PER_EVT
#define HCI_EXT_DISABLE_ONE_PKT_PER_EVT                LL_EXT_DISABLE_ONE_PKT_PER_EVT
//
#define HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT              LL_EXT_ENABLE_CLK_DIVIDE_ON_HALT
#define HCI_EXT_DISABLE_CLK_DIVIDE_ON_HALT             LL_EXT_DISABLE_CLK_DIVIDE_ON_HALT
//
#define HCI_EXT_NV_IN_USE                              LL_EXT_NV_IN_USE
#define HCI_EXT_NV_NOT_IN_USE                          LL_EXT_NV_NOT_IN_USE
//
#define HCI_EXT_ENABLE_FAST_TX_RESP_TIME               LL_EXT_ENABLE_FAST_TX_RESP_TIME
#define HCI_EXT_DISABLE_FAST_TX_RESP_TIME              LL_EXT_DISABLE_FAST_TX_RESP_TIME
//
#define HCI_EXT_ENABLE_SL_OVERRIDE                     LL_EXT_ENABLE_SL_OVERRIDE
#define HCI_EXT_DISABLE_SL_OVERRIDE                    LL_EXT_DISABLE_SL_OVERRIDE
//
#define HCI_EXT_TX_MODULATED_CARRIER                   LL_EXT_TX_MODULATED_CARRIER
#define HCI_EXT_TX_UNMODULATED_CARRIER                 LL_EXT_TX_UNMODULATED_CARRIER
//
#define HCI_PTM_SET_FREQ_TUNE_DOWN                     LL_EXT_SET_FREQ_TUNE_DOWN
#define HCI_PTM_SET_FREQ_TUNE_UP                       LL_EXT_SET_FREQ_TUNE_UP
//
#define HCI_EXT_PM_IO_PORT_P0                          LL_EXT_PM_IO_PORT_P0
#define HCI_EXT_PM_IO_PORT_P1                          LL_EXT_PM_IO_PORT_P1
#define HCI_EXT_PM_IO_PORT_P2                          LL_EXT_PM_IO_PORT_P2
#define HCI_EXT_PM_IO_PORT_NONE                        LL_EXT_PM_IO_PORT_NONE
//
#define HCI_EXT_PM_IO_PORT_PIN0                        LL_EXT_PM_IO_PORT_PIN0
#define HCI_EXT_PM_IO_PORT_PIN1                        LL_EXT_PM_IO_PORT_PIN1
#define HCI_EXT_PM_IO_PORT_PIN2                        LL_EXT_PM_IO_PORT_PIN2
#define HCI_EXT_PM_IO_PORT_PIN3                        LL_EXT_PM_IO_PORT_PIN3
#define HCI_EXT_PM_IO_PORT_PIN4                        LL_EXT_PM_IO_PORT_PIN4
#define HCI_EXT_PM_IO_PORT_PIN5                        LL_EXT_PM_IO_PORT_PIN5
#define HCI_EXT_PM_IO_PORT_PIN6                        LL_EXT_PM_IO_PORT_PIN6
#define HCI_EXT_PM_IO_PORT_PIN7                        LL_EXT_PM_IO_PORT_PIN7
//
#define HCI_EXT_PER_RESET                              LL_EXT_PER_RESET
#define HCI_EXT_PER_READ                               LL_EXT_PER_READ
//
#define HCI_EXT_HALT_DURING_RF_DISABLE                 LL_EXT_HALT_DURING_RF_DISABLE
#define HCI_EXT_HALT_DURING_RF_ENABLE                  LL_EXT_HALT_DURING_RF_ENABLE
//
#define HCI_EXT_SET_USER_REVISION                      LL_EXT_SET_USER_REVISION
#define HCI_EXT_READ_BUILD_REVISION                    LL_EXT_READ_BUILD_REVISION
//
#define HCI_EXT_RESET_SYSTEM_HARD                      LL_EXT_RESET_SYSTEM_HARD
#define HCI_EXT_RESET_SYSTEM_SOFT                      LL_EXT_RESET_SYSTEM_SOFT
//
#define HCI_EXT_DISABLE_OVERLAPPED_PROCESSING          LL_EXT_DISABLE_OVERLAPPED_PROCESSING
#define HCI_EXT_ENABLE_OVERLAPPED_PROCESSING           LL_EXT_ENABLE_OVERLAPPED_PROCESSING
//
#define HCI_EXT_DISABLE_NUM_COMPL_PKTS_ON_EVENT        LL_EXT_DISABLE_NUM_COMPL_PKTS_ON_EVENT
#define HCI_EXT_ENABLE_NUM_COMPL_PKTS_ON_EVENT         LL_EXT_ENABLE_NUM_COMPL_PKTS_ON_EVENT

/*
** HCI Event Parameters
*/

// HCI Link Type for Buffer Overflow
#define HCI_LINK_TYPE_SCO_BUFFER_OVERFLOW              0
#define HCI_LINK_TYPE_ACL_BUFFER_OVERFLOW              1

/*******************************************************************************
 * TYPEDEFS
 */

typedef uint8_t hciStatus_t;

////////////////////////////////////////////////////////////
// Generic event
struct _event_hdr_def
{
  uint8_t  		event;
  uint8_t  		status;
} __attribute__((__packed__));
typedef struct _event_hdr_def _event_hdr_t;

/*
** LE Events
*/

////////////////////////////////////////////////////////////
// LE Connection Complete Event
struct _hciEvt_BLEConnComplete
{
  _event_hdr_t  hdr;
  uint8_t  		BLEEventCode;
  uint8_t  		status;
  uint16_t 		connectionHandle;
  uint8_t  		role;
  uint8_t  		peerAddrType;
  uint8_t  		peerAddr[B_ADDR_LEN];
  uint16_t 		connInterval;
  uint16_t 		connLatency;
  uint16_t 		connTimeout;
  uint8_t  		clockAccuracy;
} __attribute__((__packed__));
typedef struct _hciEvt_BLEConnComplete hciEvt_BLEConnComplete_t;

// LE Advertising Report Event
struct _hciEvt_DevInfo
{
  uint8_t  		eventType;                  // advertisment or scan response event type
  uint8_t  		addrType;                   // public or random address type
  uint8_t  		addr[B_ADDR_LEN];           // device address
  uint8_t  		dataLen;                    // length of report data
  uint8_t  		rspData[B_MAX_ADV_LEN];     // report data given by dataLen
  int8_t   		rssi;                    	// report RSSI
} __attribute__((__packed__));
typedef struct _hciEvt_DevInfo hciEvt_DevInfo_t;

struct _hciEvt_BLEAdvPktReport
{
  _event_hdr_t  hdr;
  uint8_t  		BLEEventCode;
  uint8_t  		numDevices;
  hciEvt_DevInfo_t* devInfo;              	// pointer to the array of devInfo
} __attribute__((__packed__));
typedef struct _hciEvt_BLEAdvPktReport hciEvt_BLEAdvPktReport_t;

// LE Connection Update Complete Event
struct _hciEvt_BLEConnUpdateComplete
{
  _event_hdr_t  hdr;
  uint8_t  		BLEEventCode;
  uint8_t  		status;
  uint16_t 		connectionHandle;
  uint16_t 		connInterval;
  uint16_t 		connLatency;
  uint16_t 		connTimeout;
} __attribute__((__packed__));
typedef struct _hciEvt_BLEConnUpdateComplete hciEvt_BLEConnUpdateComplete_t;

// LE Read Remote Used Features Complete Event
struct _hciEvt_BLEReadRemoteFeatureComplete
{
  _event_hdr_t  hdr;
  uint8_t  		BLEEventCode;
  uint8_t  		status;
  uint16_t 		connectionHandle;
  uint8_t  		features[8];
} __attribute__((__packed__));
typedef struct _hciEvt_BLEReadRemoteFEatureComplete hciEvt_BLEReadRemoteFeatureComplete_t;

// LE Encryption Change Event
struct _hciEvt_EncryptChange
{
  _event_hdr_t  hdr;
  uint8_t  		BLEEventCode;
  uint16_t 		connHandle;
  uint8_t  		reason;
  uint8_t  		encEnable;
} __attribute__((__packed__));
typedef struct _hciEvt_EncryptChange hciEvt_EncryptChange_t;

// LE Long Term Key Requested Event
struct _hciEvt_BLETKReq
{
  _event_hdr_t  hdr;
  uint8_t  		BLEEventCode;
  uint16_t 		connHandle;
  uint8_t  		random[B_RANDOM_NUM_SIZE];
  uint16_t 		encryptedDiversifier;
} __attribute__((__packed__));
typedef struct _hciEvt_BLETKReq hciEvt_BLELTKReq_t;

// Number of Completed Packets Event
struct _hciEvt_NumCompletePkt
{
  _event_hdr_t  hdr;
  uint8_t  		numHandles;
  uint16_t 		*pConnectionHandle;         	// pointer to the connection handle array
  uint16_t 		*pNumCompletedPackets;        	// pointer to the number of completed packets array
} __attribute__((__packed__));
typedef struct _hciEvt_NumCompletePkt hciEvt_NumCompletedPkt_t;

// Command Complete Event
struct _hciEvt_CmdComplete
{
  _event_hdr_t  hdr;
  uint8_t  		numHciCmdPkt;               	// number of HCI Command Packet
  uint16_t 		cmdOpcode;
  uint8_t  		*pReturnParam;           		// pointer to the return parameter
} __attribute__((__packed__));
typedef struct _hciEvt_CmdComplete hciEvt_CmdComplete_t;

// Command Status Event
struct _hciEvt_CommandStatus
{
  _event_hdr_t  hdr;
  uint8_t  		cmdStatus;
  uint8_t  		numHciCmdPkt;
  uint16_t 		cmdOpcode;
} __attribute__((__packed__));
typedef struct _hciEvt_CommandStatus hciEvt_CommandStatus_t;

// Hardware Error Event
struct _hciEvt_HardwareError
{
  _event_hdr_t  hdr;
  uint8_t 		hardwareCode;
} __attribute__((__packed__));
typedef struct _hciEvt_HardwareError hciEvt_HardwareError_t;

// Disconnection Complete Event
struct _hciEvt_DisconnComplete
{
  _event_hdr_t  hdr;
  uint8_t  		status;
  uint16_t 		connHandle;                 	// connection handle
  uint8_t  		reason;
} __attribute__((__packed__));
typedef struct _hciEvt_DisconnComplete hciEvt_DisconnComplete_t;

// Data Buffer Overflow Event
struct _hciEvt_BufferOverflow
{
  _event_hdr_t  hdr;
  uint8_t 		linkType;                    	// synchronous or asynchronous buffer overflow
} __attribute__((__packed__));
typedef struct _hciEvt_BufferOverflow hciEvt_BufferOverflow_t;

// Data structure for HCI Command Complete Event Return Parameter
struct _hciRetParam_LeReadBufSize
{
  uint8_t  		status;
  uint16_t 		dataPktLen;
  uint8_t  		numDataPkts;
} __attribute__((__packed__));
typedef struct _hciRetParam_LeReadBufSize hciRetParam_LeReadBufSize_t;

struct _hciPacket
{
  _event_hdr_t 	hdr;
  uint8_t		*pData;
} __attribute__((__packed__));
typedef struct _hciPacket hciPacket_t;

struct _hciDataPacket
{
  _event_hdr_t 	hdr;
  uint8_t  		pktType;
  uint16_t 		connHandle;
  uint8_t  		pbFlag;
  uint16_t 		pktLen;
  uint8_t  		*pData;
} __attribute__((__packed__));
typedef struct _hciDataPacket hciDataPacket_t;

#ifdef __cplusplus
}
#endif

#endif /* HCI_H */

