
/**
  @headerfile:    gap.h
  $Date: 2013-05-01 13:58:23 -0700 (Wed, 01 May 2013) $
  $Revision: 34101 $

  @mainpage BLE GAP API

  This file contains the interface to the GAP.

  \image html HighLevelGAP.PNG

  \htmlinclude GAPDesc.html

  \image html CentralDeviceDiscoveryProcess.PNG

  A central device can scan for advertising peripheral and broadcaster devices. The Observer process
  is similar to the central process except that will not receive the SCAN_RSP data.
 <BR><BR>
  The central device will initialize the device, then ask for a device discovery.  The GAP will
  setup the link layer to scan and it will filter incoming advertisements and SCAN_RSPs
  based on the type of device discovery parameters passed in when GAP_DeviceDiscoveryRequest() was called.
  Then, when the scan is complete, it will return an array of devices that pass through its filter.
  <BR><BR>

  \image html CentralLinkEstablishmentProcess.PNG

  After the Device Discovery Process, the central device can establish a connection with a peripheral
  device by calling GAP_EstablishLinkRequest().  When the link has been established the GAP will send
  back the @ref GAP_LINK_ESTABLISHED_EVENT [GAP_LinkEstablished].
  <BR><BR>

  \image html CentralPairingProcess.PNG

  After a link is established, the central device can initiate a pairing process to either encrypt the
  link or exchange keys for bonding.
  <BR><BR><BR><BR><BR><BR>

  Copyright 2009 - 2013 Texas Instruments Incorporated. All rights reserved.

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
*/


#ifndef GAP_H
#define GAP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*-------------------------------------------------------------------
 * INCLUDES
 */
#include "bcomdef.h"
#include "sm.h"

/*-------------------------------------------------------------------
 * MACROS
 */

/*-------------------------------------------------------------------
 * CONSTANTS
 */

/** @defgroup BLE_GAP_DEFINES BLE GAP Constants and Structures
 * @{
 */

/** @defgroup GAP_MSG_EVENT_DEFINES GAP Message IDs
 * @{
 */
#define GAP_DEVICE_INIT_DONE_EVENT            0x0600	//!< Sent when the Device Initialization is complete.
#define GAP_DEVICE_DISCOVERY_EVENT            0x0601	//!< Sent when the Device Discovery Process is complete.
#define GAP_ADV_DATA_UPDATE_DONE_EVENT        0x0602	//!< Sent when the Advertising Data or SCAN_RSP Data has been updated.
#define GAP_MAKE_DISCOVERABLE_DONE_EVENT      0x0603	//!< Sent when the Make Discoverable Request is complete.
#define GAP_END_DISCOVERABLE_DONE_EVENT       0x0604	//!< Sent when the Advertising has ended.
#define GAP_LINK_ESTABLISHED_EVENT            0x0605	//!< Sent when the Establish Link Request is complete.
#define GAP_LINK_TERMINATED_EVENT             0x0606	//!< Sent when a connection was terminated.
#define GAP_LINK_PARAM_UPDATE_EVENT           0x0607	//!< Sent when an Update Parameters Event is received.
#define GAP_RANDOM_ADDR_CHANGED_EVENT         0x0608	//!< Sent when a random address was changed.
#define GAP_SIGNATURE_UPDATED_EVENT           0x0609	//!< Sent when the device's signature counter is updated.
#define GAP_AUTHENTICATION_COMPLETE_EVENT     0x060A	//!< Sent when the Authentication (pairing) process is complete.
#define GAP_PASSKEY_NEEDED_EVENT              0x060B	//!< Sent when a Passkey is needed.  This is part of the pairing process.
#define GAP_SLAVE_REQUESTED_SECURITY_EVENT    0x060C	//!< Sent when a Slave Security Request is received.
#define GAP_DEVICE_INFO_EVENT                 0x060D	//!< Sent during the Device Discovery Process when a device is discovered.
#define GAP_BOND_COMPLETE_EVENT               0x060E	//!< Sent when the bonding(bound) process is complete.
#define GAP_PAIRING_REQ_EVENT                 0x060F	//!< Sent when an unexpected Pairing Request is received.
#define GAP_COMMAND_STATUS_EVENT			  0x067F	//!< Sent after some commands are issued
/** @} End GAP_MSG_EVENT_DEFINES */

/** @defgroup GAP_CONN_HANDLE_DEFINES GAP Special Connection Handles
 * Used by GAP_TerminateLinkReq()
 * @{
 */
#define GAP_CONNHANDLE_INIT     0xFFFE  //!< terminates a link create
#define GAP_CONNHANDLE_ALL      0xFFFF  //!< terminates all links for the matching task ID.
/** @} End GAP_CONN_HANDLE_DEFINES */

/** @defgroup GAP_PROFILE_ROLE_DEFINES GAP Profile Roles
 * Bit mask values
 * @{
 */
#define GAP_PROFILE_BROADCASTER   0x01 //!< A device that sends advertising events only.
#define GAP_PROFILE_OBSERVER      0x02 //!< A device that receives advertising events only.
#define GAP_PROFILE_PERIPHERAL    0x04 //!< A device that accepts the establishment of an LE physical link using the connection establishment procedure
#define GAP_PROFILE_CENTRAL       0x08 //!< A device that supports the Central role initiates the establishment of a physical connection
/** @} End GAP_PROFILE_ROLE_DEFINES */

