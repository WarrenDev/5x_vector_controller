
/**
* mqtt_service.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system MQTT service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_MQTT_MQTT_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_MQTT_MQTT_SERVICE_H_

#include "aef/embedded/service/service_manager.h"
#include <aef/embedded/service/fnet_stack/fnet_stack_service.h>

#ifdef __cplusplus
extern "C"
{
#endif

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

/**
* MQTT service base function codes
*/
#define MQTT_INITIALIZE				0x800
#define MQTT_SET_AUTH				0x801
#define MQTT_SET_CONNECTION			0x802
#define MQTT_SET_KEEP_ALIVE			0x803
#define MQTT_GET_KEEP_ALIVE			0x804
#define MQTT_CONNECT				0x805
#define MQTT_CONNECT_EX				0x806
#define MQTT_DISCONNECT				0x807
#define MQTT_DISCONNECT_EX			0x808
#define MQTT_PUBLISH				0x809
#define MQTT_PUBLISH_WITH_QOS		0x80A
#define MQTT_PUBLISH_REL			0x80B
#define MQTT_PUBLISH_ACK			0x80C
#define MQTT_PUBLISH_COMPLETE		0x80D
#define	MQTT_SUBSCRIBE				0x80E
#define MQTT_UNSUBSCRIBE			0x80F
#define MQTT_PING_REQ				0x810
#define MQTT_DEINITIALIZE			0x811

/**
* MQTT service I/O Control codes
*/
#define IOCTL_MQTT_INITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_SET_AUTH			SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_SET_AUTH,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_SET_CONNECTION	SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_SET_CONNECTION,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_SET_KEEP_ALIVE	SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_SET_KEEP_ALIVE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_GET_KEEP_ALIVE	SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_GET_KEEP_ALIVE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_CONNECT			SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_CONNECT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_CONNECT_EX		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_CONNECT_EX,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_DISCONNECT		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_DISCONNECT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_DISCONNECT_EX	SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_DISCONNECT_EX,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_PUBLISH			SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_PUBLISH,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_PUBLISH_WITH_QOS	SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_PUBLISH_WITH_QOS,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_PUBLISH_REL		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_PUBLISH_REL,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_PUBLISH_ACK		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_PUBLISH_ACK,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_PUBLISH_COMPLETE	SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_PUBLISH_COMPLETE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_SUBSCRIBE		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_SUBSCRIBE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_UNSUBSCRIBE		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_UNSUBSCRIBE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_PING_REQ			SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_PING_REQ,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_MQTT_DEINITIALIZE		SRVIOCTLCODE(SERVICE_TYPE_MQTT,MQTT_DEINITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

/**
* MQTT definitions
*/
#ifndef MQTT_SERVER_NAME_LENGTH
	#define MQTT_SERVER_NAME_LENGTH		257
#endif

#ifndef MQTT_IP_ADDRESS_LENGTH
	#define MQTT_IP_ADDRESS_LENGTH		24
#endif

#ifndef MQTT_CONF_USERNAME_LENGTH
	#define MQTT_CONF_USERNAME_LENGTH 	257    // Recommended by MQTT Specification (128 + '\0')
#endif

#ifndef MQTT_CONF_PASSWORD_LENGTH
	#define MQTT_CONF_PASSWORD_LENGTH 	257    // Recommended by MQTT Specification (200 + '\0')
#endif

#ifndef MQTT_CONF_CLIENTID_LENGTH
	#define MQTT_CONF_CLIENTID_LENGTH 	257
#endif

/**
* MQTT protocol request identifier definitions
*/
#define MQTT_MSG_CONNECT        		(1<<4)	// Client request to connect to server
#define MQTT_MSG_CONNACK        		(2<<4)  // Connect acknowledgement
#define MQTT_MSG_PUBLISH        		(3<<4)  // Publish message
#define MQTT_MSG_PUBACK         		(4<<4)  // Publish acknowledgement
#define MQTT_MSG_PUBREC         		(5<<4)  // Publish received (assured delivery part 1)
#define MQTT_MSG_PUBREL         		(6<<4)  // Publish release (assured delivery part 2)
#define MQTT_MSG_PUBCOMP        		(7<<4)  // Publish complete assured delivery part 3)
#define MQTT_MSG_SUBSCRIBE      		(8<<4)  // Client subscribe request
#define MQTT_MSG_SUBACK         		(9<<4)  // Subscribe acknowledgement
#define MQTT_MSG_UNSUBSCRIBE    		(10<<4) // Client unsubscribe request
#define MQTT_MSG_UNSUBACK       		(11<<4) // Unsubscribe acknowledgement
#define MQTT_MSG_PINGREQ        		(12<<4) // Ping request
#define MQTT_MSG_PINGRESP       		(13<<4) // Ping response
#define MQTT_MSG_DISCONNECT     		(14<<4) // Client is disconnecting
#define MQTT_MSG_ERROR          		(0<<4)  // Error response

