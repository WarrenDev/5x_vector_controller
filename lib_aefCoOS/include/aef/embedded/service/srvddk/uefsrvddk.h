
/**
* uefsrvddk.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used for developing AEF system services.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_SRVDDK_UEFSRVDDK_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_SRVDDK_UEFSRVDDK_H_

#include <stdint.h>

/**
* Definitions of service types.  Note that values used by UniKey Technologies
* are in the range 0-32767, and 32768-65535 are reserved for use by customers.
*/

#define service_type_t	uint32_t

#define SERVICE_TYPE_CONSOLE				0x00000001
#define SERVICE_TYPE_WATCHDOG				0x00000002
#define SERVICE_TYPE_LOGGER					0x00000003
#define SERVICE_TYPE_DATABASE				0x00000004
#define SERVICE_TYPE_BLE					0x00000005
#define SERVICE_TYPE_WIEGAND				0x00000006
#define SERVICE_TYPE_RFID_EM4095			0x00000007
#define SERVICE_TYPE_RFID_HTRC110			0x00000008
#define SERVICE_TYPE_RFID_PN532				0x00000009
#define SERVICE_TYPE_PROPERTY_MANAGER		0x0000000A
#define SERVICE_TYPE_UI_MANAGER				0x0000000B
#define SERVICE_TYPE_CRYPTO_MANAGER			0x0000000C
#define	SERVICE_TYPE_OSDP					0x0000000D
#define SERVICE_TYPE_FIRMWARE_UPGRADE		0x0000000E
#define SERVICE_TYPE_DEVICE_COM				0x0000000F
#define SERVICE_TYPE_KEYPAD					0x00000010
#define SERVICE_TYPE_CERTIFICATE_MANAGER	0x00000011
#define SERVICE_TYPE_SECURE_CHANNEL			0x00000012
#define SERVICE_TYPE_NETWORK				0x00000013
#define SERVICE_TYPE_MQTT					0x00000014
#define SERVICE_TYPE_GPSD					0x00000015

/**
* Macro definition for defining service IOCTL function control codes.
* Note that function codes 0-2047 are reserved for UniKey Technologies,
* and 2048-4096 are reserved for customers.
*/
#define service_ioctl_code_t	uint32_t

#define SRVIOCTLCODE(ServiceType,Function,Method,Access) (          		\
    ((ServiceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

/*
* Method code definitions to indicate how buffers are passed for I/O controls
*/
#define SERVICE_METHOD_BUFFERED			0
#define SERVICE_METHOD_IN_DIRECT        1
#define SERVICE_METHOD_OUT_DIRECT       2
#define SERVICE_METHOD_DIRECT           3

/**
* Access check value definitions
*/
#define SERVICE_ANY_ACCESS       		0
#define SERVICE_SPECIAL_ACCESS    		(SERVICE_ANY_ACCESS)
#define SERVICE_READ_ACCESS          	( 0x0001 )    // file & pipe
#define SERVICE_WRITE_ACCESS         	( 0x0002 )    // file & pipe

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_SRVDDK_UEFSRVDDK_H_ */