/**
 * @defgroup GAP_PARAMETER_ID_DEFINES GAP Parameter IDs
 * Used in place of gapParamIDs_t.
 * @{
 */
// Timers
#define TGAP_GEN_DISC_ADV_MIN          0  //!< Minimum time to remain advertising, when in Discoverable mode (mSec).  Setting this parameter to 0 turns off the timeout (default).
#define TGAP_LIM_ADV_TIMEOUT           1  //!< Maximum time to remain advertising, when in Limited Discoverable mode. In seconds (default 180 seconds)
#define TGAP_GEN_DISC_SCAN             2  //!< Minimum time to perform scanning, when performing General Discovery proc (mSec)
#define TGAP_LIM_DISC_SCAN             3  //!< Minimum time to perform scanning, when performing Limited Discovery proc (mSec)
#define TGAP_CONN_EST_ADV_TIMEOUT      4  //!< Advertising timeout, when performing Connection Establishment proc (mSec)
#define TGAP_CONN_PARAM_TIMEOUT        5  //!< Link Layer connection parameter update notification timer, connection parameter update proc (mSec)

// Constants
#define TGAP_LIM_DISC_ADV_INT_MIN      6  //!< Minimum advertising interval, when in limited discoverable mode (n * 0.625 mSec)
#define TGAP_LIM_DISC_ADV_INT_MAX      7  //!< Maximum advertising interval, when in limited discoverable mode (n * 0.625 mSec)
#define TGAP_GEN_DISC_ADV_INT_MIN      8  //!< Minimum advertising interval, when in General discoverable mode (n * 0.625 mSec)
#define TGAP_GEN_DISC_ADV_INT_MAX      9  //!< Maximum advertising interval, when in General discoverable mode (n * 0.625 mSec)
#define TGAP_CONN_ADV_INT_MIN         10  //!< Minimum advertising interval, when in Connectable mode (n * 0.625 mSec)
#define TGAP_CONN_ADV_INT_MAX         11  //!< Maximum advertising interval, when in Connectable mode (n * 0.625 mSec)
#define TGAP_CONN_SCAN_INT            12  //!< Scan interval used during Link Layer Initiating state, when in Connectable mode (n * 0.625 mSec)
#define TGAP_CONN_SCAN_WIND           13  //!< Scan window used during Link Layer Initiating state, when in Connectable mode (n * 0.625 mSec)
#define TGAP_CONN_HIGH_SCAN_INT       14  //!< Scan interval used during Link Layer Initiating state, when in Connectable mode, high duty scan cycle scan paramaters (n * 0.625 mSec)
#define TGAP_CONN_HIGH_SCAN_WIND      15  //!< Scan window used during Link Layer Initiating state, when in Connectable mode, high duty scan cycle scan paramaters (n * 0.625 mSec)
#define TGAP_GEN_DISC_SCAN_INT        16  //!< Scan interval used during Link Layer Scanning state, when in General Discovery proc (n * 0.625 mSec)
#define TGAP_GEN_DISC_SCAN_WIND       17  //!< Scan window used during Link Layer Scanning state, when in General Discovery proc (n * 0.625 mSec)
#define TGAP_LIM_DISC_SCAN_INT        18  //!< Scan interval used during Link Layer Scanning state, when in Limited Discovery proc (n * 0.625 mSec)
#define TGAP_LIM_DISC_SCAN_WIND       19  //!< Scan window used during Link Layer Scanning state, when in Limited Discovery proc (n * 0.625 mSec)
#define TGAP_CONN_EST_ADV             20  //!< Advertising interval, when using Connection Establishment proc (n * 0.625 mSec). Obsolete - Do not use.
#define TGAP_CONN_EST_INT_MIN         21  //!< Minimum Link Layer connection interval, when using Connection Establishment proc (n * 1.25 mSec)
#define TGAP_CONN_EST_INT_MAX         22  //!< Maximum Link Layer connection interval, when using Connection Establishment proc (n * 1.25 mSec)
#define TGAP_CONN_EST_SCAN_INT        23  //!< Scan interval used during Link Layer Initiating state, when using Connection Establishment proc (n * 0.625 mSec)
#define TGAP_CONN_EST_SCAN_WIND       24  //!< Scan window used during Link Layer Initiating state, when using Connection Establishment proc (n * 0.625 mSec)
#define TGAP_CONN_EST_SUPERV_TIMEOUT  25  //!< Link Layer connection supervision timeout, when using Connection Establishment proc (n * 10 mSec)
#define TGAP_CONN_EST_LATENCY         26  //!< Link Layer connection slave latency, when using Connection Establishment proc (in number of connection events)
#define TGAP_CONN_EST_MIN_CE_LEN      27  //!< Local informational parameter about min len of connection needed, when using Connection Establishment proc (n * 0.625 mSec)
#define TGAP_CONN_EST_MAX_CE_LEN      28  //!< Local informational parameter about max len of connection needed, when using Connection Establishment proc (n * 0.625 mSec)
#define TGAP_PRIVATE_ADDR_INT         29  //!< Minimum Time Interval between private (resolvable) address changes. In minutes (default 15 minutes)
#define TGAP_CONN_PAUSE_CENTRAL       30  //!< Central idle timer. In seconds (default 1 second)
#define TGAP_CONN_PAUSE_PERIPHERAL    31  //!< Minimum time upon connection establishment before the peripheral starts a connection update procedure. In seconds (default 5 seconds)

