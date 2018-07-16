
/**
* firmware_upgrade_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the Firmware Upgrade system service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_FIRMWARE_UPGRADE_FIRMWARE_UPGRADE_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_FIRMWARE_UPGRADE_FIRMWARE_UPGRADE_SERVICE_H_

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
* Firmware Upgrade service base function codes
*/
#define FW_UPGRADE_INITIALIZE					0x800
#define FW_UPGRADE_SET_HEADER					0x801
#define FW_UPGRADE_BEGIN						0x802
#define	FW_UPGRADE_ADD_CHUNK					0x803
#define	FW_UPGRADE_END							0x804
#define FW_UPGRADE_VALIDATE_IMAGE				0x805
#define FW_UPGRADE_PERFORM_UPGRADE				0x806

/*
* Firmware Upgrade service I/O Control codes
*/
#define IOCTL_FW_UPGRADE_INITIALIZE			SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FW_UPGRADE_SET_HEADER			SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_SET_HEADER,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FW_UPGRADE_BEGIN				SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_BEGIN,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FW_UPGRADE_ADD_CHUNK			SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_ADD_CHUNK,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FW_UPGRADE_END				SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_END,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FW_UPGRADE_VALIDATE_IMAGE		SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_VALIDATE_IMAGE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FW_UPGRADE_PERFORM_UPGRADE	SRVIOCTLCODE(SERVICE_TYPE_FIRMWARE_UPGRADE,FW_UPGRADE_PERFORM_UPGRADE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_FIRMWARE_UPGRADE_FIRMWARE_UPGRADE_SERVICE_H_ */