/**
* MQTT connection ACK return code identifier definitions
*/
#define MQTT_CONNACK_ACCEPTED           0x00    // Connection accepted
#define MQTT_CONNACK_REFUSED_PROTOCOL   0x01    // Connection refused - protocol version
#define MQTT_CONNACK_REFUSED_IDENTIFIER 0x02    // Connection refused - identifier rejected
#define MQTT_CONNACK_REFUSED_SERVER     0x03    // Connection refused - server unavailable
#define MQTT_CONNACK_REFUSED_CRED       0x04    // Connection refused - user name or password
#define MQTT_CONNACK_REFUSED_AUTH       0x05    // Connection refused - not authorized

#define MQTT_DUP_FLAG           		(1<<3)

#define MQTT_QOS0_FLAG          		(0<<1)
#define MQTT_QOS1_FLAG          		(1<<1)
#define MQTT_QOS2_FLAG          		(2<<1)

#define MQTT_RETAIN_FLAG        		1

#define MQTT_CLEAN_SESSION      		(1<<1)
#define MQTT_WILL_FLAG          		(1<<2)
#define MQTT_WILL_RETAIN        		(1<<5)
#define MQTT_PASSWORD_FLAG      		(1<<6)
#define MQTT_USERNAME_FLAG      		(1<<7)

#define MQTT_QOS0          				0		// QoS 0 (use in publish/subscribe
#define MQTT_QOS1          				1		// QoS 1 (use in publish/subscribe
#define MQTT_QOS2          				2		// QoS 2 (use in publish/subscribe

#define MQTT_CL_EVT_ERROR				0x00	// Error occurred
#define MQTT_CL_EVT_CONNACK				0x02	// Connection acknowledgement received from the server
#define MQTT_CL_EVT_PUBACK   			0x04  	// PUBACK has been received from the server
#define MQTT_CL_EVT_PUBCOMP  			0x07  	// PUBCOMP has been received from the server
#define MQTT_CL_EVT_SUBACK   			0x09  	// SUBACK has been received from the server
#define MQTT_CL_EVT_UNSUBACK 			0x0B  	// UNSUBACK has been received from the server

#define MQTT_ADDR_HOSTNAME				0x01	// Hostname address type
#define MQTT_ADDR_IP					0x02	// IP address type

#define MQTT_STATUS_ADDR_RESOLVED		0x01	// Server address resolved
#define MQTT_STATUS_ONLINE				0x02	// Connected to the server
#define MQTT_STATUS_SESSION_ACTIVE		0x04	// MQTT session active
#define MQTT_KEEP_ALIVE_ENABLED			0x08	// MQTT connection keep-alive enabled

/**
* Callback Routines
* The routines are invoked by the MQTT service
*
* \note The user application implements the callbacks that are registered with the MQTT service.
*       It is recommended to signal another task from the callback routines
*       invoked from the library and invoke the core service API calls from that task.
*/
typedef struct
{
	/**
	 * Callback routine to receive a PUBLISH from the server.
	 * The client application must provide this routine for the instances where it has
	 * subscribed to certain set of topics from the server. The callback is
	 * invoked in the context of the MQTT service task.
	 *
	 * \param[in] ctx 			Application handle returned
	 * \param[in] topStr 		Name of topic published by the server. Not NUL terminated.
	 * \param[in] topLen 		Length of the topic name published by the server.
	 * \param[in] payload 		Payload published by the server.
	 * \param[in] payloadLenth	Length of the payload.
	 * \param[in] dup 			Assert to indicate that it is re-send by the server
	 * \param[in] qoS 			Quality of service of the published message
	 * \param[in] retain 		Asserted to indicate that a retained message has been published
	 * \param[in] messageId		Message Id
	 *
	 * \return none.
	 */
	void (*svcMqttRecv)(void *ctx, const char *topStr, uint32_t topLen,
                        const char *payload, uint32_t pay_len,
                        uint8_t dup, uint8_t qos, uint8_t retain, uint16_t messageId);

	/**
	 * Indication of events from the MQTT service.
	 * These events are notified as part of the processing carried out by the
	 * MQTT service task. The client application must populate the callback to receive
	 * events about the progress made by the MQTT service.
	 *
	 * This handler is used by the MQTT service to report acknowledgements from the server.
	 *
	 * \param[in] ctx 			Application handle returned
	 * \param[in] evt 			Identifier of the reported event.
	 * \param[in] buf 			Pointer to packet buffer
	 * \param[in] len 			Length of buffer
	 *
	 * \return none.
	 */
	void (*svcMqttEvent)(void *ctx, uint32_t evt, const void *buf, uint32_t len);

	/**
	 * Notifies the client application about the termination of MQTT connection.
	 * After servicing this callback, the application can destroy the associated
	 * context if it no longer required
	 *
	 * \param[in] ctx 			Application handle returned
	 *
	 * \return none.
	 */
	void (*svcMqttDisconn)(void *ctx);
} svcMqttClientCbs_t;

/**
* MQTT CONNACK - Acknowledge connection request structure definition
*/
struct mqttConnAck_def
{
    uint8_t type;                               // Message type (0x2x)
    uint8_t remainingLength;                    // Remaining length
    uint8_t reserved;                           // Reserved
    uint8_t returnCode;                         // Return code
} __attribute__((__packed__));
typedef struct mqttConnAck_def mqttConnAck_t;