// Proprietary
#define TGAP_SM_TIMEOUT               32  //!< SM Message Timeout (milliseconds). Default 30 seconds.
#define TGAP_SM_MIN_KEY_LEN           33  //!< SM Minimum Key Length supported. Default 7.
#define TGAP_SM_MAX_KEY_LEN           34  //!< SM Maximum Key Length supported. Default 16.
#define TGAP_FILTER_ADV_REPORTS       35  //!< Filter duplicate advertising reports. Default TRUE.
#define TGAP_SCAN_RSP_RSSI_MIN        36  //!< Minimum RSSI required for scan responses to be reported to the app. Default -127.

#if !defined ( TESTMODES )
  #define TGAP_AUTH_TASK_ID           37  //!< Task ID override for Task Authentication control (for stack internal use only)
  #define TGAP_PARAMID_MAX            38  //!< ID MAX-valid Parameter ID
#else
  #define TGAP_GAP_TESTCODE           37  //!< GAP TestCodes - puts GAP into a test mode
  #define TGAP_SM_TESTCODE            38  //!< SM TestCodes - puts SM into a test mode
  #define TGAP_AUTH_TASK_ID           39  //!< Task ID override for Task Authentication control (for stack internal use only)
  #define TGAP_PARAMID_MAX            40  //!< ID MAX-valid Parameter ID

  #define TGAP_GATT_TESTCODE          100 //!< GATT TestCodes - puts GATT into a test mode (paramValue maintained by GATT)
  #define TGAP_ATT_TESTCODE           101 //!< ATT TestCodes - puts ATT into a test mode (paramValue maintained by ATT)
  #define TGAP_GGS_TESTCODE           102 //!< GGS TestCodes - puts GGS into a test mode (paramValue maintained by GGS)
#endif

/** @} End GAP_PARAMETER_ID_DEFINES */

/** @defgroup GAP_DEVDISC_MODE_DEFINES GAP Device Discovery Modes
 * @{
 */
#define DEVDISC_MODE_NONDISCOVERABLE  0x00    //!< No discoverable setting
#define DEVDISC_MODE_GENERAL          0x01    //!< General Discoverable devices
#define DEVDISC_MODE_LIMITED          0x02    //!< Limited Discoverable devices
#define DEVDISC_MODE_ALL              0x03    //!< Not filtered
/** @} End GAP_DEVDISC_MODE_DEFINES */

/** @defgroup GAP_ADDR_TYPE_DEFINES GAP Address Types
 * @{
 */
#define ADDRTYPE_PUBLIC               0x00  //!< Use the BD_ADDR
#define ADDRTYPE_STATIC               0x01  //!< Static address
#define ADDRTYPE_PRIVATE_NONRESOLVE   0x02  //!< Generate Non-Resolvable Private Address
#define ADDRTYPE_PRIVATE_RESOLVE      0x03  //!< Generate Resolvable Private Address
/** @} End GAP_ADDR_TYPE_DEFINES */

/** @defgroup GAP_ADVERTISEMENT_TYPE_DEFINES GAP Advertiser Event Types
 * for eventType field in gapAdvertisingParams_t, gapDevRec_t and gapDeviceInfoEvent_t
 * @{
 */
#define GAP_ADTYPE_ADV_IND                0x00  //!< Connectable undirected advertisement
#define GAP_ADTYPE_ADV_DIRECT_IND         0x01  //!< Connectable directed advertisement
#define GAP_ADTYPE_ADV_DISCOVER_IND       0x02  //!< Discoverable undirected advertisement
#define GAP_ADTYPE_ADV_NONCONN_IND        0x03  //!< Non-Connectable undirected advertisement
#define GAP_ADTYPE_SCAN_RSP_IND           0x04  //!< Only used in gapDeviceInfoEvent_t
/** @} End GAP_ADVERTISEMENT_TYPE_DEFINES */

/** @defgroup GAP_FILTER_POLICY_DEFINES GAP Advertiser Filter Scan Parameters
 * @{
 */
#define GAP_FILTER_POLICY_ALL         0x00 //!< Allow Scan Request from Any, Allow Connect Request from Any (default).
#define GAP_FILTER_POLICY_WHITE_SCAN  0x01 //!< Allow Scan Request from White List Only, Allow Connect from Any
#define GAP_FILTER_POLICY_WHITE_CON   0x02 //!< Allow Scan Request from Any, Connect from White List Only
#define GAP_FILTER_POLICY_WHITE       0x03 //!< Allow Scan Request and Connect from White List Only
/** @} End GAP_FILTER_POLICY_DEFINES */

//! Advertiser Channel Map
#define ADV_CHANMAP_SIZE   5

