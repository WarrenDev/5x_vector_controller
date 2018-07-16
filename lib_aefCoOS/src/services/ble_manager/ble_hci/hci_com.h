
/**
* hci_com.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definitions used with BLE HCI communications.
*
*/

#ifndef SRC_SERVICES_BLE_MANAGER_BLE_HCI_HCI_COM_H_
#define SRC_SERVICES_BLE_MANAGER_BLE_HCI_HCI_COM_H_

#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/driver/uart/uart_driver.h>

#include "hci_ti.h"
#include "gatt.h"
#include "gap.h"
#include "att.h"
#include "gatt_uuid.h"

/**
 * Misc. identifier definitions
 */
#define BLEHCICOM_XMITTIMEOUT		250L					// Transmit timeout (ms)
#define BLEHCICOM_RECVTIMEOUT		500L					// Receive timeout (ms)
#define BLEHCICOM_INITIALSIZE		3						// Initial size
#define	BLEHCICOM_TXBUFFER_SIZE		HCI_MAX_CMD_PKT_SIZE	// Transmit buffer size
#define BLEHCICOM_RXBUFFER_SIZE		ATT_MAX_MTU_SIZE		// Receive buffer size

#define DEFAULT_START_HANDLE		0x0001
#define DEFAULT_END_HANDLE			0xFFFF

/**
 * Misc. identifier definitions
 */
#define	HCI_VS_OGF					0x3F

/**
 * HCI Extension Commands
 */

/**
 * The 10-bit OCF (Opcode Command Field) of the HCI Opcode is further
 * devided into two subsections:
 * - Subgroup (3 bits):
 *   - 0 (LL)
 *   - 1 (L2CAP)
 *   - 2 (ATT)
 *   - 3 (GATT)
 *   - 4 (GAP)
 *   - 5 (UTIL)
 *   - 6 (Reserved)
 *   - 7 (User Profile)
 * - Command (7 bits) or Profile (7 bits) if Subgroup value is set to
 *   User Profile (i.e., all ones) in which case, another octet is
 *   required to represent user profile commands.
 */
#define HCI_EXT_LL_SUBGRP                     	0x00
#define HCI_EXT_L2CAP_SUBGRP                  	0x01
#define HCI_EXT_ATT_SUBGRP                    	0x02
#define HCI_EXT_GATT_SUBGRP                   	0x03
#define HCI_EXT_GAP_SUBGRP                    	0x04
#define HCI_EXT_UTIL_SUBGRP                   	0x05
#define HCI_EXT_PROFILE_SUBGRP                	0x07

#define HCI_EXT_UTIL_RESET                    	0x00
#define HCI_EXT_UTIL_NV_READ                  	0x01
#define HCI_EXT_UTIL_NV_WRITE                 	0x02
#define HCI_EXT_UTIL_FORCE_BOOT               	0x03

/*8
 * GAP Initialization and Configuration
 */
#define HCI_EXT_GAP_DEVICE_INIT               	0x00
#define HCI_EXT_GAP_CONFIG_DEVICE_ADDR        	0x03

/**
 * GAP Device Discovery
 */
#define HCI_EXT_GAP_DEVICE_DISC_REQ           	0x04
#define HCI_EXT_GAP_DEVICE_DISC_CANCEL        	0x05
#define HCI_EXT_GAP_MAKE_DISCOVERABLE         	0x06
#define HCI_EXT_GAP_UPDATE_ADV_DATA           	0x07
#define HCI_EXT_GAP_END_DISC                  	0x08

/**
 * GAP Link Establishment
 */
#define HCI_EXT_GAP_EST_LINK_REQ              	0x09
#define HCI_EXT_GAP_TERMINATE_LINK            	0x0A
#define HCI_EXT_GAP_AUTHENTICATE              	0x0B
#define HCI_EXT_GAP_PASSKEY_UPDATE            	0x0C
#define HCI_EXT_GAP_SLAVE_SECURITY_REQ_UPDATE 	0x0D
#define HCI_EXT_GAP_SIGNABLE                  	0x0E
#define HCI_EXT_GAP_BOND                      	0x0F
#define HCI_EXT_GAP_TERMINATE_AUTH            	0x10
#define HCI_EXT_GAP_UPDATE_LINK_PARAM_REQ     	0x11

