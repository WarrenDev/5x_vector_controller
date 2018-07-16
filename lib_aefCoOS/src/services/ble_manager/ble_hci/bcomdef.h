/*
 * bcomdef.h
 *
 * Revised:        $Date: 2012-11-27 14:14:39 -0800 (Tue, 27 Nov 2012) $
 * Revision:       $Revision: 32324 $
 *
 * Description:    Type definitions and macros for BLE stack.
 *
 *
 * Copyright 2009 - 2012 Texas Instruments Incorporated. All rights reserved.
 *
 *  Created on: Apr 17, 2014
 *      Author: Albert Warren
 *      Imported from the TI BLE stack code
 *
 */

#ifndef BCOMDEF_H_
#define BCOMDEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * TYPEDEFS
 */

// Generic Status return
typedef uint8_t Status_t;

#if !defined ( MAX_NUM_LL_CONN )
  #if ( CTRL_CONFIG & INIT_CFG )
    #define MAX_NUM_LL_CONN                       3
  #elif ( !( CTRL_CONFIG & INIT_CFG ) && ( CTRL_CONFIG & ADV_CONN_CFG ) )
    #define MAX_NUM_LL_CONN                       1
  #else // no connection needed
    #define MAX_NUM_LL_CONN                       0
  #endif // CTRL_CONFIG=INIT_CFG
#endif // !MAX_NUM_LL_CONN

/** @defgroup BLE_COMMON_DEFINES BLE Common Defines
 * @{
 */
//! Default Public and Random Address Length
#define B_ADDR_LEN                                6

//! Default key length
#define KEYLEN                                    16

//! BLE Channel Map length
#define B_CHANNEL_MAP_LEN                         5

//! BLE Event mask length
#define B_EVENT_MASK_LEN                          8

//! BLE Local Name length
#define B_LOCAL_NAME_LEN                          248

//! BLE Maximum Advertising Packet Length
#define B_MAX_ADV_LEN                             31

//! BLE Random Number Size
#define B_RANDOM_NUM_SIZE                         8

//! BLE Feature Supported length
#define B_FEATURE_SUPPORT_LENGTH                  8

/** @defgroup BLE_STATUS_VALUES BLE Default BLE Status Values
 * returned as bStatus_t
 * @{
 */
#define bleSuccess							0x00			//!< Success
#define bleNotReady                     	0x10  			//!< Not ready to perform task
#define bleAlreadyInRequestedMode       	0x11  			//!< Already performing that task
#define bleIncorrectMode                	0x12  			//!< Not setup properly to perform that task
#define bleMemAllocError                	0x13  			//!< Memory allocation error occurred
#define bleNotConnected                 	0x14  			//!< Can't perform function when not in a connection
#define bleNoResources                  	0x15  			//!< There are no resource available
#define blePending                      	0x16  			//!< Waiting
#define bleTimeout                      	0x17  			//!< Timed out performing function
#define bleInvalidRange                 	0x18  			//!< A parameter is out of range
#define bleLinkEncrypted                	0x19  			//!< The link is already encrypted
#define bleProcedureComplete            	0x1A  			//!< The Procedure is completed

// GAP Status Return Values - returned as bStatus_t
#define bleGAPUserCanceled              	0x30  			//!< The user canceled the task
#define bleGAPConnNotAcceptable         	0x31  			//!< The connection was not accepted
#define bleGAPBondRejected              	0x32  			//!< The bound information was rejected.

// ATT Status Return Values - returned as bStatus_t
#define bleInvalidPDU                   	0x40  			//!< The attribute PDU is invalid
#define bleInsufficientAuthen           	0x41  			//!< The attribute has insufficient authentication
#define bleInsufficientEncrypt          	0x42  			//!< The attribute has insufficient encryption
#define bleInsufficientKeySize          	0x43  			//!< The attribute has insufficient encryption key size

// L2CAP Status Return Values - returned as bStatus_t

/** @} End BLE_STATUS_VALUES */

/** @defgroup BLE_NV_IDS BLE Non-volatile IDs
 * @{
 */
// Device NV Items -    Range 0 - 0x1F
#define BLE_NVID_IRK                    	0x02  			//!< The Device's IRK
#define BLE_NVID_CSRK                   	0x03  			//!< The Device's CSRK
#define BLE_NVID_SIGNCOUNTER            	0x04  			//!< The Device's Sign Counter

// Bonding NV Items -   Range  0x20 - 0x5F    - This allows for 10 bondings
#define BLE_NVID_GAP_BOND_START         	0x20  			//!< Start of the GAP Bond Manager's NV IDs
#define BLE_NVID_GAP_BOND_END           	0x5f  			//!< End of the GAP Bond Manager's NV IDs Range

// GATT Configuration NV Items - Range  0x70 - 0x79 - This must match the number of Bonding entries
#define BLE_NVID_GATT_CFG_START         	0x70  			//!< Start of the GATT Configuration NV IDs
#define BLE_NVID_GATT_CFG_END           	0x79  			//!< End of the GATT Configuration NV IDs
/** @} End BLE_NV_IDS */

/*********************************************************************
 * BLE OSAL GAP GLOBAL Events
 */
#define GAP_EVENT_SIGN_COUNTER_CHANGED  	0x4000  		//!< The device level sign counter changed


/** @defgroup BLE_MSG_IDS BLE OSAL Message ID Events
 *      Reserved Message ID Event Values:<BR>
 *        0xC0 - Key Presses<BR>
 *        0xE0 to 0xFC - App<BR>
 * @{
 */
// GAP - Messages IDs (0xD0 - 0xDF)
#define GAP_MSG_EVENT                         0xD0 			//!< Incoming GAP message

// SM - Messages IDs (0xC1 - 0xCF)
#define SM_NEW_RAND_KEY_EVENT                 0xC1 			//!< New Rand Key Event message

// GATT - Messages IDs (0xB0 - 0xBF)
#define GATT_MSG_EVENT                        0xB0 			//!< Incoming GATT message
#define GATT_SERV_MSG_EVENT                   0xB1 			//!< Incoming GATT Serv App message

// L2CAP - Messages IDs (0xA0 - 0xAF)
#define L2CAP_DATA_EVENT                      0xA0 			//!< Incoming data on a channel
#define L2CAP_SIGNAL_EVENT                    0xA2 			//!< Incoming Signaling message

// HCI - Messages IDs (0x90 - 0x9F)
#define HCI_DATA_EVENT                        0x90 			//!< HCI Data Event message
#define HCI_GAP_EVENT_EVENT                   0x91 			//!< GAP Event message
#define HCI_SMP_EVENT_EVENT                   0x92 			//!< SMP Event message
#define HCI_EXT_CMD_EVENT                     0x93 			//!< HCI Extended Command Event message
/** @} End BLE_MSG_IDS */

/*********************************************************************
 * TYPEDEFS
 */

  //! BLE Generic Status return: @ref BLE_STATUS_VALUES
typedef Status_t bStatus_t;

/** @} End GAP_MSG_EVENT_DEFINES */

#ifdef __cplusplus
}
#endif

#endif /* BCOMDEF_H_ */
