
/**
* logger_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system logger service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_LOGGER_LOGGER_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_LOGGER_LOGGER_SERVICE_H_

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
* logger service base function codes
*/
#define LOGGER_INITIALIZE		0x800
#define LOGGER_READ_EVENT		0x801
#define LOGGER_WRITE_EVENT		0x802
#define LOGGER_DELETE_EVENT		0x803

/*
* Logger service I/O Control codes
*/
#define IOCTL_LOGGER_INITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_LOGGER,LOGGER_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_LOGGER_READ_EVENT		SRVIOCTLCODE(SERVICE_TYPE_LOGGER,LOGGER_READ_EVENT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_LOGGER_WRITE_EVENT	SRVIOCTLCODE(SERVICE_TYPE_LOGGER,LOGGER_WRITE_EVENT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_LOGGER_DELETE_EVENT	SRVIOCTLCODE(SERVICE_TYPE_LOGGER,LOGGER_DELETE_EVENT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_LOGGER_LOGGER_SERVICE_H_ */