/**
 * GAP Parameters
 */
#define HCI_EXT_GAP_SET_PARAM                 	0x30
#define HCI_EXT_GAP_GET_PARAM                 	0x31
#define HCI_EXT_GAP_RESOLVE_PRIVATE_ADDR      	0x32
#define HCI_EXT_GAP_SET_ADV_TOKEN             	0x33
#define HCI_EXT_GAP_REMOVE_ADV_TOKEN          	0x34
#define HCI_EXT_GAP_UPDATE_ADV_TOKENS         	0x35
#define HCI_EXT_GAP_BOND_SET_PARAM            	0x36
#define HCI_EXT_GAP_BOND_GET_PARAM            	0x37
#define HCI_EXT_GAP_BOND_SERVICE_CHANGE       	0x38

/**
 * GATT Sub-Procedure Commands
 */
#define GATT_FIND_INCLUDED_SERVICES           	0x30
#define GATT_DISC_ALL_CHARS                   	0x32
#define GATT_READ_USING_CHAR_UUID             	0x34
#define GATT_WRITE_NO_RSP                     	0x36
#define GATT_SIGNED_WRITE_NO_RSP              	0x38
#define GATT_RELIABLE_WRITES                  	0x3a
#define GATT_READ_CHAR_DESC                   	0x3c
#define GATT_READ_LONG_CHAR_DESC              	0x3e
#define GATT_WRITE_CHAR_DESC                  	0x40
#define GATT_WRITE_LONG_CHAR_DESC         		0x42

/**
 * GATT HCI Extension messages (0x7C - 0x7F)
 */
#define HCI_EXT_GATT_ADD_SERVICE              ( GATT_BASE_METHOD | 0x3C ) // 0x7C
#define HCI_EXT_GATT_DEL_SERVICE              ( GATT_BASE_METHOD | 0x3D ) // 0x7D
#define HCI_EXT_GATT_ADD_ATTRIBUTE            ( GATT_BASE_METHOD | 0x3E ) // 0x7E

/**
 * HCI Extension Events
 */

/**
 * HCI extension events must start from 0x0400. The upper 6 bits of all
 * zeros is reserved for the HCI extension embedded commands. The rest of
 * the event bits should follow the HCI extension command format (i.e.,
 * 3-bit Subgroup + 7-bit Event).
 */
#define HCI_EXT_BASE_EVENT                    ( 0x0001 << 10 )  // 0x0400

#define HCI_EXT_LL_EVENT                      ( HCI_EXT_BASE_EVENT | (HCI_EXT_LL_SUBGRP << 7) )    // 0x0400
#define HCI_EXT_L2CAP_EVENT                   ( HCI_EXT_BASE_EVENT | (HCI_EXT_L2CAP_SUBGRP << 7) ) // 0x0480
#define HCI_EXT_ATT_EVENT                     ( HCI_EXT_BASE_EVENT | (HCI_EXT_ATT_SUBGRP << 7) )   // 0x0500
#define HCI_EXT_GATT_EVENT                    ( HCI_EXT_BASE_EVENT | (HCI_EXT_GATT_SUBGRP << 7) )  // 0x0580
#define HCI_EXT_GAP_EVENT                     ( HCI_EXT_BASE_EVENT | (HCI_EXT_GAP_SUBGRP << 7) )   // 0x0600

/**
 * GAP Events
 */
