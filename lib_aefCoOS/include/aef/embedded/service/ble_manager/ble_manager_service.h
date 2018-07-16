
/**
* ble_manager_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the BLE Manager system service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_BLE_MANAGER_BLE_MANAGER_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_BLE_MANAGER_BLE_MANAGER_SERVICE_H_

#include <aef/embedded/service/srvddk/uefsrvddk.h>
#include <../src/services/ble_manager/ble_hci/bcomdef.h>
#include <../src/services/ble_manager/ble_hci/gap.h>
#include <../src/services/ble_manager/ble_hci/att.h>

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
* BLE Manager service base function codes
*/
#define BLE_MANAGER_INITIALIZE					0x800
#define BLE_MANAGER_CENTRAL_INIT				0x801
#define BLE_MANAGER_CENTRAL_START				0x802
#define BLE_MANAGER_PERIPHERAL_INIT				0x803
#define BLE_MANAGER_PERIPHERAL_START			0x804
#define BLE_MANAGER_BEGIN_SCAN					0x805
#define BLE_MANAGER_END_SCAN					0x806
#define BLE_MANAGER_CREATE_LINK					0x809
#define BLE_MANAGER_DESTROY_LINK				0x80A
#define BLE_MANAGER_DESTROY_LINK_EX				0x80B
#define BLE_MANAGER_FIND_SERVICE				0x80C
#define BLE_MANAGER_DISC_ALL_CHARS				0x80D
#define BLE_MANAGER_READ_CHAR_VALUE				0x80E
#define BLE_MANAGER_READ_CHAR_VALUE_RSP			0x80F
#define BLE_MANAGER_READ_BLOB_CHAR_VALUE		0x810
#define BLE_MANAGER_READ_BLOB_CHAR_VALUE_RSP	0x811
#define BLE_MANAGER_WRITE_CHAR_VALUE			0x812
#define	BLE_MANAGER_WRITE_CHAR_VALUE_RSP		0x813
#define BLE_MANAGER_PREPARE_WRITE_REQ			0x814
#define BLE_MANAGER_EXECUTE_WRITE_REQ			0x815
#define BLE_MANAGER_GET_BDADDR					0x81A
#define BLE_MANAGER_SET_BDADDR					0x81B
#define BLE_MANAGER_MAKE_DISCOVERABLE			0x820
#define BLE_MANAGER_UPDATE_ADVERT_DATA			0x821
#define BLE_MANAGER_END_DISCOVERY				0x822
#define BLE_MANAGER_AUTHENTICATE				0x823
#define BLE_MANAGER_PASSKEY_UPDATE				0x824
#define	BLE_MANAGER_SLAVE_SECURITY_UPDATE_REQ	0x825
#define BLE_MANAGER_SIGNABLE					0x826
#define BLE_MANAGER_BOND						0x827
#define BLE_MANAGER_TERMINATE_AUTH				0x828
#define BLE_MANAGER_UPDATE_LINK_PARM_REQ		0x829
#define BLE_MANAGER_EXCHANGE_MTU				0x82A
#define	BLE_MANAGER_FIND_INFO_REQ				0x82B
#define BLE_MANAGER_FIND_BY_TYPE_VALUE_RSP		0x82C
#define BLE_MANAGER_ADD_SERVICE					0x82D
#define	BLE_MANAGER_DEL_SERVICE					0x82E
#define BLE_MANAGER_ADD_ATTRIBUTE				0x82F
#define BLE_MANAGER_ADD_ATTRIBUTE_LONG			0x830
#define BLE_MANAGER_ERROR_RSP					0x831
#define BLE_MANAGER_OBSERVER_ATTACH				0x840
#define BLE_MANAGER_OBSERVER_DETACH				0x841
#define BLE_MANAGER_NP_RESET					0x850
#define BLE_MANAGER_NP_FORCE_BOOT				0x851

