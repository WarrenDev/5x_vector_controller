
/**
* hci_com.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of BLE HCI communications.
*
*/

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <CoOS.h>
#include "hci_com.h"

#define HIBYTE(w)					((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#define LOBYTE(w)					((uint8_t)(w))
#define MAKEWORD(low,hi)			((uint16_t)(((uint8_t)(low))|(((uint16_t)((uint8_t)(hi)))<<8)))
#define HIWORD(w)					((uint16_t)(((uint32_t)(w) >> 16) & 0xFFFF))
#define LOWORD(w)					((uint16_t)(w))
#define MAKELONG(low,hi)			((uint32_t)(((uint16_t)(low))|(((uint32_t)((uint16_t)(hi)))<<16)))
#define LLSB(x)             		((x) & 0xFF)
#define LNLSB(x)            		(((x) >> 8) & 0xFF)
#define LNMSB(x)            		(((x) >> 16) & 0xFF)
#define LMSB(x)             		(((x) >> 24) & 0xFF)

static const stream_driver_vtable_t* hci_uart_drv = NULL;
static stream_driver_ctx_t* hci_uart_ctx = NULL;
static uint8_t	transmit_buffer[BLEHCICOM_TXBUFFER_SIZE];

static uint32_t	hci_put_packet			( uint8_t* packet, uint32_t size );
static uint32_t	hci_create_packet		( uint8_t* pPacket, uint16_t uiopcode, uint8_t ucparmSize, uint8_t* pparms );
static uint32_t	hci_create_event_packet ( uint8_t* pPacket, uint8_t ucEventCode, uint8_t ucparmSize, uint8_t* pparms );
static uint32_t	hci_create_acl_packet 	( uint8_t* pPacket, uint16_t uiConnHandle, uint8_t ucPBFlag, uint8_t ucBCFlag, uint16_t uiDataSize, uint8_t* pData );
static uint32_t	hci_create_sco_packet	( uint8_t* pPacket, uint16_t uiConnHandle, uint8_t ucDataSize, uint8_t* pData );

////////////////////////////////////////////////////////////
// Initialize
void hci_com_initialize ( const stream_driver_vtable_t* drv, stream_driver_ctx_t* ctx )
{
	hci_uart_drv = drv;
	hci_uart_ctx = ctx;
} // Initialize

////////////////////////////////////////////////////////////
// HciExtUtil_Reset
uint32_t HciExtUtil_Reset ( uint8_t ucResetType )
{

	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_UTIL_SUBGRP << 7) + HCI_EXT_UTIL_RESET;
	uint8_t  parm   = ucResetType;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0x01, &parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtUtil_Reset

////////////////////////////////////////////////////////////
// HciExtUtil_NVRead
uint32_t HciExtUtil_NVRead ( uint8_t ucId, uint8_t ucLength )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_UTIL_SUBGRP << 7) + HCI_EXT_UTIL_NV_READ;
	parm[0] = ucId;
	parm[1] = ucLength;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtUtil_NVRead

////////////////////////////////////////////////////////////
// HciExtUtil_NVRead
uint32_t HciExtUtil_NVWrite ( uint8_t ucId, uint8_t ucLength, uint8_t* pData )
{

	uint8_t parm[2+ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_UTIL_SUBGRP << 7) + HCI_EXT_UTIL_NV_WRITE;
	parm[0] = ucId;
	parm[1] = ucLength;
	memcpy ( &parm[2], pData, ucLength );
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtUtil_NVWrite

////////////////////////////////////////////////////////////
// HciExtUtil_Reset
uint32_t HciExtUtil_ForceBoot ( void )
{

	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_UTIL_SUBGRP << 7) + HCI_EXT_UTIL_FORCE_BOOT;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0x00, NULL );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtUtil_ForceBoot