#define HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT          ( HCI_EXT_GAP_EVENT | 0x00 )
#define HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT          ( HCI_EXT_GAP_EVENT | 0x01 )
#define HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_EVENT      ( HCI_EXT_GAP_EVENT | 0x02 )
#define HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT    ( HCI_EXT_GAP_EVENT | 0x03 )
#define HCI_EXT_GAP_END_DISCOVERABLE_DONE_EVENT     ( HCI_EXT_GAP_EVENT | 0x04 )
#define HCI_EXT_GAP_LINK_ESTABLISHED_EVENT          ( HCI_EXT_GAP_EVENT | 0x05 )
#define HCI_EXT_GAP_LINK_TERMINATED_EVENT           ( HCI_EXT_GAP_EVENT | 0x06 )
#define HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT         ( HCI_EXT_GAP_EVENT | 0x07 )
#define HCI_EXT_GAP_RANDOM_ADDR_CHANGED_EVENT       ( HCI_EXT_GAP_EVENT | 0x08 )
#define HCI_EXT_GAP_SIGNATURE_UPDATED_EVENT         ( HCI_EXT_GAP_EVENT | 0x09 )
#define HCI_EXT_GAP_AUTH_COMPLETE_EVENT             ( HCI_EXT_GAP_EVENT | 0x0A )
#define HCI_EXT_GAP_PASSKEY_NEEDED_EVENT            ( HCI_EXT_GAP_EVENT | 0x0B )
#define HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_EVENT  ( HCI_EXT_GAP_EVENT | 0x0C )
#define HCI_EXT_GAP_DEVICE_INFO_EVENT               ( HCI_EXT_GAP_EVENT | 0x0D )
#define HCI_EXT_GAP_BOND_COMPLETE_EVENT             ( HCI_EXT_GAP_EVENT | 0x0E )
#define HCI_EXT_GAP_PAIRING_REQ_EVENT               ( HCI_EXT_GAP_EVENT | 0x0F )

#define HCI_EXT_GAP_CMD_STATUS_EVENT                ( HCI_EXT_GAP_EVENT | 0x7F )

/**
 * GATT / ATT Events
 */
#define HCI_EXT_ATT_ERROR_RSP						( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_ERROR_RSP )
#define HCI_EXT_ATT_EXCHANGEMTU_RSP					( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_EXCHANGE_MTU_RSP )
#define HCI_EXT_ATT_FINDINFO_RSP					( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_FIND_INFO_RSP )
#define HCI_EXT_ATT_FINDINFOBYTYPEVALUE_RSP			( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_FIND_BY_TYPE_VALUE_RSP )
#define HCI_EXT_ATT_BEADBYTYPE_RSP					( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BY_TYPE_RSP )
#define HCI_EXT_ATT_READ_RSP						( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_RSP )
#define HCI_EXT_ATT_READBLOB_RSP					( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BLOB_RSP )
#define HCI_EXT_ATT_READMULTI_RSP					( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_MULTI_RSP )
#define HCI_EXT_ATT_READBYGRPTYPE_RSP				( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BY_GRP_TYPE_RSP )
#define HCI_EXT_ATT_WRITE_RSP						( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_WRITE_RSP )
#define HCI_EXT_ATT_PREPAREWRITE_RSP				( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_PREPARE_WRITE_RSP )
#define HCI_EXT_ATT_EXECUTEWRITE_RSP				( (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_EXECUTE_WRITE_RSP )

struct _hciExtCmd
{
  uint8_t  pktType;
  uint16_t opCode;
  uint8_t  len;
  uint8_t  *pData;
} __attribute__((__packed__));
typedef struct _hciExtCmd hciExtCmd_t;

struct _hciExtEvent
{
  uint8_t  pktType;
  uint8_t  eventCode;
  uint8_t  len;
  uint8_t  *pData;
} __attribute__((__packed__));
typedef struct _hciExtEvent hciExtEvent_t;

struct _hciExtData
{
  uint8_t   pktType;
  uint16_t  connectionHandle;
  uint8_t   len;
  uint8_t   *pData;
} __attribute__((__packed__));
typedef struct _hciExtData hciExtData_t;

union _hciExtMsg_def
{
	hciExtCmd_t		hciCmd;
	hciExtEvent_t	hciEvent;
	hciExtData_t	hciData;
	uint8_t			ucBuffer[BLEHCICOM_RXBUFFER_SIZE];
} __attribute__((__packed__));
typedef union _hciExtMsg_def hciExtMsg_t;

/**
 * Packet layout -
 *
 *   Command Packet
 * --------------------------------------------------------------------------
 * ||  Packet Type   |  OpCode   |  Parameter Length  |    Parameter(s)    ||
 * ||    8 bits      |  16 bits  |       8 bits       |  Parameter Length  ||
 * --------------------------------------------------------------------------
 *
 *   Event Packet
 * -------------------------------------------------------------------------------
 * ||  Packet Type   |  Event Code  |  Parameter Length  |  Event Parameter(s)  ||
 * ||    8 bits      |    8 bits    |       8 bits       |   Parameter Length   ||
 * -------------------------------------------------------------------------------
 *
 *   Data Packet
 * ------------------------------------------------------------------------------------------
 * ||  Packet Type   |  Connection Handle  |  Flags PB|BC  |  Data Length  |     Data      ||
 * ||    8 bits      |       12 bits       |     4 bits    |    16 bits    |  Data Length  ||
 * ------------------------------------------------------------------------------------------
 */