/*
* BLE Manager service I/O Control codes
*/
#define IOCTL_BLE_MANAGER_INITIALIZE				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_CENTRAL_INIT				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_CENTRAL_INIT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_CENTRAL_START				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_CENTRAL_START,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_PERIPHERAL_INIT			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_PERIPHERAL_INIT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_PERIPHERAL_START			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_PERIPHERAL_START,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_BEGIN_SCAN				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_BEGIN_SCAN,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_END_SCAN					SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_END_SCAN,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_CREATE_LINK				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_CREATE_LINK,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_DESTROY_LINK				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_DESTROY_LINK,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_DESTROY_LINK_EX			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_DESTROY_LINK_EX,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_FIND_SERVICE				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_FIND_SERVICE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_DISC_ALL_CHARS			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_DISC_ALL_CHARS,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_READ_CHAR_VALUE			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_READ_CHAR_VALUE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_READ_CHAR_VALUE_RSP		SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_READ_CHAR_VALUE_RSP,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_READ_BLOB_CHAR_VALUE		SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_READ_BLOB_CHAR_VALUE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_READ_BLOB_CHAR_VALUE_RSP	SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_READ_BLOB_CHAR_VALUE_RSP,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_WRITE_CHAR_VALUE			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_WRITE_CHAR_VALUE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_WRITE_CHAR_VALUE_RSP		SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_WRITE_CHAR_VALUE_RSP,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_PREPARE_WRITE_REQ			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_PREPARE_WRITE_REQ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_EXECUTE_WRITE_REQ			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_EXECUTE_WRITE_REQ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_GET_BDADDR				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_GET_BDADDR,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_SET_BDADDR				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_SET_BDADDR,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_MAKE_DISCOVERABLE			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_MAKE_DISCOVERABLE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_UPDATE_ADVERT_DATA		SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_UPDATE_ADVERT_DATA,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_END_DISCOVERY				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_END_DISCOVERY,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_AUTHENTICATE				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_AUTHENTICATE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_PASSKEY_UPDATE			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_PASSKEY_UPDATE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define	IOCTL_BLE_MANAGER_SLAVE_SECURITY_UPDATE_REQ	SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_SLAVE_SECURITY_UPDATE_REQ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_SIGNABLE					SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_SIGNABLE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_BOND						SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_BOND,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_TERMINATE_AUTH			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_TERMINATE_AUTH,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_UPDATE_LINK_PARM_REQ		SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_UPDATE_LINK_PARM_REQ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_EXCHANGE_MTU				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_EXCHANGE_MTU,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define	IOCTL_BLE_MANAGER_FIND_INFO_REQ				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_FIND_INFO_REQ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define	IOCTL_BLE_MANAGER_FIND_BY_TYPE_VALUE_RSP	SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_FIND_BY_TYPE_VALUE_RSP,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_ADD_SERVICE				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_ADD_SERVICE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define	IOCTL_BLE_MANAGER_DEL_SERVICE				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_DEL_SERVICE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_ADD_ATTRIBUTE				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_ADD_ATTRIBUTE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_ADD_ATTRIBUTE_LONG		SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_ADD_ATTRIBUTE_LONG,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_ERROR_RSP					SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_ERROR_RSP,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_OBSERVER_ATTACH			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_OBSERVER_ATTACH,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_OBSERVER_DETACH			SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_OBSERVER_DETACH,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_NP_RESET					SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_NP_RESET,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_BLE_MANAGER_NP_FORCE_BOOT				SRVIOCTLCODE(SERVICE_TYPE_BLE,BLE_MANAGER_NP_FORCE_BOOT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

#define DEFAULT_ADVERT_OFF_TIME       			30000   // 30 seconds
#define RSSI_NOT_AVAILABLE            			127
#define DEFAULT_MIN_CONN_INTERVAL     			0x0006  // 100 milliseconds
#define DEFAULT_MAX_CONN_INTERVAL     			0x0C80  // 4 seconds
#define MIN_CONN_INTERVAL             			0x0006
#define MAX_CONN_INTERVAL             			0x0C80
#define DEFAULT_TIMEOUT_MULTIPLIER    			1000
#define CONN_INTERVAL_MULTIPLIER      			6
#define MIN_SLAVE_LATENCY             			0
#define MAX_SLAVE_LATENCY             			500
#define MIN_TIMEOUT_MULTIPLIER        			0x000a
#define MAX_TIMEOUT_MULTIPLIER        			0x0c80
#define MAX_TIMEOUT_VALUE             			0xFFFF
#define MAX_PREPARE_WRITES						5

