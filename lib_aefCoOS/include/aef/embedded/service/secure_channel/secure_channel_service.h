
/**
* secure_channel_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system secure channel service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_SECURE_CHANNEL_SECURE_CHANNEL_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_SECURE_CHANNEL_SECURE_CHANNEL_SERVICE_H_

#include <aef/embedded/service/srvddk/uefsrvddk.h>

/**
* The IOCTL function codes from 0x800 to 0xFFF are for customer use.
*
* The format of I/O control codes for the iocontrol call:
*    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method)
*
* Configuring system service I/O control codes in this manner produces
* unique system-wide I/O control codes.  It also provides a mechanism for
* isolating kernel mode memory spaces from user mode memory spaces and
* allows us to use the memory protection unit (MPU) of the processor.
*/

/*
* Secure channel base function codes
*/
#define SECURE_CHANNEL_INITIALIZE	0x800
#define SECURE_CHANNEL_RESET		0x801
#define SECURE_CHANNEL_OPEN			0x802
#define SECURE_CHANNEL_CLOSE		0x803
#define SECURE_CHANNEL_READ			0x804
#define SECURE_CHANNEL_WRITE		0x805
#define SECURE_CHANNEL_SYNC			0x806

/*
* Secure channel service I/O Control codes
*/
#define IOCTL_SECURE_CHANNEL_INITIALIZE	SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_SECURE_CHANNEL_RESET		SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_RESET,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_SECURE_CHANNEL_OPEN		SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_OPEN,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_SECURE_CHANNEL_CLOSE		SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_CLOSE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_SECURE_CHANNEL_READ		SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_READ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_SECURE_CHANNEL_WRITE		SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_WRITE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_SECURE_CHANNEL_SYNC		SRVIOCTLCODE(SERVICE_TYPE_SECURE_CHANNEL,SECURE_CHANNEL_SYNC,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_SECURE_CHANNEL_SECURE_CHANNEL_SERVICE_H_ */