//! Maximum Pairing Passcode/Passkey value.  Range of a passkey can be 0 - 999,999.
#define GAP_PASSCODE_MAX                 999999

/** Sign Counter Initialized - Sign counter hasn't been used yet.  Used when setting up
 *  a connection's signing information.
 */
#define GAP_INIT_SIGN_COUNTER            0xFFFFFFFF

/** @defgroup GAP_ADVCHAN_DEFINES GAP Advertisement Channel Map
 * @{
 */
#define GAP_ADVCHAN_37  0x01  //!< Advertisement Channel 37
#define GAP_ADVCHAN_38  0x02  //!< Advertisement Channel 38
#define GAP_ADVCHAN_39  0x04  //!< Advertisement Channel 39
#define GAP_ADVCHAN_ALL (GAP_ADVCHAN_37 | GAP_ADVCHAN_38 | GAP_ADVCHAN_39) //!< All Advertisement Channels Enabled
/** @} End GAP_ADVCHAN_DEFINES */

/** @defgroup GAP_WHITELIST_DEFINES GAP White List Options
 * @{
 */
#define WL_NOTUSED    0x00    //!< White list not used but the advertiser's address in this command is used
#define WL_USED       0x01    //!< White list is used and the advertiser's address in this command is not used.
/** @} End GAP_WHITELIST_DEFINES */

/** @defgroup GAP_ADTYPE_DEFINES GAP Advertisment Data Types
 * These are the data type identifiers for the data tokens in the advertisement data field.
 * @{
 */
#define GAP_ADTYPE_FLAGS                        0x01 //!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
#define GAP_ADTYPE_16BIT_MORE                   0x02 //!< Service: More 16-bit UUIDs available
#define GAP_ADTYPE_16BIT_COMPLETE               0x03 //!< Service: Complete list of 16-bit UUIDs
#define GAP_ADTYPE_32BIT_MORE                   0x04 //!< Service: More 32-bit UUIDs available
#define GAP_ADTYPE_32BIT_COMPLETE               0x05 //!< Service: Complete list of 32-bit UUIDs
#define GAP_ADTYPE_128BIT_MORE                  0x06 //!< Service: More 128-bit UUIDs available
#define GAP_ADTYPE_128BIT_COMPLETE              0x07 //!< Service: Complete list of 128-bit UUIDs
#define GAP_ADTYPE_LOCAL_NAME_SHORT             0x08 //!< Shortened local name
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 //!< Complete local name
#define GAP_ADTYPE_POWER_LEVEL                  0x0A //!< TX Power Level: 0xXX: -127 to +127 dBm
#define GAP_ADTYPE_OOB_CLASS_OF_DEVICE          0x0D //!< Simple Pairing OOB Tag: Class of device (3 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     0x0E //!< Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     0x0F //!< Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets)
#define GAP_ADTYPE_SM_TK                        0x10 //!< Security Manager TK Value
#define GAP_ADTYPE_SM_OOB_FLAG                  0x11 //!< Secutiry Manager OOB Flags
#define GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    0x12 //!< Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max)
#define GAP_ADTYPE_SIGNED_DATA                  0x13 //!< Signed Data field
#define GAP_ADTYPE_SERVICES_LIST_16BIT          0x14 //!< Service Solicitation: list of 16-bit Service UUIDs
#define GAP_ADTYPE_SERVICES_LIST_128BIT         0x15 //!< Service Solicitation: list of 128-bit Service UUIDs
#define GAP_ADTYPE_SERVICE_DATA                 0x16 //!< Service Data
#define GAP_ADTYPE_APPEARANCE                   0x19 //!< Appearance
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC        0xFF //!< Manufacturer Specific Data: first 2 octets contain the Company Identifier Code followed by the additional manufacturer specific data
/** @} End GAP_ADTYPE_DEFINES */

/** @defgroup GAP_ADTYPE_FLAGS_MODES GAP ADTYPE Flags Discovery Modes
 * @{
 */
#define GAP_ADTYPE_FLAGS_LIMITED                0x01 //!< Discovery Mode: LE Limited Discoverable Mode
#define GAP_ADTYPE_FLAGS_GENERAL                0x02 //!< Discovery Mode: LE General Discoverable Mode
#define GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    0x04 //!< Discovery Mode: BR/EDR Not Supported
/** @} End GAP_ADTYPE_FLAGS_MODES */

/** @defgroup GAP_APPEARANCE_VALUES GAP Appearance Values
 * @{
 */
