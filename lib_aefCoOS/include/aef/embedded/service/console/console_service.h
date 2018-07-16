
/**
* console_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system console service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_CONSOLE_CONSOLE_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_CONSOLE_CONSOLE_SERVICE_H_

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
* Console base function codes
*/
#define CONSOLE_READ_CHAR		0x800
#define CONSOLE_READ			0x801
#define CONSOLE_READ_WAIT		0x802
#define CONSOLE_CHAR_PRESENT	0x803
#define CONSOLE_WRITE			0x804

/*
* Console service I/O Control codes
*/
#define IOCTL_CONSOLE_READ_CHAR		SRVIOCTLCODE(SERVICE_TYPE_CONSOLE,CONSOLE_READ_CHAR,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CONSOLE_READ			SRVIOCTLCODE(SERVICE_TYPE_CONSOLE,CONSOLE_READ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CONSOLE_READ_WAIT		SRVIOCTLCODE(SERVICE_TYPE_CONSOLE,CONSOLE_READ_WAIT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CONSOLE_CHAR_PRESENT	SRVIOCTLCODE(SERVICE_TYPE_CONSOLE,CONSOLE_CHAR_PRESENT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CONSOLE_WRITE			SRVIOCTLCODE(SERVICE_TYPE_CONSOLE,CONSOLE_WRITE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_CONSOLE_CONSOLE_SERVICE_H_ */