/**
* MQTT ACK - Acknowledge request structure definition
*/
struct mqttAck_def
{
    uint8_t type;                               // Message type (0x2x)
    uint8_t remainingLength;                    // Remaining length
    uint8_t msgIdMsb;                           // Message ID MSB
    uint8_t msgIdLsg;                           // Message ID LSB
} __attribute__((__packed__));
typedef struct mqttAck_def mqttAck_t;

/**
* MQTT initialization parameter structure definition
*/
typedef struct mqtt_init_parms_def
{
	socket_desc_t socket;						// Transport descriptor
    char* 		  clientid;   					// Client id
    uint16_t	  topicSize;					// Topic size
    uint16_t	  bufferSize;					// Message buffer size
} mqtt_init_parms_t;

/**
* MQTT connection parameter structure definition
*/
typedef struct mqtt_connection_parms_def
{
	void* 				broker;					// MQTT broker context
    char*				serverAddr;   			// Server address
    uint8_t				addrType;				// Address type (IP or host name)
    uint16_t			serverPort;				// Server port
    uint16_t			keepAlive;				// Keep alive time (seconds)
    uint8_t				clean;					// Clean session
    svcMqttClientCbs_t	cbs;					// Client callbacks
} mqtt_connection_parms_t;

/**
* MQTT authorization parameter structure definition
*/
typedef struct mqtt_auth_parms_def
{
	void* 			 broker;					// MQTT broker context
    char* 		     userName;   				// User name
    char*			 password;					// Password
} mqtt_auth_parms_t;

/**
* MQTT set keep-alive parameter structure definition
*/
typedef struct mqtt_keepalive_parms_def
{
	void*	 		 broker;					// MQTT broker context
    uint16_t		 keepAlive;   				// Keep alive time (seconds)
} mqtt_keepalive_parms_t;

/**
* MQTT connect parameter structure definition
*/
typedef struct mqtt_connect_parms_def
{
	void* 			 broker;					// MQTT broker context
} mqtt_connect_parms_t;

/**
* MQTT disconnect parameter structure definition
*/
typedef struct mqtt_disconnect_parms_def
{
	void* 			 broker;					// MQTT broker context
} mqtt_disconnect_parms_t;

/**
* MQTT publish parameter structure definition
*/
typedef struct mqtt_publish_parms_def
{
	void* 			 broker;					// MQTT broker context
	const char*      topic;						// Message topic
	const char*	     msg;						// Message
	uint8_t			 retain;					// Enable / disable retain flag
} mqtt_publish_parms_t;

/**
* MQTT publish with QoS parameter structure definition
*/
typedef struct mqtt_publish_qus_parms_def
{
	void*			 broker;					// MQTT broker context
	const char*      topic;						// Message topic
	const char*	     msg;						// Message
	uint8_t			 retain;					// Enable / disable retain flag
	uint8_t			 qos;						// Quality of service (0, 1, 2)
	uint16_t		 messageId;					// Message ID
} mqtt_publish_qos_parms_t;

/**
* MQTT publish release parameter structure definition
*/
typedef struct mqtt_publish_rel_parms_def
{
	void*			 broker;					// MQTT broker context
	uint16_t		 messageId;					// Message ID
} mqtt_publish_rel_parms_t;

/**
* MQTT publish ack parameter structure definition
*/
typedef struct mqtt_publish_ack_parms_def
{
	void*			 broker;					// MQTT broker context
	uint16_t		 messageId;					// Message ID
} mqtt_publish_ack_parms_t;

/**
* MQTT publish complete parameter structure definition
*/
typedef struct mqtt_publish_comp_parms_def
{
	void*			 broker;					// MQTT broker context
	uint16_t		 messageId;					// Message ID
} mqtt_publish_comp_parms_t;

/**
* MQTT subscribe to a topic parameter structure definition
*/
typedef struct mqtt_subscribe_parms_def
{
	void*			 broker;					// MQTT broker context
	const char*      topic;						// Message topic
	uint8_t			 qos;						// Quality of service
	uint16_t		 messageId;					// Message ID
} mqtt_subscribe_parms_t;

/**
* MQTT unsubscribe from a topic parameter structure definition
*/
typedef struct mqtt_unsubscribe_parms_def
{
	void*			 broker;					// MQTT broker context
	const char*      topic;						// Message topic
	uint16_t		 messageId;					// Message ID
} mqtt_unsubscribe_parms_t;

/**
* MQTT ping request parameter structure definition
*/
typedef struct mqtt_ping_req_parms_def
{
	void*			 broker;					// MQTT broker context
} mqtt_ping_req_parms_t;

/**
* MQTT deinitialize parameter structure definition
*/
typedef struct mqtt_deinitialize_parms_def
{
	void*			 broker;					// MQTT broker context
} mqtt_deinitialize_parms_t;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_MQTT_MQTT_SERVICE_H_ */