#define GAP_APPEARE_UNKNOWN                     0x0000 //!< Unknown
#define GAP_APPEARE_GENERIC_PHONE               0x0040 //!< Generic Phone
#define GAP_APPEARE_GENERIC_COMPUTER            0x0080 //!< Generic Computer
#define GAP_APPEARE_GENERIC_WATCH               0x00C0 //!< Generic Watch
#define GAP_APPEARE_WATCH_SPORTS                0x00C1 //!< Watch: Sports Watch
#define GAP_APPEARE_GENERIC_CLOCK               0x0100 //!< Generic Clock
#define GAP_APPEARE_GENERIC_DISPLAY             0x0140 //!< Generic Display
#define GAP_APPEARE_GENERIC_RC                  0x0180 //!< Generic Remote Control
#define GAP_APPEARE_GENERIC_EYE_GALSSES         0x01C0 //!< Generic Eye-glasses
#define GAP_APPEARE_GENERIC_TAG                 0x0200 //!< Generic Tag
#define GAP_APPEARE_GENERIC_KEYRING             0x0240 //!< Generic Keyring
#define GAP_APPEARE_GENERIC_MEDIA_PLAYER        0x0280 //!< Generic Media Player
#define GAP_APPEARE_GENERIC_BARCODE_SCANNER     0x02C0 //!< Generic Barcode Scanner
#define GAP_APPEARE_GENERIC_THERMOMETER         0x0300 //!< Generic Thermometer
#define GAP_APPEARE_GENERIC_THERMO_EAR          0x0301 //!< Thermometer: Ear
#define GAP_APPEARE_GENERIC_HR_SENSOR           0x0340 //!< Generic Heart rate Sensor
#define GAP_APPEARE_GENERIC_HRS_BELT            0x0341 //!< Heart Rate Sensor: Heart Rate Belt
#define GAP_APPEARE_GENERIC_BLOOD_PRESSURE      0x0380 //!< Generic Blood Pressure
#define GAP_APPEARE_GENERIC_BP_ARM              0x0381 //!< Blood Pressure: Arm
#define GAP_APPEARE_GENERIC_BP_WRIST            0x0382 //!< Blood Pressure: Wrist
#define GAP_APPEARE_GENERIC_HID                 0x03C0 //!< Generic Human Interface Device (HID)
#define GAP_APPEARE_HID_KEYBOARD                0x03C1 //!< HID Keyboard
#define GAP_APPEARE_HID_MOUSE                   0x03C2 //!< HID Mouse
#define GAP_APPEARE_HID_JOYSTIC                 0x03C3 //!< HID Joystick
#define GAP_APPEARE_HID_GAMEPAD                 0x03C4 //!< HID Gamepad
#define GAP_APPEARE_HID_DIGITIZER_TYABLET       0x03C5 //!< HID Digitizer Tablet
#define GAP_APPEARE_HID_DIGITAL_CARDREADER      0x03C6 //!< HID Card Reader
#define GAP_APPEARE_HID_DIGITAL_PEN             0x03C7 //!< HID Digital Pen
#define GAP_APPEARE_HID_BARCODE_SCANNER         0x03C8 //!< HID Barcode Scanner
/** @} End GAP_APPEARANCE_VALUES */

/*-------------------------------------------------------------------
 * TYPEDEFS - Initialization and Configuration
 */

/**
 * GAP Parameters IDs: @ref GAP_PARAMETER_ID_DEFINES
 */
typedef uint16_t gapParamIDs_t;

/**
 * GAP event header format.
 */
struct _gapEventHdr_def
{
  uint16_t opCode;                  		//!< GAP type of command. Ref: @ref GAP_MSG_EVENT_DEFINES
  uint8_t status;							//!< status
} __attribute__((__packed__));
typedef struct _gapEventHdr_def gapEventHdr_t;

/**
 * GAP_RANDOM_ADDR_CHANGED_EVENT message format.  This message is sent to the
 * app when the random address changes.
 */
struct _gapRandomAddrEvent_def
{
  gapEventHdr_t  hdr;              		//!< GAP_MSG_EVENT and status
  uint8_t addrType;                     //!< Address type: @ref GAP_ADDR_TYPE_DEFINES
  uint8_t newRandomAddr[B_ADDR_LEN];    //!< the new calculated private addr
} __attribute__((__packed__));
typedef struct _gapRandomAddrEvent_def gapRandomAddrEvent_t;

/**
 * Connection parameters for the peripheral device.  These numbers are used
 * to compare against connection events and request connection parameter
 * updates with the master.
 */
struct _gapPeriConnectParams_def
{
  /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  uint16_t intervalMin;
  /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  uint16_t intervalMax;
  /** Number of LL latency connection events (0x0000 - 0x03e8) */
  uint16_t latency;
  /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
  uint16_t timeout;
} __attribute__((__packed__));
typedef struct _gapPeriConnectParams_def gapPeriConnectParams_t;

/**
 * GAP_DEVICE_INIT_DONE_EVENT message format.  This message is sent to the
 * app when the Device Initialization is done [initiated by calling
 * GAP_DeviceInit()].
 */
struct _gapDeviceInitDoneEvent_def
{
  gapEventHdr_t  hdr;              		//!< GAP_MSG_EVENT and status
  uint8_t devAddr[B_ADDR_LEN];          //!< Device's BD_ADDR
  uint16_t dataPktLen;                  //!< HC_LE_Data_Packet_Length
  uint8_t numDataPkts;                  //!< HC_Total_Num_LE_Data_Packets
  uint8_t IRK[16];						//!< Identity resolving key
  uint8_t CSRK[16];						//!< Connection signature resolving key
} __attribute__((__packed__));
typedef struct _gapDeviceInitDoneEvent_def gapDeviceInitDoneEvent_t;

