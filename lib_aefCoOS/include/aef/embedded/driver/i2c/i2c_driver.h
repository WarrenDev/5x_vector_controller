
/**
* i2c_driver.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the I2C device driver.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_I2C_I2C_DRIVER_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_I2C_I2C_DRIVER_H_

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
* I2C driver function codes
*/
#define I2C_INITIALIZE			0x800
#define I2C_DEINIT				0x801
#define I2C_ENABLE				0x802
#define I2C_DISABLE				0x803
#define I2C_FLUSH				0x804
#define	I2C_GLOBAL_RESET		0x805

/*
* I2C device driver I/O Control codes
*/
#define IOCTL_I2C_INITIALIZE   	DEVIOCTLCODE(DEVICE_TYPE_I2C,I2C_INITIALIZE,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_I2C_DEINIT		DEVIOCTLCODE(DEVICE_TYPE_I2C,I2C_DEINIT,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_I2C_ENABLE   		DEVIOCTLCODE(DEVICE_TYPE_I2C,I2C_ENABLE,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_I2C_DISABLE		DEVIOCTLCODE(DEVICE_TYPE_I2C,I2C_DISABLE,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_I2C_FLUSH   		DEVIOCTLCODE(DEVICE_TYPE_I2C,I2C_FLUSH,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_I2C_GLOBAL_RESET	DEVIOCTLCODE(DEVICE_TYPE_I2C,I2C_GLOBAL_RESET,METHOD_DIRECT,DEVICE_ANY_ACCESS)

/**
* @brief I2C configuration data structure
*/
typedef struct i2c_config_def
{
	uint32_t	clockSpeed;				/*!< I2C clock speed */
	uint16_t	mode;					/*!< I2C mode */
	uint16_t	dutyCycle;				/*!< I2C duty cycle */
	uint16_t	masterAddress;			/*!< I2C master address */
	uint16_t	ack;					/*!< I2C ACK */
	uint16_t	acknowledgedAddress;	/*!< I2C Acknowledged address */
} i2c_config_t;

/**
* @brief I2C transfer data structure
*/
typedef struct i2c_transfer_def
{
	uint8_t	address;					/*!< I2C device address */
	uint8_t reg;						/*!< I2C device register */
	void* data_buffer;					/*!< Data buffer */
	uint16_t size;						/*!< Data length */
	uint32_t flags;						/*!< Transfer flags */
} i2c_transfer_t;

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_I2C_I2C_DRIVER_H_ */
