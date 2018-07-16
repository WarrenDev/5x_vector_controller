
/**
* devicecom_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the Device communications service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_DEVICECOM_DEVICECOM_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_DEVICECOM_DEVICECOM_SERVICE_H_

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
* Device communications service base function codes
*/
#define DEVICE_COM_INITIALIZE			0x800
#define DEVICE_COM_RESET				0x801
#define DEVICE_COM_READ					0x802
#define DEVICE_COM_WRITE				0x803
#define DEVICE_COM_RECV_PACKET			0x804
#define DEVICE_COM_SEND_PACKET			0x805

/*
* Device communications service I/O Control codes
*/
#define IOCTL_DEVICE_COM_INITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_DEVICE_COM,DEVICE_COM_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DEVICE_COM_RESET			SRVIOCTLCODE(SERVICE_TYPE_DEVICE_COM,DEVICE_COM_RESET,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DEVICE_COM_READ			SRVIOCTLCODE(SERVICE_TYPE_DEVICE_COM,DEVICE_COM_READ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DEVICE_COM_WRITE			SRVIOCTLCODE(SERVICE_TYPE_DEVICE_COM,DEVICE_COM_WRITE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DEVICE_COM_RECV_PACKET	SRVIOCTLCODE(SERVICE_TYPE_DEVICE_COM,DEVICE_COM_RECV_PACKET,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DEVICE_COM_SEND_PACKET	SRVIOCTLCODE(SERVICE_TYPE_DEVICE_COM,DEVICE_COM_SEND_PACKET,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

/**
* Packet layout -
*
* ---------------------------------------------------------------------------
* ||  SOF   |  Sequence   |  Size   |  Packet Type  |  Payload   |   CRC   ||
* || 8 bits |   8 bits    | 16 bits |     8 bits    |  Size - 3  | 16 bits ||
* ---------------------------------------------------------------------------
*/

/**
* Enumeration of Device communication command frame types
*/
typedef enum devicecom_cmd
{
  	/**
   	* Get driver version number
   	*/
   	DEVICECOM_GET_VERSION		= 0x01,

	/**
	* Send credential using credential_data_t
	*/
	DEVICECOM_SEND_CREDENTIAL,

	/**
	* Send Wiegand credential using wiegand_data_t
	*/
	DEVICECOM_SEND_WIEGAND,

} devicecom_cmd_t;

/**
* Enumeration of Device communication response frame types
*/
typedef enum devicecom_rsp
{
  	/**
   	* ACK
   	*/
   	DEVICECOM_ACK		= 0x10,

  	/**
   	* NAK
   	*/
	DEVICECOM_NAK		= 0x20,

} devicecom_rsp_t;

/**
* Communications packet header structure definition
*/
struct _devicecom_hdr_def
{
	uint8_t		sof;						// Start of frame
	uint8_t		sequence;					// Frame sequence number
	uint16_t	size;						// Frame size (length of type, payload, CRC)
	uint8_t		frame_type;					// Frame type
} __attribute__((__packed__));
typedef struct _devicecom_hdr_def devicecom_hdr_t;

/**
* Credential data structure
* This data structure is used to send card data over the serial link
* in the form of site code and card code.
*/
struct _credential_data_def
{
	uint32_t	site_code;
	uint32_t	card_code;
} __attribute__((__packed__));
typedef struct _credential_data_def credential_data_t;

/**
* Wiegand data structure
* This data structure is used to send card data over the serial link
* in the form of a raw Wiegand bit code.  Supports Wiegand values up to
* 128 bits.
*/
struct _wiegand_data_def
{
	uint64_t	wiegand_code_lo;
	uint64_t	wiegand_code_hi;
} __attribute__((__packed__));
typedef struct _wiegand_data_def wiegand_data_t;

/**
* Device communications service command structure
* This data structure is used a application using the Device communications
* service to send a message over the serial link.
*/
struct _device_com_message_def
{
	devicecom_cmd_t	message_type;
	uint16_t		message_size;
	void*			message_data;
};
typedef struct _device_com_message_def device_com_message_t;

/**
* Device communications service response structure
* This data structure is used by the Device communications service to return a
* message response received over the serial link to the application.
*/
struct _device_com_response_def
{
	devicecom_rsp_t	response_type;
	uint16_t		length;
	uint8_t			data[1];
};
typedef struct _device_com_response_def device_com_response_t;

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_DEVICECOM_DEVICECOM_SERVICE_H_ */