/**
 * GAP_SIGNATURE_UPDATED_EVENT message format.  This message is sent to the
 * app when the signature counter has changed.  This message is to inform the
 * application in case it wants to save it to be restored on reboot or reconnect.
 * This message is sent to update a connection's signature counter and to update
 * this device's signature counter.  If devAddr == BD_ADDR, then this message pertains
 * to this device.
 */
struct _gapSignUpdateEvent_def
{
  gapEventHdr_t  hdr;              		//!< GAP_MSG_EVENT and status
  uint8_t opcode;                       //!< GAP_SIGNATURE_UPDATED_EVENT
  uint8_t addrType;                     //!< Device's address type for devAddr
  uint8_t devAddr[B_ADDR_LEN];          //!< Device's BD_ADDR, could be own address
  uint32_t signCounter;                 //!< new Signed Counter
} __attribute__((__packed__));
typedef struct _gapSignUpdateEvent_def gapSignUpdateEvent_t;

/**
 * GAP_DEVICE_INFO_EVENT message format.  This message is sent to the
 * app during a Device Discovery Request, when a new advertisement or scan
 * response is received.
 */
struct _gapDeviceInfoEvent_def
{
  gapEventHdr_t  hdr;    		//!< GAP_MSG_EVENT and status
  uint8_t eventType;          	//!< Advertisement Type: @ref GAP_ADVERTISEMENT_TYPE_DEFINES
  uint8_t addrType;           	//!< address type: @ref GAP_ADDR_TYPE_DEFINES
  uint8_t addr[B_ADDR_LEN];   	//!< Address of the advertisement or SCAN_RSP
  int8_t rssi;                	//!< Advertisement or SCAN_RSP RSSI
  uint8_t dataLen;            	//!< Length (in bytes) of the data field (evtData)
  uint8_t *pEvtData;          	//!< Data field of advertisement or SCAN_RSP
} __attribute__((__packed__));
typedef struct _gapDeviceInfoEvent_def gapDeviceInfoEvent_t;

/*-------------------------------------------------------------------
 * TYPEDEFS - Device Discovery
 */

/**
 * Type of device discovery (Scan) to perform.
 */
struct _gapDevRec_def
{
  uint8_t eventType;        //!< Indicates advertising event type used by the advertiser: @ref GAP_ADVERTISEMENT_TYPE_DEFINES
  uint8_t addrType;         //!< Address Type: @ref GAP_ADDR_TYPE_DEFINES
  uint8_t addr[B_ADDR_LEN]; //!< Device's Address
} __attribute__((__packed__));
typedef struct _gapDevRec_def gapDevRec_t;

/**
 * GAP_DEVICE_DISCOVERY_EVENT message format. This message is sent to the
 * Application after a scan is performed.
 */
struct _gapDevDiscEvent_def
{
  gapEventHdr_t  hdr; 		//!< GAP_MSG_EVENT and status
  uint8_t numDevs;         	//!< Number of devices found during scan
  gapDevRec_t *pDevList; 	//!< array of device records
} __attribute__((__packed__));
typedef struct _gapDevDiscEvent_def gapDevDiscEvent_t;

/**
 * Advertising Parameters
 */
struct _gapAdvertisingParams_def
{
  uint8_t eventType;          //!< Advertise Event Type: @ref GAP_ADVERTISEMENT_TYPE_DEFINES
  uint8_t initiatorAddrType;  //!< Initiator's address type: @ref GAP_ADDR_TYPE_DEFINES
  uint8_t initiatorAddr[B_ADDR_LEN];  //!< Initiator's addr - used only with connectable directed eventType (ADV_EVTTYPE_CONNECTABLE_DIRECTED).
  uint8_t channelMap;         //!< Channel Map: Bit mask @ref GAP_ADVCHAN_DEFINES
  uint8_t filterPolicy;       //!< Filer Policy: @ref GAP_FILTER_POLICY_DEFINES. Ignored when directed advertising is used.
} __attribute__((__packed__));
typedef struct _gapAdvertisingParams_def gapAdvertisingParams_t;

/**
 * GAP_MAKE_DISCOVERABLE_DONE_EVENT message format.  This message is sent to the
 * app when the Advertise config is complete.
 */
struct _gapMakeDiscoverableRspEvent_def
{
  gapEventHdr_t  hdr; 		//!< GAP_MSG_EVENT and status
  uint16_t interval;       	//!< actual advertising interval selected by controller
} __attribute__((__packed__));
typedef struct _gapMakeDiscoverableRspEvent_def gapMakeDiscoverableRspEvent_t;

/**
 * GAP_END_DISCOVERABLE_DONE_EVENT message format.  This message is sent to the
 * app when the Advertising has stopped.
 */
struct _gapEndDiscoverableRspEvent_def
{
  gapEventHdr_t  hdr; //!< GAP_MSG_EVENT and status
} __attribute__((__packed__));
typedef struct _gapEndDiscoverableRspEvent_def gapEndDiscoverableRspEvent_t;

/**
 * GAP_ADV_DATA_UPDATE_DONE_EVENT message format.  This message is sent to the
 * app when Advertising Data Update is complete.
 */
