
/**
* rgbled_driver.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the RGB LED device device driver.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_RGBLED_RGBLED_DRIVER_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_RGBLED_RGBLED_DRIVER_H_

#include "aef/embedded/driver/ddk/uefddk.h"

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

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
* RGBLED device driver base function codes
*/
#define RGBLED_CONFIGURE		0x800
#define RGBLED_RESET			0x801
#define	RGBLED_ENABLE			0x802
#define RGBLED_DISABLE			0x803
#define RGBLED_SET_LED			0x804

/*
* RGBLED device driver I/O Control codes
*/
#define IOCTL_RGBLED_CONFIGURE 	((DEVICE_TYPE_RGBLED<<16)|(DEVICE_ANY_ACCESS<<14)|(RGBLED_CONFIGURE<<2)|METHOD_DIRECT)
#define IOCTL_RGBLED_RESET 		((DEVICE_TYPE_RGBLED<<16)|(DEVICE_ANY_ACCESS<<14)|(RGBLED_RESET<<2)|METHOD_DIRECT)
#define IOCTL_RGBLED_ENABLE 	((DEVICE_TYPE_RGBLED<<16)|(DEVICE_ANY_ACCESS<<14)|(RGBLED_ENABLE<<2)|METHOD_DIRECT)
#define IOCTL_RGBLED_DISABLE 	((DEVICE_TYPE_RGBLED<<16)|(DEVICE_ANY_ACCESS<<14)|(RGBLED_DISABLE<<2)|METHOD_DIRECT)
#define IOCTL_RGBLED_SET_LED 	((DEVICE_TYPE_RGBLED<<16)|(DEVICE_ANY_ACCESS<<14)|(RGBLED_SET_LED<<2)|METHOD_DIRECT)

/**
* Enumeration of RGB LED IDs.
*/
typedef enum rgbled_id
{
	RGBLED_ALL_OFF  = 0x00,
	RGBLED_1		= 0x01,
	RGBLED_2		= 0x02,
	RGBLED_3		= 0x04,
	RGBLED_4		= 0x08,
	RGBLED_5		= 0x10,
	RGBLED_6		= 0x20,
	RGBLED_7		= 0x40,
	RGBLED_8		= 0x80,
	RGBLED_ALL_ON	= 0xFF,
} rgbled_id_t;

/**
* Enumeration of color IDs.
*/
typedef enum rgbled_color
{
	COLOR_NONE		= 0x00,
	COLOR_BLACK	 	= 0x01,	// All off
	COLOR_WHITE	 	= 0x02,	// All on
	COLOR_RED	 	= 0x03,	// Red on
	COLOR_GREEN	 	= 0x04,	// Green on
	COLOR_BLUE	 	= 0x05,	// Blue on
	COLOR_MAGENTA	= 0x06,	// Magenta ( not yet supported )
	COLOR_CYAN   	= 0x07, // Cyan ( not yet supported )
	COLOR_ORANGE 	= 0x08,	// Red on/Green on
	COLOR_PURPLE 	= 0x09,	// Red on/Blue on
	COLOR_TEAL   	= 0x0A,	// Green on/Blue on
} rgbled_color_t;

/**
* Number of LEDs supported
*/
#define NUM_OF_LEDS		8

/**
* RGB LED control structure definition
*/
typedef struct rgbled_driver_control_def
{
	rgbled_id_t		led_control;
	rgbled_color_t	color_control;
	uint16_t		intensity[NUM_OF_LEDS];
	uint32_t		timeout;
} rgbled_driver_control_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_RGBLED_RGBLED_DRIVER_H_ */
