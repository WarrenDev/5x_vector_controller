
/**
* crypto_manager_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the crypto manager service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_CRYPTO_MANAGER_CRYPTO_MANAGER_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_CRYPTO_MANAGER_CRYPTO_MANAGER_SERVICE_H_

#include <aef/embedded/service/srvddk/uefsrvddk.h>
#include <aef/embedded/service/crypto_manager/prng_service.h>

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
* Crypto manager service base function codes
*/
#define CRYPTO_INITIALIZE				0x800
#define CRYPTO_DEINITIALIZE				0x801
#define CRYPTO_PRNG_ID					0x802
#define CRYPTO_PRNG_INIT				0x803
#define CRYPTO_PRNG_RESEED				0x804
#define CRYPTO_PRNG_READ				0x805
#define CRYPTO_PRNG_SELFTEST			0x806

/*
* Crypto manager service I/O Control codes
*/
#define IOCTL_CRYPTO_INITIALIZE			SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CRYPTO_DEINITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_DEINITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CRYPTO_PRNG_ID			SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_PRNG_ID,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CRYPTO_PRNG_INIT			SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_PRNG_INIT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CRYPTO_PRNG_RESEED		SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_PRNG_RESEED,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CRYPTO_PRNG_READ			SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_PRNG_READ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_CRYPTO_PRNG_SELFTEST		SRVIOCTLCODE(SERVICE_TYPE_CRYPTO_MANAGER,CRYPTO_PRNG_SELFTEST,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_CRYPTO_MANAGER_CRYPTO_MANAGER_SERVICE_H_ */