struct _gapAdvDataUpdateEvent_def
{
  gapEventHdr_t  hdr; 		//!< GAP_MSG_EVENT and status
  uint8_t adType;          	//!< TRUE if advertising data, FALSE if SCAN_RSP
} __attribute__((__packed__));
typedef struct _gapAdvDataUpdateEvent_def gapAdvDataUpdateEvent_t;

/*-------------------------------------------------------------------
 * TYPEDEFS - Link Establishment
 */

/**
 * Update Link Parameters Request parameters
 */
struct _gapUpdateLinkParamReq_def
{
  uint16_t connectionHandle; //!< Connection handle of the update
  uint16_t intervalMin;      //!< Minimum Connection Interval
  uint16_t intervalMax;      //!< Maximum Connection Interval
  uint16_t connLatency;      //!< Connection Latency
  uint16_t connTimeout;      //!< Connection Timeout
} __attribute__((__packed__));
typedef struct _gapUpdateLinkParamReq_def gapUpdateLinkParamReq_t;

/**
 * GAP_LINK_ESTABLISHED_EVENT message format.  This message is sent to the app
 * when the link request is complete.<BR>
 * <BR>
 * For an Observer, this message is sent to complete the Establish Link Request.<BR>
 * For a Peripheral, this message is sent to indicate that a link has been created.
 */
struct _gapEstLinkReqEvent_def
{
  gapEventHdr_t  hdr;     		//!< GAP_MSG_EVENT and status
  uint8_t devAddrType;         	//!< Device address type: @ref GAP_ADDR_TYPE_DEFINES
  uint8_t devAddr[B_ADDR_LEN]; 	//!< Device address of link
  uint16_t connectionHandle;   	//!< Connection Handle from controller used to ref the device
  uint16_t connInterval;       	//!< Connection Interval
  uint16_t connLatency;        	//!< Connection Latency
  uint16_t connTimeout;        	//!< Connection Timeout
  uint8_t clockAccuracy;       	//!< Clock Accuracy
} __attribute__((__packed__));
typedef struct _gapEstLinkReqEvent_def gapEstLinkReqEvent_t;

/**
 * GAP_LINK_PARAM_UPDATE_EVENT message format.  This message is sent to the app
 * when the connection parameters update request is complete.
 */
struct _gapLinkUpdateEvent_def
{
  gapEventHdr_t hdr;     		//!< GAP_MSG_EVENT and status
  uint16_t connectionHandle;  	//!< Connection handle of the update
  uint16_t connInterval;      	//!< Requested connection interval
  uint16_t connLatency;       	//!< Requested connection latency
  uint16_t connTimeout;       	//!< Requested connection timeout
} __attribute__((__packed__));
typedef struct _gapLinkUpdateEvent_def gapLinkUpdateEvent_t;

/**
 * GAP_LINK_TERMINATED_EVENT message format.  This message is sent to the
 * app when a link to a device is terminated.
 */
struct _gapTerminateLinkEvent_def
{
  gapEventHdr_t  hdr;   		//!< GAP_MSG_EVENT and status
  uint16_t connectionHandle; 	//!< connection Handle
  uint8_t reason;            	//!< termination reason from LL
} __attribute__((__packed__));
typedef struct _gapTerminateLinkEvent_def gapTerminateLinkEvent_t;

/*-------------------------------------------------------------------
 * TYPEDEFS - Authentication, Bounding and Pairing
 */

/**
 * GAP_PASSKEY_NEEDED_EVENT message format.  This message is sent to the
 * app when a Passkey is needed from the app's user interface.
 */
struct _gapPasskeyNeededEvent_def
{
  gapEventHdr_t  hdr;        		//!< GAP_MSG_EVENT and status
  uint8_t deviceAddr[B_ADDR_LEN]; 	//!< address of device to pair with, and could be either public or random.
  uint16_t connectionHandle;      	//!< Connection handle
  uint8_t uiInputs;               	//!< Pairing User Interface Inputs - Ask user to input passcode
  uint8_t uiOutputs;              	//!< Pairing User Interface Outputs - Display passcode
} __attribute__((__packed__));
typedef struct _gapPasskeyNeededEvent_def gapPasskeyNeededEvent_t;

/**
 * GAP_AUTHENTICATION_COMPLETE_EVENT message format.  This message is sent to the app
 * when the authentication request is complete.
 */
struct _gapAuthCompleteEvent_def
{
  gapEventHdr_t  hdr;           	//!< GAP_MSG_EVENT and status
  uint16_t connectionHandle;       	//!< Connection Handle from controller used to ref the device
  uint8_t authState;                //!< TRUE if the pairing was authenticated (MITM)
  smSecurityInfo_t *pSecurityInfo; 	//!< BOUND - security information from this device
  smIdentityInfo_t *pIdentityInfo; 	//!< BOUND - identity information
  smSigningInfo_t *pSigningInfo;   	//!< Signing information
  smSecurityInfo_t *pDevSecInfo;   	//!< BOUND - security information from connected device
} __attribute__((__packed__));
typedef struct _gapAuthCompleteEvent_def gapAuthCompleteEvent_t;

