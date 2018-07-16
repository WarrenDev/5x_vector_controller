
/**
* service_id.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported built-in service identifiers.
*
*/
#ifndef  UNIKEY_SERVICE_ID_HEADER_GUARD
# define UNIKEY_SERVICE_ID_HEADER_GUARD

#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Enumeration of built-in service identifiers.
    */
    typedef enum service_id
    {

    	/**
    	* System console service
    	*/
    	SRV_SYSTEM_CONSOLE			= 0x0001,

		/**
		* System watchdog service
		*/
		SRV_SYSTEM_WATCHDOG			= 0x0002,

		/**
		* System logging service
		*/
		SRV_SYSTEM_LOGGER			= 0x0003,

		/**
		* Database service
		*/
		SRV_SYSTEM_DATABASE			= 0x0004,

		/**
		* System BLE service
		*/
		SRV_SYSTEM_BLE				= 0x0005,

		/**
		* System property manager
		*/
		SRV_SYSTEM_PROPERTY_MANAGER	= 0x0006,

		/**
		* System Wiegand service
		*/
		SRV_SYSTEM_WIEGAND			= 0x0010,

		/**
		* System EM4095 125 kHz RFID service
		*/
		SRV_SYSTEM_RFID_EM4095		= 0x0011,

		/**
		* System HTRC110 125 kHz RFID service
		*/
		SRV_SYSTEM_RFID_HTRC110		= 0x0012,

		/*
		* System PN532 13.56 MHz RFID service
		*/
		SRV_SYSTEM_RFID_PN532		= 0x0013,

		/**
		* SIA OSDP service
		*/
		SRV_SYSTEM_OSDP				= 0x0014,

		/*
		* User Interface service
		*/
		SRV_SYSTEM_UI				= 0x0015,

		/*
		* Firmware upgrade service
		*/
		SRV_SYSTEM_FIRMWARE_UPGRADE	= 0x0016,

		/*
		* Crypto manager service
		*/
		SRV_SYSTEM_CRYPTO			= 0x0017,

		/*
		* Device communications service (serial)
		*/
		SRV_SYSTEM_DEVICE_COM		= 0x0018,

		/*
		* Keypad communications service
		*/
		SRV_SYSTEM_KEYPAD			= 0x0019,

		/*
		* Certificate Manager service
		*/
		SRV_CERTIFICATE_MANAGER		= 0x001A,

		/*
		* Secure Channel Manager service
		*/
		SRV_SECURE_CHANNEL_MANAGER	= 0x001B,

		/*
		* FNET Network service
		*/
		SRV_FNET_NETWORK			= 0x001C,

		/*
		* MQTT protocol service
		*/
		SRV_SYSTEM_MQTT_COM			= 0x001D,

		/*
		* TELIT cellular modem service
		*/
		SRV_TELIT_CELLULAR_MODEM	= 0x001E,

		/*
		* GPSD service
		*/
		SRV_SYSTEM_GPSD				= 0x001F,

    } service_id_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //UNIKEY_SERVICE_ID_HEADER_GUARD