////////////////////////////////////////////////////////////
// HciExtUtil_SetRxGainCmd
uint32_t HciExt_SetRxGainCmd ( uint8_t ucRxGain )
{

	uint8_t  parm   = ucRxGain;
	uint16_t opcode = HCI_EXT_SET_RX_GAIN;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0x01, &parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExt_SetRxGainCmd

////////////////////////////////////////////////////////////
// HciExt_ExtendRFRangeCmd
uint32_t HciExt_ExtendRFRangeCmd ( void )
{

	uint16_t opcode = HCI_EXT_EXTEND_RF_RANGE;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0x00, NULL );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExt_ExtendRFRangeCmd

////////////////////////////////////////////////////////////
// HciExt_ReadBuildRevision
uint32_t HciExt_ReadBuildRevision ( void )
{

	uint8_t  parm[3];
	uint16_t opcode = HCI_EXT_BUILD_REVISION;
	parm[0] = 0x01;								// Read build revision
	parm[1] = 0x00;								// Ignored
	parm[2] = 0x00;								// Ignored
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExt_ReadBuildRevision

////////////////////////////////////////////////////////////
// HciExt_DelaySleep (1.4.0)
uint32_t HciExt_DelaySleep ( uint16_t usDelay )
{

	uint8_t  parm[2];
	uint16_t opcode = HCI_EXT_DELAY_SLEEP;
	parm[0] = LOBYTE(usDelay);
	parm[1] = HIBYTE(usDelay);
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExt_DelaySleep

////////////////////////////////////////////////////////////
// HciExt_ResetSystem (1.4.0)
//   Issue a system reset. 0 - hard reset; 1 - soft reset
uint32_t HciExt_ResetSystem ( uint8_t ucMode )
{

	uint8_t  parm   = ucMode;
	uint16_t opcode = HCI_EXT_RESET_SYSTEM;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 1, &parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExt_ResetSystem

////////////////////////////////////////////////////////////
// HciExt_OverlappedProcessing (1.4.0)
//   Enable/disable overlapped processing.  0 - disable; 1 - enable
uint32_t HciExt_OverlappedProcessing ( uint8_t ucMode )
{

	uint8_t  parm   = ucMode;
	uint16_t opcode = HCI_EXT_OVERLAPPED_PROCESSING;
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 1, &parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExt_OverlappedProcessing

////////////////////////////////////////////////////////////
// HciExtGAP_DeviceInit
uint32_t HciExtGAP_DeviceInit ( uint8_t ucProfile, uint8_t ucMaxScanResponses, uint8_t* pIRK, uint8_t* pSRK, uint32_t ulSignCounter )
{

	uint8_t parm[38];
	uint8_t ucOffset = (KEYLEN*2)+2;
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_DEVICE_INIT;

	memset ( parm, '\0', sizeof(parm) );
	parm[0] = ucProfile;
	parm[1] = ucMaxScanResponses;

	if ( pIRK )
		memcpy ( &parm[2], pIRK, KEYLEN );
	if ( pSRK )
		memcpy ( &parm[2+KEYLEN], pSRK, KEYLEN );

	parm[ucOffset++] = LLSB(ulSignCounter);
	parm[ucOffset++] = LNLSB(ulSignCounter);
	parm[ucOffset++] = LNMSB(ulSignCounter);
	parm[ucOffset++] = LMSB(ulSignCounter);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_DeviceInit

////////////////////////////////////////////////////////////
// HciExtGAP_ConfigDeviceAddr
uint32_t HciExtGAP_ConfigDeviceAddr ( uint8_t ucAddrType, uint8_t* pStaticAddr )
{

	uint8_t parm[1 + HCI_BDADDR_LEN];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_CONFIG_DEVICE_ADDR;

	memset ( parm, '\0', sizeof(parm) );
	parm[0] = ucAddrType;

	if ( pStaticAddr )
		memcpy ( &parm[1], pStaticAddr, HCI_BDADDR_LEN );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_ConfigDeviceAddr

////////////////////////////////////////////////////////////
// HciExtGAP_DeviceDiscReq
uint32_t HciExtGAP_DeviceDiscReq ( uint8_t ucMode, uint8_t ucActiveScan, uint8_t ucWhiteList )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_DEVICE_DISC_REQ;

	parm[0] = ucMode;
	parm[1] = ucActiveScan;
	parm[2] = ucWhiteList;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_DeviceDiscReq

////////////////////////////////////////////////////////////
// HciExtGAP_DeviceDiscCancel
uint32_t HciExtGAP_DeviceDiscCancel ( void )
{

	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_DEVICE_DISC_CANCEL;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0, NULL );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_DeviceDiscCancel

////////////////////////////////////////////////////////////
// HciExtGAP_MakeDiscoverable
uint32_t HciExtGAP_MakeDiscoverable ( uint8_t ucEventType, uint8_t ucInitiatorAddrType, uint8_t* pInitiatorAddr,
		                                         uint8_t ucChannelMap, uint8_t ucFilterPolicy )
{

	uint8_t parm[10];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_MAKE_DISCOVERABLE;

	parm[0] = ucEventType;
	parm[1] = ucInitiatorAddrType;
	memcpy ( &parm[2], pInitiatorAddr, HCI_BDADDR_LEN );
	parm[8] = ucChannelMap;
	parm[9] = ucFilterPolicy;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_MakeDiscoverable

////////////////////////////////////////////////////////////
// HciExtGAP_UpdateAdvertData
uint32_t HciExtGAP_UpdateAdvertData ( uint8_t ucAdType, uint8_t ucLength, uint8_t* pAdvertData )
{

	uint8_t parm[2 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_UPDATE_ADV_DATA;

	parm[0] = ucAdType;
	parm[1] = ucLength;
	memcpy ( &parm[2], pAdvertData, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_UpdateAdvertData

////////////////////////////////////////////////////////////
// HciExtGAP_EndDiscovery
uint32_t HciExtGAP_EndDiscovery ( void )
{

	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_END_DISC;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0, NULL );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_EndDiscovery

////////////////////////////////////////////////////////////
// HciExtGAP_EstLinkReq
uint32_t HciExtGAP_EstLinkReq ( uint8_t ucHighDutyCycle, uint8_t ucWhiteList, uint8_t ucAddrType, uint8_t* pPeerAddr )
{

	uint8_t parm[3 + HCI_BDADDR_LEN];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_EST_LINK_REQ;

	parm[0] = ucHighDutyCycle;
	parm[1] = ucWhiteList;
	parm[2] = ucAddrType;
	memcpy ( &parm[3], pPeerAddr, HCI_BDADDR_LEN );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_EstLinkReq

////////////////////////////////////////////////////////////
// HciExtGAP_TerminateLink
uint32_t HciExtGAP_TerminateLink ( uint16_t usConnectionHandle )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_TERMINATE_LINK;

	parm[0] = LOBYTE(usConnectionHandle);
	parm[1] = HIBYTE(usConnectionHandle);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_TerminateLInk

////////////////////////////////////////////////////////////
// HciExtGAP_TerminateLinkEx- TI BLE 1.4.0
uint32_t HciExtGAP_TerminateLinkEx ( uint16_t usConnectionHandle, uint8_t ucReason )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_TERMINATE_LINK;

	parm[0] = LOBYTE(usConnectionHandle);
	parm[1] = HIBYTE(usConnectionHandle);
	parm[2] = ucReason;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_TerminateLInkEx

////////////////////////////////////////////////////////////
// HciExtGAP_Authenticate
uint32_t HciExtGAP_Authenticate ( gapAuthParams_t *pParams, gapPairingReq_t *pPairReq )
{

	uint8_t parm[sizeof(gapAuthParams_t) + sizeof(gapPairingReq_t)];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_AUTHENTICATE;

	memcpy ( parm, pParams, sizeof(gapAuthParams_t) );
	memcpy ( &parm[sizeof(gapAuthParams_t)], pPairReq, sizeof(gapPairingReq_t) );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_Authenticate

////////////////////////////////////////////////////////////
// HciExtGAP_PasskeyUpdate
uint32_t HciExtGAP_PasskeyUpdate ( uint16_t usConnectionHandle, uint8_t* pPasskey )
{

	uint8_t parm[8];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_PASSKEY_UPDATE;

	parm[0] = LOBYTE(usConnectionHandle);
	parm[1] = HIBYTE(usConnectionHandle);

	memcpy ( &parm[2], pPasskey, 6 );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_PasskeyUpdate

////////////////////////////////////////////////////////////
// HciExtGAP_SlaveSecurityUpdateReq
uint32_t HciExtGAP_SlaveSecurityUpdateReq ( uint16_t usConnectionHandle, uint8_t ucAuthReq )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_SLAVE_SECURITY_REQ_UPDATE;

	parm[0] = LOBYTE(usConnectionHandle);
	parm[1] = HIBYTE(usConnectionHandle);
	parm[2] = ucAuthReq;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_SlaveSecurityUpdateReq

////////////////////////////////////////////////////////////
// HciExtGAP_Signable
uint32_t HciExtGAP_Signable ( uint16_t usConnectionHandle, uint8_t ucAuthenticated,
		                                 uint8_t* pKey, uint32_t ulSignCounter )
{

	uint8_t parm[23];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_SIGNABLE;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = ucAuthenticated;
	memcpy ( &parm[3], pKey, KEYLEN );
	parm[19] = LLSB(ulSignCounter);
	parm[20] = LNLSB(ulSignCounter);
	parm[21] = LNMSB(ulSignCounter);
	parm[22] = LMSB(ulSignCounter);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_Signable

////////////////////////////////////////////////////////////
// HciExtGAP_Bond
uint32_t HciExtGAP_Bond ( uint16_t usConnectionHandle, uint8_t ucAuthenticated, uint8_t* pLtk,
		                  uint16_t usDiversifier, uint8_t* pRandomNum, uint8_t ucKeySize )
{

	uint8_t parm[30];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_BOND;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = ucAuthenticated;
	memcpy ( &parm[3], pLtk, KEYLEN );
	parm[19] = LOBYTE(usDiversifier);
	parm[20] = HIBYTE(usDiversifier);
	memcpy ( &parm[21], pRandomNum, B_RANDOM_NUM_SIZE );
	parm[29] = ucKeySize;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_Bond

////////////////////////////////////////////////////////////
// HciExtGAP_TerminateAuth
uint32_t HciExtGAP_Terminate_Auth ( uint16_t usConnectionHandle, uint8_t ucReason )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_TERMINATE_AUTH;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = ucReason;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_Terminate_Auth

////////////////////////////////////////////////////////////
// HciExtGAP_UpdateLinkparmReq
uint32_t HciExtGAP_UpdateLinkparmReq ( uint16_t usConnectionHandle, uint16_t usIntervalMin, uint16_t usIntervalMax,
		                               uint16_t usConnLatency, uint16_t usConnTimeout )
{

	uint8_t parm[10];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_UPDATE_LINK_PARAM_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usIntervalMin);
	parm[3]  = HIBYTE(usIntervalMin);
	parm[4]  = LOBYTE(usIntervalMax);
	parm[5]  = HIBYTE(usIntervalMax);
	parm[6]  = LOBYTE(usConnLatency);
	parm[7]  = HIBYTE(usConnLatency);
	parm[8]  = LOBYTE(usConnTimeout);
	parm[9]  = HIBYTE(usConnTimeout);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_UpdateLinkparmReq

////////////////////////////////////////////////////////////
// HciExtGAP_Setparm
uint32_t HciExtGAP_Setparm ( uint16_t usparmId, uint16_t usparmValue )
{

	uint8_t parm[4];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_SET_PARAM;

	parm[0]  = LOBYTE(usparmId);
	parm[1]  = HIBYTE(usparmId);
	parm[2]  = LOBYTE(usparmValue);
	parm[3]  = HIBYTE(usparmValue);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_Setparm

////////////////////////////////////////////////////////////
// HciExtGAP_Getparm
uint32_t HciExtGAP_Getparm ( uint16_t usparmId )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_GET_PARAM;

	parm[0]  = LOBYTE(usparmId);
	parm[1]  = HIBYTE(usparmId);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_Getparm

////////////////////////////////////////////////////////////
// HciExtGAP_ResolvePrivateAddr
uint32_t HciExtGAP_ResolvePrivateAddr ( uint8_t* pIRK, uint8_t* pAddr )
{

	uint8_t parm[22];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_RESOLVE_PRIVATE_ADDR;

	memcpy ( &parm[0], pIRK, KEYLEN );
	memcpy ( &parm[KEYLEN], pAddr, B_ADDR_LEN );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_ResolvePrivateAddr

////////////////////////////////////////////////////////////
// HciExtGAP_SetAdvertisingToken
uint32_t HciExtGAP_SetAdvertisingToken ( uint8_t ucAdType, uint8_t ucAttrLen, uint8_t* pAttr )
{

	uint8_t parm[2 + ucAttrLen];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_SET_ADV_TOKEN;

	parm[0] = ucAdType;
	parm[1] = ucAttrLen;
	memcpy ( &parm[2], pAttr, ucAttrLen );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_SetAdvertisingToken

////////////////////////////////////////////////////////////
// HciExtGAP_RemoveAdvertisingToken
uint32_t HciExtGAP_RemoveAdvertisingToken ( uint8_t ucToken )
{

	uint8_t parm;
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_REMOVE_ADV_TOKEN;

	parm = ucToken;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), &parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_RemoveAdvertisingToken

////////////////////////////////////////////////////////////
// HciExtGAP_UpdateAdvertisingTokens
uint32_t HciExtGAP_UpdateAdvertisingTokens ( void )
{

	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_UPDATE_ADV_TOKENS;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, 0, NULL );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_UpdateAdvertisingTokens

////////////////////////////////////////////////////////////
// HciExtGAP_BondSetparm
uint32_t HciExtGAP_BondSetparm ( uint16_t usParam, uint8_t ucLength, uint8_t* pValue )
{

	uint8_t parm[3+ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_BOND_SET_PARAM;

	parm[0]  = LOBYTE(usParam);
	parm[1]  = HIBYTE(usParam);
	parm[2]  = ucLength;
	memcpy ( &parm[3], pValue, ucLength );
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_BondSetparm

////////////////////////////////////////////////////////////
// HciExtGAP_BondGetparm
uint32_t HciExtGAP_BondGetparm ( uint16_t usParam )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_BOND_GET_PARAM;

	parm[0]  = LOBYTE(usParam);
	parm[1]  = HIBYTE(usParam);
	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_BondGetparm

////////////////////////////////////////////////////////////
// HciExtGAP_BondServiceChange
uint32_t HciExtGAP_BondServiceChange ( uint16_t usConnectionHandle, uint8_t ucSetparm )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GAP_SUBGRP << 7) + HCI_EXT_GAP_BOND_SERVICE_CHANGE;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = ucSetparm;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGAP_BondGetparm

////////////////////////////////////////////////////////////
// HciExtGATT_ExchangeMTR
uint32_t HciExtGATT_ExchangeMTU ( uint16_t usClientMTU )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_EXCHANGE_MTU_REQ;

	parm[0]  = LOBYTE(usClientMTU);
	parm[1]  = HIBYTE(usClientMTU);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ExchangeMTU

////////////////////////////////////////////////////////////
// HciExtGATT_FindInfoReq
uint32_t HciExtGATT_FindInfoReq ( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle )
{

	uint8_t parm[6];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_FIND_INFO_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usStartHandle);
	parm[3]  = HIBYTE(usStartHandle);
	parm[4]  = LOBYTE(usEndHandle);
	parm[5]  = HIBYTE(usEndHandle);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_FindInfoReq

////////////////////////////////////////////////////////////
// HciExtGATT_AddService
uint32_t HciExtGATT_AddService ( uint16_t usUUID, uint16_t usNumOfAttrs )
{

	uint8_t parm[4];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + HCI_EXT_GATT_ADD_SERVICE;

	parm[0]  = LOBYTE(usUUID);
	parm[1]  = HIBYTE(usUUID);
	parm[2]  = LOBYTE(usNumOfAttrs);
	parm[3]  = HIBYTE(usNumOfAttrs);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_AddService

////////////////////////////////////////////////////////////
// HciExtGATT_DelService
uint32_t HciExtGATT_DelService ( uint16_t usUUID )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + HCI_EXT_GATT_DEL_SERVICE;

	parm[0]  = LOBYTE(usUUID);
	parm[1]  = HIBYTE(usUUID);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_DelService

////////////////////////////////////////////////////////////
// HciExtGATT_AddAttribute
uint32_t HciExtGATT_AddAttribute ( uint16_t usUUID, uint8_t ucPermissions )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + HCI_EXT_GATT_ADD_ATTRIBUTE;

	parm[0]  = LOBYTE(usUUID);
	parm[1]  = HIBYTE(usUUID);
	parm[2]  = ucPermissions;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_AddAttribute

////////////////////////////////////////////////////////////
// HciExtGATT_AddAttribute
uint32_t HciExtGATT_AddAttribute128 ( uint8_t* pAttrUUID, uint8_t ucPermissions )
{

	uint8_t parm[17];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + HCI_EXT_GATT_ADD_ATTRIBUTE;

	memcpy ( &parm[0], pAttrUUID, 16 );
	parm[16] = ucPermissions;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_AddAttribute128

////////////////////////////////////////////////////////////
// HciExtGATT_FindIncludedServices
uint32_t HciExtGATT_FindIncludedServices ( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle )
{

	uint8_t parm[6];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_FIND_INCLUDED_SERVICES;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usStartHandle);
	parm[3]  = HIBYTE(usStartHandle);
	parm[4]  = LOBYTE(usEndHandle);
	parm[5]  = HIBYTE(usEndHandle);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_FindIncludedServices

////////////////////////////////////////////////////////////
// HciExtGATT_FindPrimaryServiceByUUID
uint32_t HciExtGATT_FindPrimaryServiceByUUID ( uint16_t usConnectionHandle, uint8_t* pServiceUUID, uint8_t ucLength )
{

	uint8_t parm[8 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_FIND_BY_TYPE_VALUE_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(DEFAULT_START_HANDLE);
	parm[3]  = HIBYTE(DEFAULT_START_HANDLE);
	parm[4]  = LOBYTE(DEFAULT_END_HANDLE);
	parm[5]  = HIBYTE(DEFAULT_END_HANDLE);
	parm[6]  = LOBYTE(GATT_PRIMARY_SERVICE_UUID);
	parm[7]  = HIBYTE(GATT_PRIMARY_SERVICE_UUID);
	memcpy ( &parm[8], pServiceUUID, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGatt_FindPrimaryServiceByUUID

////////////////////////////////////////////////////////////
// HciExtGATT_FindByTypeValueRsp
uint32_t HciExtGATT_FindByTypeValueRsp ( uint16_t usConnectionHandle, attHandlesInfo_t* pHandleInfo, uint8_t ucCount )
{

	uint8_t parm[2 + (sizeof(attHandlesInfo_t) * ucCount)];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_FIND_BY_TYPE_VALUE_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	memcpy ( &parm[2], pHandleInfo, (sizeof(attHandlesInfo_t) * ucCount) );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGatt_FindByTypeValueRsp

////////////////////////////////////////////////////////////
// HciExtGATT_DiscoverAllChars
uint32_t HciExtGATT_DiscoverAllChars ( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle )
{

	uint8_t parm[8];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BY_TYPE_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usStartHandle);
	parm[3]  = HIBYTE(usStartHandle);
	parm[4]  = LOBYTE(usEndHandle);
	parm[5]  = HIBYTE(usEndHandle);
	parm[6]  = LOBYTE(GATT_CHARACTER_UUID);
	parm[7]  = HIBYTE(GATT_CHARACTER_UUID);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_DiscoverAllChars

////////////////////////////////////////////////////////////
// HciExtGATT_ReadByTypeRsp
uint32_t HciExtGATT_ReadByTypeRsp ( uint16_t usConnectionHandle, uint8_t ucLength, attCharDesc_t* pCharDesc, uint8_t ucCount )
{

	uint8_t parm[3 + (sizeof(attCharDesc_t) * ucCount)];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BY_TYPE_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = ucLength;
	memcpy ( &parm[3], pCharDesc, (sizeof(attCharDesc_t) * ucCount) );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGatt_ReadByTypeRsp

////////////////////////////////////////////////////////////
// HciExtGATT_ReadCharValue
uint32_t HciExtGATT_ReadCharValue ( uint16_t usConnectionHandle, uint16_t usAttrHandle )
{

	uint8_t parm[4];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadCharValue

////////////////////////////////////////////////////////////
// HciExtGATT_ReadCharValueRsp
uint32_t HciExtGATT_ReadCharValueRsp ( uint16_t usConnectionHandle, uint8_t* pValue, uint8_t ucLength )
{

	uint8_t parm[2 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	memcpy ( &parm[2], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadCharValueRsp

////////////////////////////////////////////////////////////
// HciExtGATT_ReadBlobCharValue
uint32_t HciExtGATT_ReadBlobCharValue ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset )
{

	uint8_t parm[6];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BLOB_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	parm[4]  = LOBYTE(usOffset);
	parm[5]  = HIBYTE(usOffset);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadBlobCharValue

////////////////////////////////////////////////////////////
// HciExtGATT_ReadBlobCharValueRsp
uint32_t HciExtGATT_ReadBlobCharValueRsp ( uint16_t usConnectionHandle, uint8_t* pValue, uint8_t ucLength )
{

	uint8_t parm[2 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_READ_BLOB_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	memcpy ( &parm[2], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadBlobCharValueRsp

////////////////////////////////////////////////////////////
// HciExtGATT_ReadUsingCharUUID
uint32_t HciExtGATT_ReadUsingCharUUID ( uint16_t usConnectionHandle, uint16_t usStartHandle, uint16_t usEndHandle,
												   uint8_t ucLength, uint8_t* pUUID )
{

	uint8_t parm[6 + sizeof(attReadByTypeReq_t)];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_READ_USING_CHAR_UUID;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usStartHandle);
	parm[3]  = HIBYTE(usStartHandle);
	parm[4]  = LOBYTE(usEndHandle);
	parm[5]  = HIBYTE(usEndHandle);
	parm[6]  = ucLength;
	memcpy ( &parm[7], pUUID, ATT_UUID_SIZE );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadUsingCharUUID

////////////////////////////////////////////////////////////
// HciExtGATT_WriteNoRsp
uint32_t HciExtGATT_WriteNoRsp ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength,
								 uint8_t* pValue )
{

	uint8_t parm[4 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_WRITE_NO_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	memcpy ( &parm[4], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_WriteNoRsp

////////////////////////////////////////////////////////////
// HciExtGATT_SignedWriteNoRsp
uint32_t HciExtGATT_SignedWriteNoRsp ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength,
											      uint8_t* pValue )
{

	uint8_t parm[4 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_SIGNED_WRITE_NO_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	memcpy ( &parm[4], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_SignedWriteNoRsp

////////////////////////////////////////////////////////////
// HciExtGATT_WriteCharValue
uint32_t HciExtGATT_WriteCharValue ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength,
								     uint8_t* pValue )
{

	uint8_t parm[6 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_WRITE_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = 0x00;	// Signature
	parm[3]  = 0x00;	// Command
	parm[4]  = LOBYTE(usAttrHandle);
	parm[5]  = HIBYTE(usAttrHandle);
	memcpy ( &parm[6], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_WriteCharValue

////////////////////////////////////////////////////////////
// HciExtGATT_WriteCharValueRsp
uint32_t HciExtGATT_WriteCharValueRsp ( uint16_t usConnectionHandle )
{

	uint8_t parm[2];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_WRITE_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_WriteCharValueRsp

////////////////////////////////////////////////////////////
// HciExtGATT_PrepareWriteReq
uint32_t HciExtGATT_PrepareWriteReq ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset, uint16_t usLength,
									  uint8_t* pValue )
{

	uint8_t parm[6 + usLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_PREPARE_WRITE_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	parm[4]  = LOBYTE(usOffset);
	parm[5]  = HIBYTE(usOffset);
	memcpy ( &parm[6], pValue, usLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_PrepareWriteReq

////////////////////////////////////////////////////////////
// HciExtGATT_ExecuteWriteReq
uint32_t HciExtGATT_ExecuteWriteReq ( uint16_t usConnectionHandle, uint8_t ucFlags )
{

	uint8_t parm[3];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_EXECUTE_WRITE_REQ;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = ucFlags;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ExecuteWriteReq

////////////////////////////////////////////////////////////
// HciExtGATT_ReadCharDesc
uint32_t HciExtGATT_ReadCharDesc ( uint16_t usConnectionHandle, uint16_t usAttrHandle )
{

	uint8_t parm[4];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_READ_CHAR_DESC;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadCharDesc

////////////////////////////////////////////////////////////
// HciExtGATT_ReadLongCharDesc
uint32_t HciExtGATT_ReadLongCharDesc ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset )
{

	uint8_t parm[6];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_READ_LONG_CHAR_DESC;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	parm[4]  = LOBYTE(usOffset);
	parm[5]  = HIBYTE(usOffset);

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_ReadLongCharDesc

////////////////////////////////////////////////////////////
// HciExtGATT_WriteCharDesc
uint32_t HciExtGATT_WriteCharDesc ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint8_t ucLength,
									uint8_t* pValue )
{

	uint8_t parm[4 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_WRITE_CHAR_DESC;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	memcpy ( &parm[4], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_WriteCharDesc

////////////////////////////////////////////////////////////
// HciExtGATT_WriteLongCharDesc
uint32_t HciExtGATT_WriteLongCharDesc ( uint16_t usConnectionHandle, uint16_t usAttrHandle, uint16_t usOffset,
										uint8_t ucLength, uint8_t* pValue )
{

	uint8_t parm[6 + ucLength];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_GATT_SUBGRP << 7) + GATT_WRITE_LONG_CHAR_DESC;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = LOBYTE(usAttrHandle);
	parm[3]  = HIBYTE(usAttrHandle);
	parm[4]  = LOBYTE(usOffset);
	parm[5]  = HIBYTE(usOffset);
	memcpy ( &parm[6], pValue, ucLength );

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	return hci_put_packet ( transmit_buffer, packet_length );

} // HciExtGATT_WriteLongCharDesc

////////////////////////////////////////////////////////////
// HciExtATT_ErrorRsp
uint32_t HciExtATT_ErrorRsp ( uint16_t usConnectionHandle, uint8_t reqOpcode, uint16_t usHandle, uint8_t errCode )
{

	uint8_t parm[6];
	uint16_t opcode = (HCI_VS_OGF << 10) + (HCI_EXT_ATT_SUBGRP << 7) + ATT_ERROR_RSP;

	parm[0]  = LOBYTE(usConnectionHandle);
	parm[1]  = HIBYTE(usConnectionHandle);
	parm[2]  = reqOpcode;
	parm[3]  = LOBYTE(usHandle);
	parm[4]  = HIBYTE(usHandle);
	parm[5]  = errCode;

	uint32_t packet_length = hci_create_packet ( transmit_buffer, opcode, sizeof(parm), parm );

	hci_put_packet ( transmit_buffer, packet_length );

	return packet_length;

} // HciExtATT_ErrorRsp

////////////////////////////////////////////////////////////
// get_packet
//   Retrieve HCI packets from the HCI network processor
uint32_t get_packet ( uint8_t* packet )
{
uint8_t* pBuffer = packet;
uint16_t HeaderSize = 0;
uint16_t PayloadSize = 0;
uint32_t bytes_read;

	////////////////////////////////////////////////////////
	// Retrieve the HCI packet type
//	*pBuffer++ = uart_getchar ();
	hci_uart_drv->read (hci_uart_ctx, pBuffer, 1, &bytes_read);
	pBuffer++;

	////////////////////////////////////////////////////////
	// Retrieve the balance of the header based on packet type
	switch ( packet[0] )
	{

		case HCI_CMD_PACKET:
			hci_uart_drv->read (hci_uart_ctx, pBuffer, (uint32_t)(HCI_CMD_MIN_LENGTH-1), &bytes_read);
			pBuffer += bytes_read;
			HeaderSize = HCI_CMD_MIN_LENGTH;
			PayloadSize = (uint16_t) packet[3];
			break;
		case HCI_ACL_DATA_PACKET:
		case HCI_SCO_DATA_PACKET:
			hci_uart_drv->read (hci_uart_ctx, pBuffer, (uint32_t)(HCI_DATA_MIN_LENGTH-1), &bytes_read);
			pBuffer += bytes_read;
			HeaderSize = HCI_DATA_MIN_LENGTH;
			PayloadSize = MAKEWORD(packet[3],packet[4]);
			if ( PayloadSize >= (BLEHCICOM_RXBUFFER_SIZE-HCI_DATA_MIN_LENGTH) )
			{
				PayloadSize = 0;
				HeaderSize  = 0;
			}
			break;
		case HCI_EVENT_PACKET:
			hci_uart_drv->read (hci_uart_ctx, pBuffer, (uint32_t)(HCI_EVENT_MIN_LENGTH-1), &bytes_read);
			pBuffer += bytes_read;
			HeaderSize = HCI_EVENT_MIN_LENGTH;
			PayloadSize = (uint16_t) packet[2];
			break;
		default:
			break;

	} // switch

	if ( PayloadSize )
	{
		hci_uart_drv->read (hci_uart_ctx, pBuffer, (uint32_t)PayloadSize, &bytes_read);
	}

	return ( (uint32_t)(PayloadSize + HeaderSize) );

} // CRPUCon::GetPacket

////////////////////////////////////////////////////////////
// hci_put_packet
//   Send a BLE HCI packet to the network controller
uint32_t hci_put_packet ( uint8_t* packet, uint32_t size )
{

	////////////////////////////////////////////////////////
	// Serialize write access to the network controller

	uint32_t bytes_written = 0L;

	if ( hci_uart_drv )
	{
		hci_uart_drv->write(hci_uart_ctx, packet, size, &bytes_written );
	}

	return bytes_written;

} // hci_put_packet

////////////////////////////////////////////////////////////
// get_response
//   Retrieve the last HCI response message
uint32_t get_response ( void* response )
{

	uint32_t status = 0L;
	do
	{
		hci_uart_drv->iocontrol(hci_uart_ctx, IOCTL_UART_CHARPRESENT, NULL, 0, NULL, 0, &status);
	} while (status == 0L);

	return get_packet ( (uint8_t*)response );

} // GetResponse

////////////////////////////////////////////////////////////
// hci_create_packet
//   Create a HCI command packet
uint32_t hci_create_packet ( uint8_t* pPacket, uint16_t uiopcode, uint8_t ucparmSize, uint8_t* pparms )
{
	uint8_t* pPtr = pPacket;

	*pPtr++ = HCI_CMD_PACKET;
	*pPtr++ = LOBYTE(uiopcode);
	*pPtr++ = HIBYTE(uiopcode);
	*pPtr++ = ucparmSize;
	if ( ucparmSize )
		memcpy ( pPtr, pparms, ucparmSize );

	return ( HCI_CMD_MIN_LENGTH + ucparmSize );

} // hci_create_packet

////////////////////////////////////////////////////////////
// hci_create_packet
//   Create a HCI event packet
uint32_t hci_create_event_packet	( uint8_t* pPacket, uint8_t ucEventCode, uint8_t ucparmSize, uint8_t* pparms )
{
	uint8_t* pPtr = pPacket;

	*pPtr++ = HCI_EVENT_PACKET;
	*pPtr++ = ucEventCode;
	*pPtr++ = ucparmSize;
	if ( ucparmSize )
		memcpy ( pPtr, pparms, ucparmSize );

	return ( HCI_EVENT_MIN_LENGTH + ucparmSize );

} // hci_create_packet

////////////////////////////////////////////////////////////
// hci_create_acl_packet
//   Create a HCI ACL data packet
uint32_t hci_create_acl_packet ( uint8_t* pPacket, uint16_t uiConnHandle, uint8_t ucPBFlag, uint8_t ucBCFlag, uint16_t uiDataSize, uint8_t* pData )
{
	uint8_t* pPtr = pPacket;
	uint8_t  ucFlags   = ((ucPBFlag << 4) | (ucBCFlag << 6));
	uint16_t uiChandle = (ucFlags << 8) | (uiConnHandle & 0x0FFF);

	*pPtr++ = HCI_ACL_DATA_PACKET;
	*pPtr++ = LOBYTE(uiChandle);
	*pPtr++ = HIBYTE(uiChandle);
	*pPtr++ = LOBYTE(uiDataSize);
	*pPtr++ = HIBYTE(uiDataSize);
	if ( uiDataSize )
		memcpy ( pPtr, pData, uiDataSize );

	return ( HCI_DATA_MIN_LENGTH + uiDataSize );

} // hci_create_acl_packet

////////////////////////////////////////////////////////////
// hci_create_sco_packet
//   Create a HCI SCO data packet
uint32_t hci_create_sco_packet ( uint8_t* pPacket, uint16_t uiConnHandle, uint8_t ucDataSize, uint8_t* pData )
{
	uint8_t* pPtr = pPacket;

	*pPtr++ = HCI_SCO_DATA_PACKET;
	*pPtr++ = LOBYTE(uiConnHandle);
	*pPtr++ = HIBYTE(uiConnHandle);
	*pPtr++ = ucDataSize;
	memcpy ( pPtr, pData, ucDataSize );

	return ( HCI_CMD_MIN_LENGTH + ucDataSize );

} // hci_create_sco_packet