/**
 * securityInfo and identityInfo are only used if secReqs.bondable == BOUND, which means that
 * the device is already bound and we should use the security information and keys.
 */
struct _gapAuthParams_def
{
  uint16_t connectionHandle;      	//!< Connection Handle from controller,
  smLinkSecurityReq_t  secReqs; 	//!< Pairing Control info
} __attribute__((__packed__));
typedef struct _gapAuthParams_def gapAuthParams_t;

/**
 * GAP_SLAVE_REQUESTED_SECURITY_EVENT message format.  This message is sent to the app
 * when a Slave Security Request is received.
 */
struct _gapSlaveSecurityReqEvent_def
{
  gapEventHdr_t  hdr;        		//!< GAP_MSG_EVENT and status
  uint16_t connectionHandle;      	//!< Connection Handle
  uint8_t deviceAddr[B_ADDR_LEN]; 	//!< address of device requesting security
  uint8_t authReq;                	//!< Authentication Requirements: Bit 2: MITM, Bits 0-1: bonding (0 - no bonding, 1 - bonding
} __attribute__((__packed__));
typedef struct _gapSlaveSecurityReqEvent_def gapSlaveSecurityReqEvent_t;

/**
 * GAP_BOND_COMPLETE_EVENT message format.  This message is sent to the
 * app when a bonding is complete.  This means that a key is loaded and the link is encrypted.
 */
struct _gapBondCompleteEvent_def
{
  gapEventHdr_t  hdr;   			//!< GAP_MSG_EVENT and status
  uint16_t connectionHandle; 		//!< connection Handle
} __attribute__((__packed__));
typedef struct _gapBondCompleteEvent_def gapBondCompleteEvent_t;

/**
 * Pairing Request fields - the parsed fields of the SMP Pairing Request command.
 */
struct _gapPairingReq_def
{
  uint8_t ioCap;         //!< Pairing Request ioCap field
  uint8_t oobDataFlag;   //!< Pairing Request OOB Data Flag field
  uint8_t authReq;       //!< Pairing Request Auth Req field
  uint8_t maxEncKeySize; //!< Pairing Request Maximum Encryption Key Size field
  keyDist_t keyDist;   //!< Pairing Request Key Distribution field
} __attribute__((__packed__));
typedef struct _gapPairingReq_def gapPairingReq_t;

/**
 * GAP_PAIRING_REQ_EVENT message format.<BR>
 * <BR>
 * This message is sent to the
 * app when an unexpected Pairing Request is received.  The application is
 * expected to setup for a Security Manager pairing/bonding.<BR>
 * <BR>
 * To setup an SM Pairing, the application should call GAP_Authenticate() with these "pairReq" fields.<BR>
 * <BR>
* NOTE: This message should only be sent to peripheral devices.
 */
struct _gapPairingReqEvent_def
{
  gapEventHdr_t hdr;    		//!< GAP_MSG_EVENT and status
  uint16_t connectionHandle; 	//!< connection Handle
  gapPairingReq_t pairReq; 		//!< The Pairing Request fields received.
} __attribute__((__packed__));
typedef struct _gapPairingReqEvent_def gapPairingReqEvent_t;

/**
 * GAP Advertisement/Scan Response Data Token - These data items are stored as low byte first (OTA
 * format).  The data space for these items are passed in and maintained by
 * the calling application
 */
struct _gapAdvDataToken_def
{
  uint8_t adType;     //!< ADTYPE value: @ref GAP_ADTYPE_DEFINES
  uint8_t attrLen;    //!< Number of bytes in the attribute data
  uint8_t *pAttrData; //!< pointer to Attribute data
} __attribute__((__packed__));
typedef struct _gapAdvDataToken_def gapAdvDataToken_t;

union _gapEventMsg_def
{
	gapEventHdr_t					gapEventHdr;
	gapRandomAddrEvent_t			gapRandomAddrEvent;
	gapDeviceInitDoneEvent_t		gapDeviceInitDoneEvent;
	gapSignUpdateEvent_t			gapSignUpdateEvent;
	gapDeviceInfoEvent_t			gapDeviceInfoEvent;
	gapDevDiscEvent_t				gapDevDiscEvent;
	gapMakeDiscoverableRspEvent_t	gapMakeDiscoverableRspEvent;
	gapEndDiscoverableRspEvent_t	gapEndDiscoverableRspEvent;
	gapAdvDataUpdateEvent_t			gapAdvDataUpdateEvent;
	gapEstLinkReqEvent_t			gapEstLinkReqEvent;
	gapLinkUpdateEvent_t			gapLinkUpdateEvent;
	gapTerminateLinkEvent_t			gapTerminateLinkEvent;
	gapPasskeyNeededEvent_t			gapPasskeyNeededEvent;
	gapAuthCompleteEvent_t			gapAuthCompleteEvent;
	gapSlaveSecurityReqEvent_t		gapSlaveSecurityReqEvent;
	gapBondCompleteEvent_t			gapBondCompleteEvent;
	gapPairingReqEvent_t			gapPairingReqEvent;
} __attribute__((__packed__));
typedef union _gapEventMsg_def gapEventMsg_t;

/** @} End BLE_GAP_DEFINES */

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* GAP_H */