/**
*  Function pointer definition for HCI message dependents
*/
typedef void (*hci_msg_notification_func_t) (void* instance, void* hci_msg_data);

/**
* HCI message observer structure definition
*/
struct _hci_msg_observer_def
{
	void* instance;
	uint16_t hci_msg;
	hci_msg_notification_func_t notification;
} __attribute__((__packed__));
typedef struct _hci_msg_observer_def hci_msg_observer_t;

/**
* HCI message observer parameters structure definition
*/
struct _hci_observer_parms_def
{
	void* instance;
	uint16_t hci_msg;
	hci_msg_notification_func_t noti;
} __attribute__((__packed__));
typedef struct _hci_observer_parms_def hci_observer_parms_t;

/**
* Discovery parameters structure definition
*/
struct _discovery_parms_def
{
	uint8_t		mode;
	uint8_t		scan_type;
	uint8_t		white_list;
} __attribute__((__packed__));
typedef struct _discovery_parms_def discovery_parms_t;

/**
* Create link parameters structure definition
*/
struct _create_link_parms_def
{
	uint8_t*	addr;
	uint8_t		addr_type;
	uint8_t		high_duty_cycle;
	uint8_t		white_list;
} __attribute__((__packed__));
typedef struct _create_link_parms_def create_link_parms_t;

/**
* Destroy link parameters structure definition
*/
struct _destroy_link_parms_def
{
	uint16_t	handle;
	uint8_t		reason;
} __attribute__((__packed__));
typedef struct _destroy_link_parms_def destroy_link_parms_t;

/**
* Find service parameters structure definition
*/
struct _find_service_parms_def
{
	uint16_t	handle;
	uint8_t*	uuid;
	uint8_t		length;
} __attribute__((__packed__));
typedef struct _find_service_parms_def find_service_parms_t;

/**
* Discover all characteristics parameters structure definition
*/
struct _disc_all_chars_parms_def
{
	uint16_t	handle;
	uint16_t	start_handle;
	uint16_t	end_handle;
} __attribute__((__packed__));
typedef struct _disc_all_chars_parms_def disc_all_chars_parms_t;

/**
* read characteristic parameters structure definition
*/
struct _read_char_parms_def
{
	uint16_t	handle;
	uint16_t	attr_handle;
} __attribute__((__packed__));
typedef struct _read_char_parms_def read_char_parms_t;

/**
* Read blob characteristic parameters structure definition
*/
struct _read_blob_char_parms_def
{
	uint16_t	handle;
	uint16_t	attr_handle;
	uint16_t	offset;
} __attribute__((__packed__));
typedef struct _read_blob_char_parms_def read_blob_char_parms_t;

/**
* Write characteristic parameters structure definition
*/
struct _write_char_parms_def
{
	uint16_t	handle;
	uint16_t	attr_handle;
	uint8_t		length;
	uint8_t*	value;
} __attribute__((__packed__));
typedef struct _write_char_parms_def write_char_parms_t;

/**
* Prepare write request parameters structure definition
*/
struct _prepare_write_req_parms_def
{
	uint16_t	handle;
	uint16_t	attr_handle;
	uint16_t	offset;
	uint8_t		length;
	uint8_t*	value;
} __attribute__((__packed__));
typedef struct _prepare_write_req_parms_def prepare_write_req_parms_t;

/**
* Execute write request parameters structure definition
*/
struct _execute_write_req_parms_def
{
	uint16_t	handle;
	uint8_t		flags;
} __attribute__((__packed__));
typedef struct _execute_write_req_parms_def execute_write_req_parms_t;

/**
* Discovery mode parameters structure definition
*/
struct _discovery_mode_parms_def
{
	uint16_t	mode;
} __attribute__((__packed__));
typedef struct _discovery_parms_def discovery_parms_t;

/**
* Configure device address structure definition
*/
struct _configure_device_addr_def
{
	uint8_t addr_type;
	uint8_t	addr[B_ADDR_LEN];
} __attribute__((__packed__));
typedef struct _configure_device_addr_def configure_device_addr_t;

