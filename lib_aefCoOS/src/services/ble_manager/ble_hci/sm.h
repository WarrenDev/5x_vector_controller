
/**
  @headerfile:    sm.h
  $Date: 2012-10-29 13:32:07 -0700 (Mon, 29 Oct 2012) $
  $Revision: 31951 $

  @mainpage BLE SM API

  This file contains the interface to the SM.


  Copyright 2009 - 2012 Texas Instruments Incorporated. All rights reserved.

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

#ifndef SM_H
#define SM_H

#ifdef __cplusplus
extern "C"
{
#endif

/*-------------------------------------------------------------------
 * INCLUDES
 */
#include "bcomdef.h"

/*-------------------------------------------------------------------
 * MACROS
 */

/*-------------------------------------------------------------------
 * CONSTANTS
 */
/** @defgroup SM_IO_CAP_DEFINES SM I/O Capabilities
 * @{
 */
#define DISPLAY_ONLY              		0x00  //!< Display Only Device
#define DISPLAY_YES_NO            		0x01  //!< Display and Yes and No Capable
#define KEYBOARD_ONLY             		0x02  //!< Keyboard Only
#define NO_INPUT_NO_OUTPUT        		0x03  //!< No Display or Input Device
#define KEYBOARD_DISPLAY          		0x04  //!< Both Keyboard and Display Capable
/** @} End SM_IO_CAP_DEFINES */

#define SM_AUTH_MITM_MASK(a)    (((a) & 0x04) >> 2)

/** @defgroup SM_PASSKEY_TYPE_DEFINES SM Passkey Types (Bit Masks)
 * @{
 */
#define SM_PASSKEY_TYPE_INPUT   		0x01    //!< Input the passkey
#define SM_PASSKEY_TYPE_DISPLAY 		0x02    //!< Display the passkey
/** @} End SM_PASSKEY_TYPE_DEFINES */

/** @defgroup SM_BONDING_FLAGS_DEFINES SM AuthReq Bonding Flags
 * Bonding flags 0x02 and 0x03 are reserved.
 * @{
 */
#define SM_AUTH_REQ_NO_BONDING    		0x00  //!< No bonding
#define SM_AUTH_REQ_BONDING       		0x01  //!< Bonding
/** @} End SM_BONDING_FLAGS_DEFINES */

#define PASSKEY_LEN     				6   //! Passkey Character Length (ASCII Characters)

#define SM_AUTH_STATE_AUTHENTICATED		0x04  //! Authenticate requested
#define SM_AUTH_STATE_BONDING          	0x01  //! Bonding requested

/*-------------------------------------------------------------------
 * General TYPEDEFS
 */

/**
 * SM_NEW_RAND_KEY_EVENT message format.  This message is sent to the
 * requesting task.
 */
//typedef struct
//{
//  osal_event_hdr_t  hdr;      //!< SM_NEW_RAND_KEY_EVENT and status
//  uint8_t newKey[KEYLEN];       //!< New key value - if status is SUCCESS
//} smNewRandKeyEvent_t;

/**
 * Key Distribution field  - True or False fields
 */
struct _keyDist_def
{
  unsigned int sEncKey:1;    //!< Set to distribute slave encryption key
  unsigned int sIdKey:1;     //!< Set to distribute slave identity key
  unsigned int sSign:1;      //!< Set to distribute slave signing key
  unsigned int mEncKey:1;    //!< Set to distribute master encryption key
  unsigned int mIdKey:1;     //!< Set to distribute master identity key
  unsigned int mSign:1;      //!< Set to distribute master signing key
  unsigned int reserved:2;   //!< Reserved - not to be used
} __attribute__((__packed__));
typedef struct _keyDist_def keyDist_t;

/**
 * Link Security Requirements
 */
struct _smLinkSecurityReq_def
{
  uint8_t ioCaps;               //!< I/O Capabilities (ie.
  uint8_t oobAvailable;         //!< True if Out-of-band key available
  uint8_t oob[KEYLEN];          //!< Out-Of-Bounds key
  uint8_t authReq;              //!< Authentication Requirements
  uint8_t maxEncKeySize;        //!< Maximum Encryption Key size (7-16 bytes)
  keyDist_t keyDist;          //!< Key Distribution mask
} __attribute__((__packed__));
typedef struct _smLinkSecurityReq_def smLinkSecurityReq_t;

/**
 * Link Security Information
 */
struct _smSecurityInfo_def
{
  uint8_t ltk[KEYLEN];              //!< Long Term Key (LTK)
  uint16_t div;                     //!< LTK Diversifier
  uint8_t rand[B_RANDOM_NUM_SIZE];  //!< LTK random number
  uint8_t keySize;                  //!< LTK Key Size (7-16 bytes)
} __attribute__((__packed__));
typedef struct _smSecurityInfo_def smSecurityInfo_t;

/**
 * Link Identity Information
 */
struct _smIdentityInfo_def
{
  uint8_t irk[KEYLEN];          //!< Identity Resolving Key (IRK)
  uint8_t bd_addr[B_ADDR_LEN];  //!< The advertiser may set this to zeroes to not disclose its BD_ADDR (public address).
} __attribute__((__packed__));
typedef struct _smIdentityInfo_def smIdentityInfo_t;

/**
 * Signing Information
 */
struct _smSigningInfo_def
{
  uint8_t  srk[KEYLEN]; //!< Signature Resolving Key (CSRK)
  uint32_t signCounter; //!< Sign Counter
} __attribute__((__packed__));
typedef struct _smSigningInfo_def smSigningInfo_t;

/**
 * Pairing Request & Response - authReq field
 */
struct _authReq_def
{
  unsigned int bonding:2;    //!< Bonding flags
  unsigned int mitm:1;       //!< Man-In-The-Middle (MITM)
  unsigned int reserved:5;   //!< Reserved - don't use
} __attribute__((__packed__));
typedef struct _authReq_def authReq_t;

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* SM_H */
