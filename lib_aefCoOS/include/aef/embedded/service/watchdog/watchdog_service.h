
/**
* watchdog_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system watchdog service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_WATCHDOG_WATCHDOG_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_WATCHDOG_WATCHDOG_SERVICE_H_

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
* Watchdog service base function codes
*/
#define WATCHDOG_INITIALIZE		0x800
#define WATCHDOG_ENABLE			0x801
#define WATCHDOG_DISABLE		0x802
#define WATCHDOG_REGISTER		0x803
#define WATCHDOG_UPDATE			0x804
#define WATCHDOG_REBOOT			0x805

/*
* Watchdog service I/O Control codes
*/
#define IOCTL_WATCHDOG_INITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_WATCHDOG,WATCHDOG_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_WATCHDOG_ENABLE			SRVIOCTLCODE(SERVICE_TYPE_WATCHDOG,WATCHDOG_ENABLE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_WATCHDOG_DISABLE			SRVIOCTLCODE(SERVICE_TYPE_WATCHDOG,WATCHDOG_DISABLE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_WATCHDOG_REGISTER			SRVIOCTLCODE(SERVICE_TYPE_WATCHDOG,WATCHDOG_REGISTER,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_WATCHDOG_UPDATE			SRVIOCTLCODE(SERVICE_TYPE_WATCHDOG,WATCHDOG_UPDATE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_WATCHDOG_REBOOT			SRVIOCTLCODE(SERVICE_TYPE_WATCHDOG,WATCHDOG_REBOOT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_WATCHDOG_WATCHDOG_SERVICE_H_ */
