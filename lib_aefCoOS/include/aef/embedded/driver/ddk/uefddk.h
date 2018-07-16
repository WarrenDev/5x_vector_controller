
/**
* uefddk.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used for UEF device driver development.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_DDK_UEFDDK_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_DDK_UEFDDK_H_

#include <stdint.h>

/**
* Definitions of device types.  Note that values used by UniKey Technologies
* are in the range 0-32767, and 32768-65535 are reserved for use by customers.
*/

#define device_type_t	uint32_t

#define DEVICE_TYPE_UART                0x00000001
#define DEVICE_TYPE_I2C              	0x00000002
#define DEVICE_TYPE_SPI  				0x00000003
#define	DEVICE_TYPE_TIMER				0x00000004
#define DEVICE_TYPE_LED					0x00000005
#define	DEVICE_TYPE_RGBLED				0x00000006
#define	DEVICE_TYPE_BUZZER				0x00000007
#define DEVICE_TYPE_CONTROLLER          0x00000010
#define DEVICE_TYPE_DATALINK            0x00000011
#define	DEVICE_TYPE_NETWORK				0x00000012
#define DEVICE_TYPE_BLUETOOTH			0x00000013
#define	DEVICE_TYPE_THREAD				0x00000014
#define DEVICT_TYPE_DEVAPI				0x00000015
#define	DEVICE_TYPE_CONSOLE				0x00000016
#define DEVICE_TYPE_VIRTUAL_BLOCK		0x00000017
#define	DEVICE_TYPE_POINT_OF_SERVICE	0x00000018
#define	DEVICE_TYPE_CRYPTO_PROVIDER		0x00000019
#define DEVICE_TYPE_BATTERY				0x0000001a
#define DEVICE_TYPE_BUS_EXTENDER		0x0000001b
#define DEVICE_TYPE_TRANSPORT			0x0000001c
#define DEVICE_TYPE_AUDIO				0x0000001d
#define DEVICE_TYPE_FILE_SYSTEM			0x0000001e
#define DEVICE_TYPE_NAMED_PIPE			0x0000001f
#define DEVICE_TYPE_VIRTUAL_DISK		0x00000020
#define DEVICE_TYPE_MASS_STORAGE		0x00000021
#define DEVICE_TYPE_MESH_NETWORK		0x00000022
#define	DEVICE_TYPE_WIEGAND				0x00000023
#define DEVICE_TYPE_EM4095				0x00000024
#define DEVICE_TYPE_HTRC110				0x00000025
#define DEVICE_TYPE_WATCHDOG			0x00000026
#define DEVICE_TYPE_ILI9341				0x00000027
#define DEVICE_TYPE_CS43L22				0x00000028
#define DEVICE_TYPE_MP45DT02			0x00000029
#define DEVICE_TYPE_MPU9250				0x0000002A
#define DEVICE_TYPE_B605B				0x0000002B

/**
* Macro definition for defining IOCTL function control codes.
* Note that function codes 0-2047 are reserved for UniKey Technologies,
* and 2048-4096 are reserved for customers.
*/

#define device_ioctl_code_t	uint32_t
#define device_type_t uint32_t

#define DEVIOCTLCODE(DeviceType,Function,Method,Access) (          		\
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) 	\
)

/*
* Method code definitions to indicate how buffers are passed for I/O controls
*/

#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_DIRECT                   3

/**
* Access check value definitions
*/

#define DEVICE_ANY_ACCESS       		0
#define DEVICE_SPECIAL_ACCESS    		(DEVICE_ANY_ACCESS)
#define DEVICE_READ_ACCESS          	( 0x0001 )    // file & pipe
#define DEVICE_WRITE_ACCESS         	( 0x0002 )    // file & pipe

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_DDK_UEFDDK_H_ */