/**
* Make discoverable parameter structure definition
*/
struct _make_discoverable_parms_def
{
	uint8_t event_type;
	uint8_t initiator_addr_type;
	uint8_t	initiator_addr[B_ADDR_LEN];
	uint8_t channel_map;
	uint8_t filter_policy;
} __attribute__((__packed__));
typedef struct _make_discoverable_parms_def make_discoverable_parms_t;

/**
* Make discoverable parameter structure definition
*/
struct _advert_data_parms_def
{
	uint8_t ad_type;
	uint8_t length;
	uint8_t	advert_data[B_MAX_ADV_LEN];
} __attribute__((__packed__));
typedef struct _advert_data_parms_def advert_data_parms_t;

/**
* Authenticate parameter structure definition
*/
struct _authenticate_parms_def
{
	gapAuthParams_t *params;
	gapPairingReq_t *pairReq;
} __attribute__((__packed__));
typedef struct _authenticate_parms_def authenticate_parms_t;

/**
* Passkey update parameter structure definition
*/
struct _passkey_update_parms_def
{
	uint16_t	handle;
	uint8_t* 	passkey;
} __attribute__((__packed__));
typedef struct _passkey_update_parms_def passkey_update_parms_t;

/**
* Slave security update request parameter structure definition
*/
struct _slave_security_update_parms_def
{
	uint16_t	handle;
	uint8_t 	authReq;
} __attribute__((__packed__));
typedef struct _slave_security_update_parms_def slave_security_update_parms_t;

/**
* Signable parameter structure definition
*/
struct _signable_parms_def
{
	uint16_t	handle;
	uint8_t 	authenticated;
	uint8_t*	key;
	uint32_t	signCounter;
} __attribute__((__packed__));
typedef struct _signable_parms_def signable_parms_t;

/**
* Bond parameter structure definition
*/
struct _bond_parms_def
{
	uint16_t	handle;
	uint8_t 	authenticated;
	uint8_t*	ltk;
	uint16_t	diversifier;
	uint8_t*	randomNum;
	uint8_t		keySize;
} __attribute__((__packed__));
typedef struct _bond_parms_def bond_parms_t;

/**
* Terminate authentication parameters structure definition
*/
struct _terminate_auth_parms_def
{
	uint16_t	handle;
	uint8_t		reason;
} __attribute__((__packed__));
typedef struct _terminate_auth_parms_def terminate_auth_parms_t;

/**
* Link parameters structure definition
*/
struct _link_parms_def
{
	uint16_t	handle;
	uint16_t	intervalMin;
	uint16_t	intervalMax;
	uint16_t	connectionLatency;
	uint16_t	connectionTimeout;
} __attribute__((__packed__));
typedef struct _link_parms_def link_parms_t;

/**
* MTU parameter structure definition
*/
struct _mtu_parms_def
{
	uint16_t	mtu;
} __attribute__((__packed__));
typedef struct _mtu_parms_def mtu_parms_t;

/**
* Find info request parameter structure definition
*/
struct _find_info_parms_def
{
	uint16_t	handle;
	uint16_t	start_handle;
	uint16_t	end_handle;
} __attribute__((__packed__));
typedef struct _find_info_parms_def find_info_parms_t;

/**
* Add service parameter structure definition
*/
struct _add_service_parms_def
{
	uint16_t	uuid;
	uint16_t	numOfAttrs;
} __attribute__((__packed__));
typedef struct _add_service_parms_def add_service_parms_t;

/**
* Delete service parameter structure definition
*/
struct _del_service_parms_def
{
	uint16_t	uuid;
} __attribute__((__packed__));
typedef struct _del_service_parms_def del_service_parms_t;

/**
* Attribute parameter structure definition
*/
struct _attribute_parms_def
{
	uint16_t	uuid;
	uint8_t		permissions;
} __attribute__((__packed__));
typedef struct _attribute_parms_def attribute_parms_t;

/**
* Attribute 128-bit parameter structure definition
*/
struct _attribute128_parms_def
{
	uint8_t*	uuid;
	uint8_t		permissions;
} __attribute__((__packed__));
typedef struct _attribute128_parms_def attribute128_parms_t;

