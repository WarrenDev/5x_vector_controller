
/**
* device_driver_id.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported built-in device driver identifiers.
*
*/
#ifndef  AEF_DEVICE_DRIVER_ID_HEADER_GUARD
# define AEF_DEVICE_DRIVER_ID_HEADER_GUARD

#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* Enumeration of built-in device driver identifiers.
*/
typedef enum device_driver_id
{

    /**
    * UART
    */
    DRV_UART_A 	= 0x0001,
	DRV_IUART_A = 0x0002,
    DRV_UART_B 	= 0x0003,
	DRV_IUART_B	= 0x0004,
    DRV_UART_C 	= 0x0005,
	DRV_IUART_C	= 0x0006,
    DRV_UART_D 	= 0x0007,
	DRV_IUART_D	= 0x0008,
    DRV_UART_E 	= 0x0009,
	DRV_IUART_E	= 0x000a,
    DRV_UART_F 	= 0x000b,
	DRV_IUART_F	= 0x000c,

    /**
    * I2C
    */
    DRV_I2C_A 	= 0x0010,
	DRV_II2C_A	= 0x0011,
    DRV_I2C_B 	= 0x0012,
	DRV_II2C_B	= 0x0013,
    DRV_I2C_C 	= 0x0014,
	DRV_II2C_C	= 0x0015,
    DRV_I2C_D 	= 0x0016,
	DRV_II2C_D	= 0x0017,

    /**
    * SPI
    */
    DRV_SPI_A 	= 0x0020,
	DRV_ISPI_A	= 0x0021,
    DRV_SPI_B 	= 0x0022,
	DRV_ISPI_B	= 0x0023,
    DRV_SPI_C 	= 0x0024,
	DRV_ISPI_C	= 0x0025,
    DRV_SPI_D 	= 0x0026,
	DRV_ISPI_D	= 0x0027,

	/**
	* Wiegand
	*/
	DRV_WIEGAND	= 0x0030,

	/**
	* RFid EM4095
	*/
	DRV_RFID_EM4095 = 0x0031,

	/**
	* RFid HTRC110
	*/
	DRV_RFID_HTRC110 = 0x0032,

	/**
	* RGB LED (FlexTimer)
	*/
	DRV_RGB_LED		= 0x0040,

	/**
	* PWM Buzzer (FlexTimer)
	*/
	DRV_BUZZER		= 0x0041,

	/**
	* TFT LCD ILI9341
	*/
	DRV_TFTLCD_ILI9341 = 0x0042,

	/**
	* CODEC CS43L22
	*/
	DRV_CODEC_CS43L22 = 0x0060,

	/**
	* Audio MP45DT02 digital microphone
	*/
	DRV_AUDIO_MP45DT02 = 0x0070,

	/**
	* 9DOF Sensor MPU-9250
	*/
	DRV_SENSOR_MPU9250 = 0x0080,

	/**
	* Laser Sensor B605B 120M Range
	*/
	DRV_SENSOR_B605B = 0x0090,

} device_driver_id_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_DEVICE_DRIVER_ID_HEADER_GUARD
