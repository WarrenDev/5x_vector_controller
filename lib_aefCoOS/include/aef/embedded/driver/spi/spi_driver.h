
/**
* spi_driver.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the SPI device driver.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_SPI_SPI_DRIVER_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_SPI_SPI_DRIVER_H_

#include <aef/embedded/driver/ddk/uefddk.h>

/**
* The IOCTL function codes from 0x800 to 0xFFF are for customer use.
*
* The format of I/O control codes for the iocontrol call:
*    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method)
*
* Configuring device driver I/O control codes in this manner produces
* unique system-wide I/O control codes.  It also provides a mechanism for
* isolating kernel mode memory spaces from user mode memory spaces and
* allows us to use the memory protection unit (MPU) of the processor.
*/

/*
* SPI driver function codes
*/
#define SPI_FLUSH				0x801

/*
* SPI device driver I/O Control codes
*/
#define IOCTL_SPI_FLUSH		DEVIOCTLCODE(DEVICE_TYPE_SPI,SPI_FLUSH,METHOD_DIRECT,DEVICE_ANY_ACCESS)

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_SPI_SPI_DRIVER_H_ */