/**
* Read characteristic value response parameter structure definition
*/
struct _read_char_value_rsp_parms_def
{
	uint16_t	handle;
	uint8_t*	value;
	uint16_t	length;
} __attribute__((__packed__));
typedef struct _read_char_value_rsp_parms_def read_char_value_rsp_parms_t;

/**
* Write characteristic value response parameter structure definition
*/
struct _write_char_value_rsp_parms_def
{
	uint16_t	handle;
} __attribute__((__packed__));
typedef struct _write_char_value_rsp_parms_def write_char_value_rsp_parms_t;

/**
* Find by type value response parameter structure definition
*/
struct _find_by_type_value_rsp_parms_def
{
	uint16_t	handle;
	attHandlesInfo_t* info;
	uint8_t		count;
} __attribute__((__packed__));
typedef struct _find_by_type_value_rsp_parms_def find_by_type_value_rsp_parms_t;

/**
* Error response parameter structure definition
*/
struct _error_rsp_parms_def
{
	uint16_t	connectionHandle;
	uint8_t		reqOpCode;
	uint16_t	handle;
	uint8_t		errorCode;
} __attribute__((__packed__));
typedef struct _error_rsp_parms_def error_rsp_parms_t;

/**
* Reset parameter structure definition
*/
struct _reset_parms_def
{
	uint8_t		resetType;
} __attribute__((__packed__));
typedef struct _reset_parms_def reset_parms_t;

/**
* Central role parameter structure definition
*/
struct _central_role_parms_def
{
	uint8_t  gapRole_ProfileRole;
	uint8_t  gapRole_IRK[KEYLEN];
	uint8_t  gapRole_SRK[KEYLEN];
	uint32_t gapRole_SignCounter;
	uint8_t  gapRole_BdAddr[B_ADDR_LEN];
	uint8_t  gapRole_MaxScanRes;
} __attribute__((__packed__));
typedef struct _central_role_parms_def central_role_parms_t;

/**
* Peripheral role parameter structure definition
*/
struct _peripheral_role_parms_def
{
	uint8_t  gapRole_ProfileRole;
	uint8_t  gapRole_IRK[KEYLEN];
	uint8_t  gapRole_SRK[KEYLEN];
	uint32_t gapRole_SignCounter;
	uint8_t  gapRole_BdAddr[B_ADDR_LEN];
	uint8_t  gapRole_AdvEnabled;
	uint8_t  gapRole_AdvNonConnEnabled;
	uint16_t gapRole_AdvertOffTime;
	uint8_t  gapRole_AdvertDataLen;
	uint8_t  gapRole_AdvertData[B_MAX_ADV_LEN];
	uint8_t  gapRole_ScanRspDataLen ;
	uint8_t  gapRole_ScanRspData[B_MAX_ADV_LEN];
	uint8_t  gapRole_AdvEventType;
	uint8_t  gapRole_AdvDirectType;
	uint8_t  gapRole_AdvDirectAddr[B_ADDR_LEN];
	uint8_t  gapRole_AdvChanMap;
	uint8_t  gapRole_AdvFilterPolicy;
	uint16_t gapRole_ConnectionHandle;
	uint16_t gapRole_RSSIReadRate;
	uint8_t  gapRole_ConnectedDevAddr[B_ADDR_LEN];
	uint8_t  gapRole_ParamUpdateEnable;
	uint16_t gapRole_MinConnInterval;
	uint16_t gapRole_MaxConnInterval;
	uint16_t gapRole_SlaveLatency;
	uint16_t gapRole_TimeoutMultiplier;
	uint16_t gapRole_ConnInterval;
	uint16_t gapRole_ConnSlaveLatency;
	uint16_t gapRole_ConnTimeout;
} __attribute__((__packed__));
typedef struct _peripheral_role_parms_def peripheral_role_parms_t;

union _ble_role_parms_def
{
	central_role_parms_t	central;
	peripheral_role_parms_t	peripheral;
} __attribute__((__packed__));
typedef union _ble_role_parms_def ble_role_parms_t;

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_BLE_MANAGER_BLE_MANAGER_SERVICE_H_ */
