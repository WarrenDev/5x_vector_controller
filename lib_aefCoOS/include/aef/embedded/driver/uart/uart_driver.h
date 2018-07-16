
/**
* uart_driver.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the UART device driver.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_UART_UART_DRIVER_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_UART_UART_DRIVER_H_

#include <aef/embedded/driver/ddk/uefddk.h>
#include <aef/embedded/osal/event.h>
#include <stdbool.h>

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
* UART driver function codes
*/
#define UART_INITIALIZE			0x800
#define UART_GETCHAR			0x801
#define UART_PUTCHAR			0x802
#define UART_CHARPRESENT		0x803
#define UART_FLUSH				0x804
#define UART_WAITFORDONE		0x805
#define UART_SETMODE			0x806

/*
* UART device driver I/O Control codes
*/
#define IOCTL_UART_INITIALIZE	DEVIOCTLCODE(DEVICE_TYPE_UART,UART_INITIALIZE,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_UART_GETCHAR		DEVIOCTLCODE(DEVICE_TYPE_UART,UART_GETCHAR,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_UART_PUTCHAR		DEVIOCTLCODE(DEVICE_TYPE_UART,UART_PUTCHAR,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_UART_CHARPRESENT	DEVIOCTLCODE(DEVICE_TYPE_UART,UART_CHARPRESENT,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_UART_FLUSH		DEVIOCTLCODE(DEVICE_TYPE_UART,UART_FLUSH,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_UART_WAITFORDONE	DEVIOCTLCODE(DEVICE_TYPE_UART,UART_WAITFORDONE,METHOD_DIRECT,DEVICE_ANY_ACCESS)
#define IOCTL_UART_SETMODE		DEVIOCTLCODE(DEVICE_TYPE_UART,UART_SETMODE,METHOD_DIRECT,DEVICE_ANY_ACCESS)

#define	UARTMODE_RAW			0x00000000
#define	UARTMODE_LINE			0x00000001

/**
* UART configuration parameter structure definition
*/
typedef struct uart_config_parms_def
{
	uint32_t			baud;				// Baud rate
	uint8_t				size;				// Number of bits
	uint8_t				parity;				// Parity
	uint8_t				stop_bits;			// Number of stop bits
	uint32_t			transmit_timeout;	// Transmit timeout (ms)
	uint32_t			receive_timeout;	// Receive timeout (ms)
	event_ctx_t*		event_handle;		// Event context (used for line mode)
} uart_config_parms_t;

/**
* UART mode parameter structure definition
*/
typedef struct uart_mode_parms_def
{
	uint32_t			mode;				// UART operational mode
	event_ctx_t*		event_handle;		// Event context (used for line mode)
} uart_mode_parms_t;

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_UART_UART_DRIVER_H_ */