void 	 hci_com_initialize 					( const stream_driver_vtable_t* drv, stream_driver_ctx_t* ctx );
uint32_t hci_get_packet							( uint8_t* packet );
uint32_t hci_get_response						( void* response );

/*
 * BLE HCI Extension commands API
 */
uint32_t HciExtUtil_Reset 						( uint8_t ucResetType );
uint32_t HciExtUtil_NVRead 						( uint8_t ucId, uint8_t ucLength );
uint32_t HciExtUtil_NVWrite 					( uint8_t ucId, uint8_t ucLength, uint8_t* pData );
uint32_t HciExtUtil_ForceBoot 					( void );

uint32_t HciExt_ExtendRFRangeCmd 				( void );
uint32_t HciExt_SetRxGainCmd 					( uint8_t ucRxGain );
uint32_t HciExt_ReadBuildRevision 				( void );
uint32_t HciExt_DelaySleep 						( uint16_t usDelay );
uint32_t HciExt_ResetSystem 					( uint8_t ucMode );
uint32_t HciExt_OverlappedProcessing 			( uint8_t ucMode );

uint32_t HciExtGAP_DeviceInit 					( uint8_t ucProfile, uint8_t ucMaxScanResponses, uint8_t* pIRK, uint8_t* pSRK, uint32_t ulSignCounter );
uint32_t HciExtGAP_ConfigDeviceAddr				( uint8_t ucAddrType, uint8_t* pStaticAddr );
uint32_t HciExtGAP_DeviceDiscReq 				( uint8_t ucMode, uint8_t ucActiveScan, uint8_t ucWhiteList );
uint32_t HciExtGAP_DeviceDiscCancel 			( void );
uint32_t HciExtGAP_MakeDiscoverable 			( uint8_t ucEventType, uint8_t ucInitiatorAddrType, uint8_t* pInitiatorAddr,
		                              	  	      uint8_t ucChannelMap, uint8_t ucFilterPolicy );
uint32_t HciExtGAP_UpdateAdvertData 			( uint8_t ucAdType, uint8_t ucLength, uint8_t* pAdvertData );
uint32_t HciExtGAP_EndDiscovery 				( void );
uint32_t HciExtGAP_EstLinkReq 					( uint8_t ucHighDutyCycle, uint8_t ucWhiteList, uint8_t ucAddrType, uint8_t* pPeerAddr );
uint32_t HciExtGAP_TerminateLink 				( uint16_t usConnectionHandle );
uint32_t HciExtGAP_TerminateLinkEx 				( uint16_t usConnectionHandle, uint8_t ucReason );
uint32_t HciExtGAP_Authenticate 				( gapAuthParams_t *pParams, gapPairingReq_t *pPairReq );
uint32_t HciExtGAP_PasskeyUpdate	 			( uint16_t usConnectionHandle, uint8_t* pPasskey );
uint32_t HciExtGAP_SlaveSecurityUpdateReq		( uint16_t usConnectionHandle, uint8_t ucAuthReq );
uint32_t HciExtGAP_Signable 					( uint16_t usConnectionHandle, uint8_t ucAuthenticated, uint8_t* pKey, uint32_t ulSignCounter );
uint32_t HciExtGAP_Bond 						( uint16_t usConnectionHandle, uint8_t ucAuthenticated, uint8_t* pLtk, uint16_t usDiversifier,
												  uint8_t* pRandomNum, uint8_t ucKeySize );
uint32_t HciExtGAP_Terminate_Auth 				( uint16_t usConnectionHandle, uint8_t ucReason );
uint32_t HciExtGAP_UpdateLinkParmReq 			( uint16_t usConnectionHandle, uint16_t usIntervalMin, uint16_t usIntervalMax,
		                                          uint16_t usConnLatency, uint16_t usConnTimeout );
uint32_t HciExtGAP_SetParm 						( uint16_t usParmId, uint16_t usParmValue );
uint32_t HciExtGAP_GetParm 						( uint16_t usParmId );
uint32_t HciExtGAP_ResolvePrivateAddr 			( uint8_t* pIRK, uint8_t* pAddr );
uint32_t HciExtGAP_SetAdvertisingToken 			( uint8_t ucAdType, uint8_t ucAttrLen, uint8_t* pAttr );
uint32_t HciExtGAP_RemoveAdvertisingToken 		( uint8_t ucToken );
uint32_t HciExtGAP_UpdateAdvertisingTokens 		( void );
uint32_t HciExtGAP_BondSetParm 					( uint16_t usParam, uint8_t ucLength, uint8_t* pValue );
uint32_t HciExtGAP_BondGetParm 					( uint16_t usParam );
uint32_t HciExtGAP_BondServiceChange 			( uint16_t usConnectionHandle, uint8_t ucSetParm );

uint32_t HciExtGATT_ExchangeMTU					( uint16_t usClientMTU );
uint32_t HciExtGATT_FindInfoReq 				( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle );
uint32_t HciExtGATT_AddService 					( uint16_t usUUID, uint16_t usNumOfAttrs );
uint32_t HciExtGATT_DelService 					( uint16_t usUUID );
uint32_t HciExtGATT_AddAttribute 				( uint16_t usUUID, uint8_t ucPermissions );
uint32_t HciExtGATT_AddAttribute128				( uint8_t* pAttrUUID, uint8_t ucPermissions );

uint32_t HciExtGATT_FindIncludedServices 		( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle );
uint32_t HciExtGATT_FindPrimaryServiceByUUID 	( uint16_t usConnectionHandle, uint8_t* pServiceUUID, uint8_t ucLength );
uint32_t HciExtGATT_FindByTypeValueRsp 			( uint16_t usConnectionHandle, attHandlesInfo_t* pHandleInfo, uint8_t ucCount );
uint32_t HciExtGATT_DiscoverAllChars 			( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle );
uint32_t HciExtGATT_ReadByTypeRsp 				( uint16_t usConnectionHandle, uint8_t ucLength, attCharDesc_t* pCharDesc, uint8_t ucCount );
uint32_t HciExtGATT_ReadCharValue 				( uint16_t usConnectionHandle, uint16_t usAttrHandle );
uint32_t HciExtGATT_ReadCharValueRsp 			( uint16_t usConnectionHandle, uint8_t* pValue, uint8_t ucLength );
uint32_t HciExtGATT_ReadBlobCharValue 			( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset );
uint32_t HciExtGATT_ReadBlobCharValueRsp 		( uint16_t usConnectionHandle, uint8_t* pValue, uint8_t ucLength );
uint32_t HciExtGATT_ReadUsingCharUUID 			( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle, uint8_t ucLength, uint8_t* pUUID );
uint32_t HciExtGATT_WriteNoRsp 					( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength, uint8_t* pValue );
uint32_t HciExtGATT_SignedWriteNoRsp 			( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength, uint8_t* pValue );
uint32_t HciExtGATT_WriteCharValue 				( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength, uint8_t* pValue );
uint32_t HciExtGATT_WriteCharValueRsp 			( uint16_t usConnectionHandle );
uint32_t HciExtGATT_PrepareWriteReq 			( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset, uint16_t usLength, uint8_t* pValue );
uint32_t HciExtGATT_ExecuteWriteReq 			( uint16_t usConnectionHandle, uint8_t ucFlags );
uint32_t HciExtGATT_ReadCharDesc 				( uint16_t usConnectionHandle, uint16_t usAttrHandle );
uint32_t HciExtGATT_ReadLongCharDesc 			( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset );
uint32_t HciExtGATT_WriteCharDesc 				( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength, uint8_t* pValue );
uint32_t HciExtGATT_WriteLongCharDesc 			( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset, uint8_t ucLength, uint8_t* pValue );

uint32_t HciExtATT_ErrorRsp						( uint16_t usConnectionHandle, uint8_t reqOpcode, uint16_t usHandle, uint8_t errCode );

#endif /* SRC_SERVICES_BLE_MANAGER_BLE_HCI_HCI_COM_H_ */
