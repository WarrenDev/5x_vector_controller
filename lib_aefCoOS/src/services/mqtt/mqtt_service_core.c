
/**
* mqtt_service_core.c
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  MQTT service core implementation.
*
*/

#include "mqtt_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>
#include <aef/embedded/system/system_management.h>
#include <aef/embedded/osal/time.h>
#include <aef/embedded/osal/time_delay.h>
#include <string.h>

#include "bsp.h"

/**
* MQTT broker context structure definition
*/
typedef struct
{
	service_vtable_t* netsrv;					// Transport
	socket_desc_t socket;						// Transport descriptor
    uint8_t* buffer;                            // Message buffer memory
    uint8_t* recvBuffer;						// Message receive memory
    uint16_t bufferSize;                        // Size of message buffer memory
    uint8_t* varHeader;                         // Variable length header memory
    uint16_t varHeaderSize;                     // Size of variable length header memory
    // Connection information
    char serverAddr[MQTT_SERVER_NAME_LENGTH];	// Server address
    uint8_t	addrType;							// Server address type (IP or host name)
    uint16_t serverPort;						// Server port
    struct sockaddr resolvedServerAddr;			// Resolved server address
    svcMqttClientCbs_t cbs;						// Callback routines
    char clientid[MQTT_CONF_CLIENTID_LENGTH];   // Client id
    // Authentication fields
    char username[MQTT_CONF_USERNAME_LENGTH];   // User name
    char password[MQTT_CONF_PASSWORD_LENGTH];   // Password
    // Will topic
    uint8_t will_retain;                        // Retain session flag
    uint8_t will_qos;                           // Will QOS flag
    uint8_t clean_session;                      // Clean session flag
    // Management fields
    uint16_t seq;                               // Message sequence number
    uint16_t alive;                             // Keep alive time
    uint8_t state;								// Service task state
    uint8_t substate;							// Service task substate
    uint8_t status;								// Status flags
} mqttBrokerCtx_t;

/**
* Internal routines.
*/
static service_status_t mqtt_core_start (service_ctx_t* ctx);
static service_status_t mqtt_core_restart (service_ctx_t* ctx);
static service_status_t mqtt_core_stop (service_ctx_t* ctx);
static service_status_t mqtt_core_status (service_ctx_t* ctx);
static service_status_t mqtt_core_pause (service_ctx_t* ctx);
static service_status_t mqtt_core_continue (service_ctx_t* ctx);

static service_status_t mqtt_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size );
static service_status_t mqtt_core_set_auth (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_set_connection (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_set_keep_alive (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_get_keep_alive (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_connect (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_connect_ex (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_disconnect (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_disconnect_ex (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_publish (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_publish_with_qus (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_publish_rel (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_publish_ack (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_publish_complete (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_subscribe (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_unsubscribe (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_ping_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);
static service_status_t mqtt_core_deinitialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size);

/**
* Internal helper routines.
*/
static uint32_t mqtt_connect ( mqttBrokerCtx_t* broker );
static uint32_t mqtt_publish_with_qos ( mqttBrokerCtx_t* broker, const char* topic, const char* msg, uint8_t retain, uint8_t qos, uint16_t* message_id );
static uint32_t mqtt_subscribe_unsubscribe ( mqttBrokerCtx_t* broker, uint8_t subUnsub, const char* topic, uint8_t qos, uint16_t* message_id );
static uint32_t mqtt_disconnect ( mqttBrokerCtx_t* broker );
static int32_t  mqtt_send_packet ( mqttBrokerCtx_t* broker, uint8_t* packet, uint32_t size, uint32_t timeout );
static int32_t  mqtt_recv_packet ( mqttBrokerCtx_t* broker, uint8_t* packet, uint32_t size, uint32_t timeout );
static uint16_t mqtt_increment_sequence ( mqttBrokerCtx_t* broker );
static void     mqtt_resolve_hostname ( mqttBrokerCtx_t* broker );

/**
* MQTT task routines
*/
static void mqtt_task_connect ( mqttBrokerCtx_t* broker );
static void mqtt_task_online ( mqttBrokerCtx_t* broker );
static void mqtt_task_disconnect ( mqttBrokerCtx_t* broker );

/**
* MQTT task state identifier definitions
*/
#define	MQTT_STATE_IDLE			0x00
#define MQTT_STATE_CONNECT		0x01
#define MQTT_STATE_ONLINE		0x02
#define MQTT_STATE_DISCONNECT	0x04

/**
* Extract the message type from buffer.
*
* /param 	buffer         Pointer to the packet.
*
* /return 	Message Type byte.
*/
#define MQTTParseMessageType(buffer) ( *buffer & 0xF0 )

/**
* Indicate if it is a duplicate packet.
*
* /param 	buffer         Pointer to the packet.
*
* /return	0 Not duplicate.
* 			!=0 Duplicate.
*/
#define MQTTParseMessageDuplicate(buffer) ( *buffer & 0x08 )

/**
* Extract the message QoS level.
*
* /param 	buffer         Pointer to the packet.
*
* /return 	QoS Level (0, 1 or 2).
*/
#define MQTTParseMessageQos(buffer) ( (*buffer & 0x06) >> 1 )

/**
* Indicate if this packet has a retain flag.
*
* /param 	buffer         Pointer to the packet.
*
* /return	0 Not duplicate.
* 			!=0 Duplicate.
*/
#define MQTTParseMessageRetain(buffer) ( *buffer & 0x01 )

/**
* Parse packet buffer for number of bytes in remaining length field.
*
* Given a packet, return number of bytes in remaining length
* field in MQTT fixed header.  Can be from 1 - 4 bytes depending
* on length of message.
*
* /param buf            Pointer to the packet.
*
* /return 	number of bytes
*/
uint8_t mqtt_num_rem_len_bytes (const uint8_t* buf)
{
    uint8_t num_bytes = 1;

    if ((buf[1] & 0x80) == 0x80)
    {
    	num_bytes++;
    	if ((buf[2] & 0x80) == 0x80)
        {
            num_bytes ++;
            if ((buf[3] & 0x80) == 0x80)
            {
            	num_bytes ++;
            }
        }
    }
    return num_bytes;
}

/**
* Parse packet buffer for remaining length value.
*
* Given a packet, return remaining length value (in fixed header).
*
* /param buf            Pointer to the packet.
*
* /return remaining length
*/
uint16_t mqtt_parse_rem_len (const uint8_t* buf)
{
    uint16_t multiplier = 1;
    uint16_t value = 0;
    uint8_t digit;

    buf++;	// skip "flags" byte in fixed header

    do
    {
    	digit = *buf;
    	value += (digit & 0x7F) * multiplier;
    	multiplier *= 128;
    	buf++;
    } while ((digit & 0x80) != 0);

    return value;
}

/**
* Parse packet buffer for message id.
*
* /param buf            Pointer to the packet.
*
* /return message id
*/
uint16_t mqtt_parse_msg_id (const uint8_t* buf)
{
    uint8_t type = MQTTParseMessageType(buf);
    uint8_t qos = MQTTParseMessageQos(buf);
    uint16_t id = 0;

    if (type >= MQTT_MSG_PUBLISH && type <= MQTT_MSG_UNSUBACK)
    {
    	if (type == MQTT_MSG_PUBLISH)
        {
            if (qos != 0)
            {
                /**
                * Fixed header length + topic (UTF encoded)
                * = 1 for "flags" byte + rlb for length bytes + topic size
                */
            	uint8_t rlb = mqtt_num_rem_len_bytes(buf);
            	uint16_t offset = *(buf+1+rlb) << 8;	// topic UTF MSB
            	offset |= *(buf+1+rlb+1);				// topic UTF LSB
            	offset += (1+rlb+2);		        	// fixed header + topic size
            	id = *(buf+offset) << 8;				// id MSB
            	id |= *(buf+offset+1);					// id LSB
            }
        }
        else
        {
            /**
            * Fixed header length = 1 for "flags" byte + rlb for length bytes
            */
            uint8_t rlb = mqtt_num_rem_len_bytes(buf);
            id = *(buf+1+rlb) << 8;	// id MSB
            id |= *(buf+1+rlb+1);	// id LSB
        }
    }
    return id;
}

/**
* Parse a packet buffer for a pointer to the publish topic.
*
*  Not called directly - called by mqtt_parse_pub_topic
*/
uint16_t mqtt_parse_pub_topic_ptr (const uint8_t* buf, const uint8_t **topic_ptr)
{
    uint16_t len = 0;

    if (MQTTParseMessageType(buf) == MQTT_MSG_PUBLISH)
    {
        /**
        * fixed header length = 1 for "flags" byte + rlb for length bytes
        */
    	uint8_t rlb = mqtt_num_rem_len_bytes(buf);
    	len = *(buf+1+rlb) << 8;	// MSB of topic UTF
    	len |= *(buf+1+rlb+1);		// LSB of topic UTF

        /**
        * Start of topic = add 1 for "flags", rlb for remaining length, 2 for UTF
        */
    	*topic_ptr = (buf + (1+rlb+2));
    }
    else
    {
    	*topic_ptr = NULL;
    }
    return len;
}

/**
* Parse a packet buffer for the publish topic.
*
* Given a packet containing an MQTT publish message,
* return the message topic.
*
* /param buf            Pointer to the packet.
* /param topic          Pointer destination buffer for topic
*
* /return size in bytes of topic (0 = no publish message in buffer)
*/
uint16_t mqtt_parse_pub_topic (const uint8_t* buf, uint8_t* topic)
{
    const uint8_t* ptr;
    uint16_t topic_len = mqtt_parse_pub_topic_ptr(buf, &ptr);

    if(topic_len != 0 && ptr != NULL)
    {
    	memcpy(topic, ptr, topic_len);
    }
    return topic_len;
}

/**
* Parse a packet buffer for a pointer to the publish message.
*
*  Not called directly - called by mqtt_parse_pub_msg
*/
uint16_t mqtt_parse_pub_msg_ptr (const uint8_t* buf, const uint8_t **msg_ptr)
{
    uint16_t len = 0;

    if (MQTTParseMessageType(buf) == MQTT_MSG_PUBLISH)
    {
        /**
        * Message starts at fixed header length + Topic (UTF encoded) + msg id (if QoS>0)
        */
    	uint8_t rlb = mqtt_num_rem_len_bytes(buf);
    	uint16_t offset = (*(buf+1+rlb)) << 8;	// topic UTF MSB
    	offset |= *(buf+1+rlb+1);				// topic UTF LSB
    	offset += (1+rlb+2);					// fixed header + topic size

    	if (MQTTParseMessageQos(buf))
        {
            offset += 2;						// add two bytes of msg id
        }

    	*msg_ptr = (buf + offset);

        /**
        * Offset is now pointing to start of message
        * Length of the message is remaining length - variable header
        * Variable header is offset - fixed header
        * fixed header is 1 + rlb
        * lom = remlen - (offset - (1+rlb))
        */
        len = mqtt_parse_rem_len(buf) - (offset-(rlb+1));
    }
    else
    {
    	*msg_ptr = NULL;
    }
    return len;
}

/**
* Parse a packet buffer for the publish message.
*
* Given a packet containing an MQTT publish message,
* return the message.
*
* /param buf            Pointer to the packet.
* /param msg            Pointer destination buffer for message
*
* /return size in bytes of topic (0 = no publish message in buffer)
*/
uint16_t mqtt_parse_publish_msg (const uint8_t* buf, uint8_t* msg)
{
    const uint8_t* ptr;

    uint16_t msg_len = mqtt_parse_pub_msg_ptr(buf, &ptr);

    if(msg_len != 0 && ptr != NULL)
    {
    	memcpy(msg, ptr, msg_len);
    }
    return msg_len;
}

/**
* FNET DNS address resolved callback - typedef void(* fnet_dns_callback_resolved_t)
*
* DNS-client callback function that is called when the DNS client has completed the resolving
*
* /param	addr_let		Pointer to the list of resolved addresses or NULL
* /param	addr_list_size	Number of resolved address
* /param	param			Parameter
*/
void mqtt_dns_resolved_addr_handler (const struct fnet_dns_resolved_addr *addr_list, fnet_size_t addr_list_size, fnet_uint32_t param)
{
	if ( addr_list != NULL && addr_list_size > 0 )
	{
		mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)param;
		memcpy ( &broker->resolvedServerAddr, &addr_list->resolved_addr, sizeof(struct sockaddr) );
		broker->resolvedServerAddr.sa_family = AF_INET;
		broker->resolvedServerAddr.sa_port   = FNET_HTONS(broker->serverPort);
		broker->status |= MQTT_STATUS_ADDR_RESOLVED;
	}
}

/**
* MQTT connection keep-alive task handler
*
* This task routine is called in the online state to run keep-alive processing
*
* \param	broker			Pointer to the broker context
*
* \returns  none
*/
void mqtt_task_keepalive ( mqttBrokerCtx_t* brokerCtx )
{
	static uint64_t OSTime = 0L;
	static uint64_t KeepAliveDelay = 0L;

	if ( (brokerCtx->status & MQTT_KEEP_ALIVE_ENABLED) )
	{
		OSTime = time_get_ticks(NULL);
		if ( KeepAliveDelay == 0L )
		{
			KeepAliveDelay = OSTime + ((brokerCtx->alive * 1000)*CFG_SYSTICK_FREQ + 500)/1000;
		}
		if ( OSTime > KeepAliveDelay )
		{
		    uint8_t packet[2] =
		    {
		    	MQTT_MSG_PINGREQ, // Message Type, DUP flag, QoS level, Retain
				0x00              // Remaining length
		    };
		    mqtt_send_packet ( brokerCtx, packet, sizeof(packet), 0L );
			KeepAliveDelay = 0L;
		}
	}
}

/**
* MQTT service task
*
* This is a system management task routine.
* This routine is registered with the system management task.
* The system management task will call this routine to run the MQTT service
*
* \param    instance		Pointer to instance data
*
* \returns  none
*/
void mqtt_service_task ( void* instance )
{
	if ( instance != NULL )
	{
		mqttBrokerCtx_t* brokerCtx = (mqttBrokerCtx_t*)instance;

		switch (brokerCtx->state)
		{
			case MQTT_STATE_CONNECT:
				mqtt_task_connect ( brokerCtx );
				break;
			case MQTT_STATE_ONLINE:
				mqtt_task_online ( brokerCtx );
				mqtt_task_keepalive ( brokerCtx );
				break;
			case MQTT_STATE_DISCONNECT:
				mqtt_task_disconnect ( brokerCtx );
				break;
			case MQTT_STATE_IDLE:
			default:
				break;
		}
	}
	return;
}

/**
* Initialize the MQTT service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_init (service_ctx_t* ctx)
{
	service_manager_vtable_t* service_manager = system_get_service_manager();
	ctx->ctx = service_manager->getservice(SRV_FNET_NETWORK);
	if ( ctx->ctx != NULL)
	{
		ctx->state = SERVICE_START_PENDING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* De-initialize the MQTT service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_deinit (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		mqtt_core_stop ( ctx );
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send a command to a system service instance.
*
* \param    ctx				Pointer to the service context
* \param    code			I/O control code to perform
* \param    input_buffer	Pointer to the input buffer
* \param    input_size		Input buffer size
* \param    output_buffer	Pointer to the output buffer
* \param    output_size		Output buffer size
* \param    bytes_read		Pointer to the actual bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
service_status_t
mqtt_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( ctx->state == SERVICE_UNINITIALIZED )
		return SERVICE_FAILURE_OFFLINE;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = mqtt_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = mqtt_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = mqtt_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = mqtt_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = mqtt_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = mqtt_core_continue(ctx);
			break;
		case IOCTL_MQTT_INITIALIZE:
			status = mqtt_core_initialize(ctx, input_buffer, input_size, output_buffer, output_size);
			break;
		case IOCTL_MQTT_SET_AUTH:
			status = mqtt_core_set_auth(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_SET_CONNECTION:
			status = mqtt_core_set_connection(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_SET_KEEP_ALIVE:
			status = mqtt_core_set_keep_alive(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_GET_KEEP_ALIVE:
			status = mqtt_core_get_keep_alive(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_CONNECT:
			status = mqtt_core_connect(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_CONNECT_EX:
			status = mqtt_core_connect_ex(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_DISCONNECT:
			status = mqtt_core_disconnect(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_DISCONNECT_EX:
			status = mqtt_core_disconnect_ex(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_PUBLISH:
			status = mqtt_core_publish(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_PUBLISH_WITH_QOS:
			status = mqtt_core_publish_with_qus(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_PUBLISH_REL:
			status = mqtt_core_publish_rel(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_PUBLISH_ACK:
			status = mqtt_core_publish_ack (ctx, input_buffer,input_size);
			break;
		case IOCTL_MQTT_PUBLISH_COMPLETE:
			status = mqtt_core_publish_complete (ctx, input_buffer,input_size);
			break;
		case IOCTL_MQTT_SUBSCRIBE:
			status = mqtt_core_subscribe(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_UNSUBSCRIBE:
			status = mqtt_core_unsubscribe(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_PING_REQ:
			status = mqtt_core_ping_req(ctx, input_buffer, input_size);
			break;
		case IOCTL_MQTT_DEINITIALIZE:
			status = mqtt_core_deinitialize(ctx, input_buffer, input_size);
			break;
		default:
			break;
	}

	return status;
}

/**
* Start service control function.
* Starts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_start (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		ctx->state = SERVICE_START_PENDING;
		ctx->state = SERVICE_RUNNING;

		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Restart service control function.
* Restarts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_restart (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Stop service control function.
* Stops the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		ctx->state = SERVICE_STOPPED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Status service control function.
* Retrieves status of the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_status (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Pause service control function.
* Pauses the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_pause (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		ctx->state = SERVICE_PAUSED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Continue service control function.
* Continues the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t mqtt_core_continue (service_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		ctx->state = SERVICE_RUNNING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the MQTT broker context.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to initialization parameters
* \param	input_size		Size of initialization parameters
* \param	output_buffer	Pointer to broker context memory area
* \param	output_size		Size of broker context memory area
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t
mqtt_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_init_parms_t) && output_buffer != NULL && output_size == sizeof(mqttBrokerCtx_t*) )
			{
				mqtt_init_parms_t* init_params = (mqtt_init_parms_t*)input_buffer;
				mqttBrokerCtx_t** brokerCtxPtr = (mqttBrokerCtx_t**)output_buffer;
				mqttBrokerCtx_t* brokerCtx = (mqttBrokerCtx_t*) malloc ( sizeof(mqttBrokerCtx_t) );
				if ( brokerCtx != NULL )
				{
					memset ( brokerCtx, 0, sizeof(mqttBrokerCtx_t) );

					/**
					 * Set up message memory Buffer
					 */
					brokerCtx->bufferSize = init_params->bufferSize;
					brokerCtx->buffer     = malloc ( brokerCtx->bufferSize );
					brokerCtx->recvBuffer = malloc ( brokerCtx->bufferSize );

					/**
					 * Set up variable length header memory buffer
					 */
					brokerCtx->varHeaderSize = init_params->topicSize + 6;
					brokerCtx->varHeader = malloc ( brokerCtx->varHeaderSize );

					/**
					 * Set up connection options
					 */
					brokerCtx->alive 		 = 60;	// Default keep-alive time
					brokerCtx->seq   		 = 1;	// Sequence number
					brokerCtx->clean_session = 1;	// Discard previous session

					/**
					 * Set up client id
					 */
					strcpy ( brokerCtx->clientid, init_params->clientid );

					/**
					 * Set up transport
					 */
					service_manager_vtable_t* service_manager = system_get_service_manager();
					brokerCtx->netsrv             = service_manager->getservice(SRV_FNET_NETWORK);
					brokerCtx->socket.fnet_socket = init_params->socket.fnet_socket;

					/**
					 * Register the system management task
					 */
					brokerCtx->state = MQTT_STATE_IDLE;
		            system_management_func_attach (ctx->name, brokerCtx, mqtt_service_task);

					/**
					 * Return the broker context
					 */
					*brokerCtxPtr = brokerCtx;

					return SERVICE_STATUS_SUCCESS;
				}
				return SERVICE_FAILURE_INITIALIZATION;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize MQTT authorization values
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to authorization parameters
* \param	input_size		Size of authorization parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_set_auth (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_auth_parms_t) )
			{
				mqtt_auth_parms_t* auth_params = (mqtt_auth_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)auth_params->broker;
				if ( auth_params->userName && auth_params->userName[0] != '\0' )
				{
					strcpy ( broker->username, auth_params->userName );
				}
				if ( auth_params->password && auth_params->password[0] != '\0' )
				{
					strcpy ( broker->password, auth_params->password );
				}
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize MQTT connection parameters
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to connection parameters
* \param	input_size		Size of connection parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_set_connection (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_connection_parms_t) )
			{
				mqtt_connection_parms_t* connection_params = (mqtt_connection_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = connection_params->broker;
				strcpy ( broker->serverAddr, connection_params->serverAddr);
				broker->serverPort    	 	= connection_params->serverPort;
				broker->addrType      	 	= connection_params->addrType;
				broker->alive      	  	 	= connection_params->keepAlive;
				broker->clean_session 	 	= connection_params->clean;
				broker->cbs.svcMqttEvent 	= connection_params->cbs.svcMqttEvent;
				broker->cbs.svcMqttRecv  	= connection_params->cbs.svcMqttRecv;
				broker->cbs.svcMqttDisconn 	= connection_params->cbs.svcMqttDisconn;

				if ( broker->addrType == MQTT_ADDR_IP )
				{
					memset ( &broker->resolvedServerAddr, 0, sizeof(struct sockaddr) );
					fnet_inet_ptos ( broker->serverAddr, &broker->resolvedServerAddr );
					broker->resolvedServerAddr.sa_family = AF_INET;
					broker->resolvedServerAddr.sa_port   = FNET_HTONS(broker->serverPort);
					broker->status |= MQTT_STATUS_ADDR_RESOLVED;
				}
				else
				{
					mqtt_resolve_hostname ( broker );
				}
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Set MQTT keep alive time
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to keep alive parameters
* \param	input_size		Size of keep alive parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_set_keep_alive (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_keepalive_parms_t) )
			{
				mqtt_keepalive_parms_t* keepalive_params = (mqtt_keepalive_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)keepalive_params->broker;
				broker->alive = keepalive_params->keepAlive;
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Get MQTT keep alive time
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to keep alive parameters
* \param	input_size		Size of keep alive parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_get_keep_alive (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_keepalive_parms_t) )
			{
				mqtt_keepalive_parms_t* keepalive_params = (mqtt_keepalive_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)keepalive_params->broker;
				keepalive_params->keepAlive = broker->alive;
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Establish a connection with the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to connection parameters
* \param	oinput_size		Size of connection parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_connect (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_connect_parms_t) )
			{
				mqtt_connect_parms_t* connect_params = (mqtt_connect_parms_t*)input_buffer;
				uint32_t result = mqtt_connect ( connect_params->broker );
				return ( (result) ? SERVICE_STATUS_SUCCESS : SERVICE_FAILURE_GENERAL );
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Async establish a connection with the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to connection parameters
* \param	oinput_size		Size of connection parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_connect_ex (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_connect_parms_t) )
			{
				mqtt_connect_parms_t* connect_params = (mqtt_connect_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)connect_params->broker;
				broker->state = MQTT_STATE_CONNECT;
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Disconnect from the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to disconnection parameters
* \param	input_size		Size of disconnection parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_disconnect (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_disconnect_parms_t) )
			{
				mqtt_disconnect_parms_t* disconnect_params = (mqtt_disconnect_parms_t*)input_buffer;
				if ( mqtt_disconnect ( disconnect_params->broker ) )
			    	return SERVICE_STATUS_SUCCESS;
			    else
			    	return SERVICE_FAILURE_GENERAL;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Async disconnect from the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to disconnection parameters
* \param	input_size		Size of disconnection parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_disconnect_ex (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_disconnect_parms_t) )
			{
				mqtt_disconnect_parms_t* disconnect_params = (mqtt_disconnect_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)disconnect_params->broker;
				broker->state = MQTT_STATE_DISCONNECT;
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Publish to the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to publish parameters
* \param	input_size		Size of publish parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_publish (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_publish_parms_t) )
			{
				mqtt_publish_parms_t* publish_params = (mqtt_publish_parms_t*)input_buffer;
				uint32_t result = mqtt_publish_with_qos ( publish_params->broker,
														  publish_params->topic,
														  publish_params->msg,
														  publish_params->retain,
														  MQTT_QOS0,
														  NULL );
				return ( (result) ? SERVICE_STATUS_SUCCESS : SERVICE_FAILURE_GENERAL );
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Publish with quality of service to the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to publish parameters
* \param	input_size		Size of publish parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_publish_with_qus (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_publish_qos_parms_t) )
			{
				mqtt_publish_qos_parms_t* publish_qos_params = (mqtt_publish_qos_parms_t*)input_buffer;
				uint32_t result = mqtt_publish_with_qos ( publish_qos_params->broker,
														  publish_qos_params->topic,
														  publish_qos_params->msg,
														  publish_qos_params->retain,
														  publish_qos_params->qos,
														  &publish_qos_params->messageId );
				return ( (result) ? SERVICE_STATUS_SUCCESS : SERVICE_FAILURE_GENERAL );
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send publish release to the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to publish release parameters
* \param	input_size		Size of publish release parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_publish_rel (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_publish_rel_parms_t) )
			{
				mqtt_publish_rel_parms_t* publish_rel_params = (mqtt_publish_rel_parms_t*)input_buffer;
			    uint8_t packet[] =
			    {
			    	MQTT_MSG_PUBREL | MQTT_QOS1_FLAG, 		// Message Type, DUP flag, QoS level, Retain
					0x02,                             		// Remaining length
					publish_rel_params->messageId >> 8,
					publish_rel_params->messageId & 0xFF
			    };
			    if ( mqtt_send_packet ( publish_rel_params->broker, packet, sizeof(packet), 0L ) )
			    	return SERVICE_STATUS_SUCCESS;
			    else
			    	return SERVICE_FAILURE_GENERAL;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send publish ack to the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to publish release parameters
* \param	input_size		Size of publish release parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_publish_ack (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_publish_ack_parms_t) )
			{
				mqtt_publish_ack_parms_t* publish_ack_params = (mqtt_publish_ack_parms_t*)input_buffer;
			    uint8_t packet[] =
			    {
			    	MQTT_MSG_PUBACK, 						// Message Type, DUP flag, QoS level, Retain
					0x02,                             		// Remaining length
					publish_ack_params->messageId >> 8,
					publish_ack_params->messageId & 0xFF
			    };
			    if ( mqtt_send_packet ( publish_ack_params->broker, packet, sizeof(packet), 0L ) )
			    	return SERVICE_STATUS_SUCCESS;
			    else
			    	return SERVICE_FAILURE_GENERAL;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send publish complete to the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to publish release parameters
* \param	input_size		Size of publish release parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_publish_complete (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_publish_comp_parms_t) )
			{
				mqtt_publish_comp_parms_t* publish_comp_params = (mqtt_publish_comp_parms_t*)input_buffer;
			    uint8_t packet[] =
			    {
			    	MQTT_MSG_PUBCOMP, 						// Message Type, DUP flag, QoS level, Retain
					0x02,                             		// Remaining length
					publish_comp_params->messageId >> 8,
					publish_comp_params->messageId & 0xFF
			    };
			    if ( mqtt_send_packet ( publish_comp_params->broker, packet, sizeof(packet), 0L ) )
			    	return SERVICE_STATUS_SUCCESS;
			    else
			    	return SERVICE_FAILURE_GENERAL;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Subscribe to a topic on the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to subscribe parameters
* \param	input_size		Size of subscribe parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_subscribe (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_subscribe_parms_t) )
			{
				mqtt_subscribe_parms_t* subscribe_params = (mqtt_subscribe_parms_t*)input_buffer;
				uint32_t result = mqtt_subscribe_unsubscribe ( subscribe_params->broker, MQTT_MSG_SUBSCRIBE,
															   subscribe_params->topic,
															   subscribe_params->qos,
															   &subscribe_params->messageId );
				return ( (result) ? SERVICE_STATUS_SUCCESS : SERVICE_FAILURE_GENERAL );
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Unsubscribe from a topic on the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to unsubscribe parameters
* \param	input_size		Size of unsubscribe parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_unsubscribe (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_unsubscribe_parms_t) )
			{
				mqtt_unsubscribe_parms_t* unsubscribe_params = (mqtt_unsubscribe_parms_t*)input_buffer;
				uint32_t result = mqtt_subscribe_unsubscribe ( unsubscribe_params->broker, MQTT_MSG_UNSUBSCRIBE,
															   unsubscribe_params->topic,
															   0,
															   &unsubscribe_params->messageId );
				return ( (result) ? SERVICE_STATUS_SUCCESS : SERVICE_FAILURE_GENERAL );
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Send a ping request to the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to ping request parameters
* \param	input_size		Size of ping request parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_ping_req (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_ping_req_parms_t) )
			{
				mqtt_ping_req_parms_t* ping_params = (mqtt_ping_req_parms_t*)input_buffer;
			    uint8_t packet[2] =
			    {
			    	MQTT_MSG_PINGREQ, // Message Type, DUP flag, QoS level, Retain
					0x00              // Remaining length
			    };
			    if ( mqtt_send_packet ( ping_params->broker, packet, sizeof(packet), 0L ) )
			    	return SERVICE_STATUS_SUCCESS;
			    else
			    	return SERVICE_FAILURE_GENERAL;
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* De-initialize the MQTT broker
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to ping request parameters
* \param	input_size		Size of ping request parameters
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
* 			SERVICE_FAILURE_INVALID_PARAMETER if parameters are incorrect
* 			SERVICE_FAILURE_OFFLINE if service is not running
*           SERVICE_FAILURE_GENERAL on service context error
*/
service_status_t mqtt_core_deinitialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( ctx->state == SERVICE_RUNNING )
		{
			if ( input_buffer != NULL && input_size == sizeof(mqtt_deinitialize_parms_t) )
			{
				mqtt_deinitialize_parms_t* deinit_params = (mqtt_deinitialize_parms_t*)input_buffer;
				mqttBrokerCtx_t* broker = (mqttBrokerCtx_t*)deinit_params->broker;
				if ( deinit_params->broker != NULL )
				{
					/**
					* Deregister the system management task
					*/
		            system_management_func_detach (mqtt_service_task);

					/**
					* Release memory buffers
					*/
		            free ( broker->varHeader );
		            free ( broker->recvBuffer );
		            free ( broker->buffer );

		            /**
		            * Release broker context memory
		            */
					free ( deinit_params->broker );
			    	return SERVICE_STATUS_SUCCESS;
				}
			}
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
		return SERVICE_FAILURE_OFFLINE;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Connect to the broker.
*
* \param 	broker         Pointer to the broker context
*
* \returns	Number of bytes received.
*/
uint32_t mqtt_connect (mqttBrokerCtx_t* broker)
{
    uint8_t flags = 0x00;

    uint16_t clientidlen = strlen(broker->clientid);
    uint16_t usernamelen = strlen(broker->username);
    uint16_t passwordlen = strlen(broker->password);
    uint16_t payload_len = clientidlen + 2;

    /**
    * Prepare flags
    */
    if (usernamelen)
    {
    	payload_len += usernamelen + 2;
    	flags |= MQTT_USERNAME_FLAG;
    }
    if (passwordlen)
    {
    	payload_len += passwordlen + 2;
    	flags |= MQTT_PASSWORD_FLAG;
    }
    if (broker->clean_session || clientidlen == 0 )
    {
    	flags |= MQTT_CLEAN_SESSION;
    }

    /**
    * Variable header
    */
    uint8_t var_header[] =
    {
    	0x00,0x04,'M','Q','T','T',  // Protocol name: MQTT (MQTT 3.1.1)
		0x04,                       // Protocol version
		flags,                      // Connect flags
		broker->alive >> 8, broker->alive & 0xFF, // Keep alive
    };

//    uint8 var_header[] =
//    {
//	0x00,0x06,0x4d,0x51,0x49,0x73,0x64,0x70,  // Protocol name: MQIsdp
//	0x03,           // Protocol version
//	flags,          // Connect flags
//	broker->alive >> 8, broker->alive & 0xFF, // Keep alive
//    };

    /**
    * Fixed header
    */
    uint8_t fixed_header[3];              // Allocating space for the maximum size fixed header
    uint8_t fixedHeaderSize = 2;          // Default size = one byte Message Type + one byte Remaining Length
    uint8_t remainLen = sizeof(var_header)+payload_len;
    fixed_header[0] = MQTT_MSG_CONNECT; // Message type

    /**
    * Remaining length
    */
    if (remainLen <= 127)
    {
        fixed_header[1] = remainLen;
    }
    else
    {
        fixedHeaderSize++;              // add an additional byte for Remaining Length
        // first byte is remainder (mod) of 128, then set the MSB to indicate more bytes
        fixed_header[1] = remainLen % 128;
        fixed_header[1] = fixed_header[1] | 0x80;
        // second byte is number of 128s
        fixed_header[2] = remainLen / 128;
    }

    uint16_t offset = 0;
    uint16_t packetsize = fixedHeaderSize + sizeof(var_header) + payload_len;
    uint8_t* packet = broker->buffer;
    memset(packet, 0, packetsize);
    memcpy(packet, fixed_header, fixedHeaderSize);
    offset += fixedHeaderSize;
    memcpy(packet+offset, var_header, sizeof(var_header));
    offset += sizeof(var_header);
    // Client ID - UTF encoded
    packet[offset++] = clientidlen >> 8;
    packet[offset++] = clientidlen & 0xFF;
    memcpy(packet+offset, broker->clientid, clientidlen);
    offset += clientidlen;

    if (usernamelen)
    {
    	// Username - UTF encoded
    	packet[offset++] = usernamelen >> 8;
    	packet[offset++] = usernamelen & 0xFF;
    	memcpy(packet+offset, broker->username, usernamelen);
    	offset += usernamelen;
    }

    if (passwordlen)
    {
    	// Password - UTF encoded
    	packet[offset++] = passwordlen >> 8;
    	packet[offset++] = passwordlen & 0xFF;
    	memcpy(packet+offset, broker->password, passwordlen);
    	offset += passwordlen;
    }

    /**
    * Send the packet
    */
    return mqtt_send_packet ( broker, packet, packetsize, 0L );
}

/**
* Publish a message on a topic with QoS.
*
* \param 	broker         Pointer to the broker context
* \param 	topic          Pointer to the topic name.
* \param 	msg            Pointer to the message.
* \param 	retain         Enable or disable the Retain flag (values: 0 or 1).
* \param 	qos            Quality of Service (values: 0, 1 or 2)
* \param 	message_id     Variable that will store the Message ID, if the pointer is not NULL.
*
* \returns	Number of bytes sent.
*/
uint32_t mqtt_publish_with_qos (mqttBrokerCtx_t* broker, const char* topic, const char* msg, uint8_t retain, uint8_t qos, uint16_t* message_id)
{
    uint16_t topiclen = strlen(topic);
    uint16_t msglen   = strlen(msg);

    uint8_t qos_flag = MQTT_QOS0_FLAG;
    uint8_t qos_size = 0; // No QoS included
    if ( qos == MQTT_QOS1 )
    {
    	qos_size = 2; // 2 bytes for QoS
    	qos_flag = MQTT_QOS1_FLAG;
    }
    else if ( qos == MQTT_QOS2 )
    {
    	qos_size = 2; // 2 bytes for QoS
    	qos_flag = MQTT_QOS2_FLAG;
    }

    /**
    * Variable header
    */
    uint16_t varheadersize = topiclen+2+qos_size; // Topic size (2 bytes), utf-encoded topic
    uint8_t* var_header = broker->varHeader;
    memset (var_header, 0, varheadersize);
    var_header[0] = topiclen >> 8;
    var_header[1] = topiclen&0xFF;
    memcpy ( var_header+2, topic, topiclen );
    if ( qos_size )
    {
    	var_header[topiclen+2] = broker->seq>>8;
    	var_header[topiclen+3] = broker->seq&0xFF;
    	if ( message_id )
    	{
    		*message_id = broker->seq;	// Returning message id
    	}
    }

    /**
    * Fixed header
    * The remaining length is one byte for messages up to 127 bytes, then two bytes after that
    * Actually, it can be up to 4 bytes but making the assumption the embedded device will only
    * need up to two bytes of length (handles up to 16,383 sized message)
    */
    uint8_t fixed_header[3];              // Allocating space for the maximum size fixed header
    uint8_t fixedHeaderSize = 2;          // Default size = one byte Message Type + one byte Remaining Length
    uint8_t remainLen = varheadersize + msglen;
    fixed_header[0] = MQTT_MSG_PUBLISH | qos_flag; // Message type, DUP flag, Qos level, Retain
    if ( retain )
    {
        fixed_header[0] |= MQTT_RETAIN_FLAG;
    }

    /**
    * Remaining length
    */
    if ( remainLen <= 127 )
    {
        fixed_header[1] = remainLen;
    }
    else
    {
        fixedHeaderSize++;              // add an additional byte for Remaining Length
        // first byte is remainder (mod) of 128, then set the MSB to indicate more bytes
        fixed_header[1] = remainLen % 128;
        fixed_header[1] = fixed_header[1] | 0x80;
        // second byte is number of 128s
        fixed_header[2] = remainLen / 128;
    }

    uint16_t packetsize = fixedHeaderSize + varheadersize + msglen;
    uint8_t* packet = broker->buffer;
    memset (packet, 0, packetsize);
    memcpy (packet, fixed_header, fixedHeaderSize);
    memcpy (packet + fixedHeaderSize, var_header, varheadersize);
    memcpy (packet + fixedHeaderSize + varheadersize, msg, msglen);

    /**
    * Send the packet
    */
    return mqtt_send_packet ( broker, packet, packetsize, 0L );
}

/**
* Subscribe to or un-subscribe from a topic.
*
* \param 	broker         Pointer to the broker context
* \param 	topic          Pointer to the topic name.
* \param	qos			   Quality of service (used for subscribe - 0, 1, or 2)
* \param 	message_id     Variable that will store the Message ID, if the pointer is not NULL.
*
* \returns	Number of bytes sent.
*/
uint32_t mqtt_subscribe_unsubscribe (mqttBrokerCtx_t* broker, uint8_t subUnsub, const char* topic, uint8_t qos, uint16_t* message_id)
{
    uint16_t topiclen = strlen(topic);

    /**
    * Variable header
    */
    uint8_t var_header[2]; // Message ID
    var_header[0] = broker->seq >> 8;
    var_header[1] = broker->seq & 0xFF;
    if (message_id)
    {
    	*message_id = broker->seq;      // Returning message id
    }

    /**
    * utf topic
    */
    uint16_t utftopicsize = (topiclen + ((subUnsub == MQTT_MSG_SUBSCRIBE) ? 3 : 2));   // Topic size (2 bytes) + QoS byte if subscribe, otherwise topic size
    uint8_t* utf_topic = broker->varHeader;
    memset(utf_topic, 0, utftopicsize);
    utf_topic[0] = topiclen >> 8;
    utf_topic[1] = topiclen & 0xFF;
    memcpy(utf_topic+2, topic, topiclen);
    if ( subUnsub == MQTT_MSG_SUBSCRIBE )
    {
    	// Add QoS bytes if subscribe
        utf_topic[2+topiclen] = qos;
    }

    /**
    * Fixed header
    */
    uint8_t fixed_header[] =
    {
    	subUnsub | MQTT_QOS1_FLAG, // Message Type, DUP flag, QoS level, Retain
		sizeof(var_header) + utftopicsize
    };

    uint16_t packetsize = sizeof(var_header)+sizeof(fixed_header)+utftopicsize;
    uint8_t* packet = broker->buffer;
    memset(packet, 0, packetsize);
    memcpy(packet, fixed_header, sizeof(fixed_header));
    memcpy(packet+sizeof(fixed_header), var_header, sizeof(var_header));
    memcpy(packet+sizeof(fixed_header)+sizeof(var_header), utf_topic, utftopicsize);

    /**
    * Send the packet
    */
    return mqtt_send_packet ( broker, packet, packetsize, 0L );
}

/**
* Disconnect from the MQTT broker
*
* \param 	broker         Pointer to the broker context
*
* \returns	Number of bytes sent.
*/
uint32_t mqtt_disconnect (mqttBrokerCtx_t* broker )
{
	if ( broker != NULL )
	{
	    uint8_t packet[2] =
	    {
	    	MQTT_MSG_DISCONNECT,    // Message Type, DUP flag, QoS level, Retain
			0x00                    // Remaining length
	    };
	    return mqtt_send_packet ( broker, packet, sizeof(packet), 0L );
	}
	return 0L;
}

/**
* Transmit a MQTT packet over the transport
*
* \param	broker			Pointer to the broker context
* \param	packet			Pointer to the packet to send
* \param	size			Size of the packet to send
* \param	timeout			Send timeout value
*
* \returns	Number of bytes sent.
*/
int32_t mqtt_send_packet ( mqttBrokerCtx_t* broker, uint8_t* packet, uint32_t size, uint32_t timeout )
{
	(void) timeout;	// not used
	if ( broker->netsrv != NULL )
	{
		uint32_t bytes_transferred = 0L;
		send_parms_t parms = { broker->socket.fnet_socket, packet, size, 0 };
		if ( broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SEND,
				                   	     &parms,
										 sizeof(send_parms_t),
										 NULL,
										 0L,
										 &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
		{
			return bytes_transferred;
		}
	}
	return FNET_ERR;
}

/**
* Receive a MQTT packet from the transport
*
* \param	broker			Pointer to the broker context
* \param	packet			Pointer to the memory buffer for the received packet
* \param	size			Size of the memory buffer
* \param	timeout			Receive timeout value
*
* \returns	Number of bytes received.
*/
int32_t mqtt_recv_packet ( mqttBrokerCtx_t* broker, uint8_t* packet, uint32_t size, uint32_t timeout )
{
	int32_t result = FNET_ERR;
	(void) timeout;	// not used
	if ( broker->netsrv != NULL )
	{
		uint32_t bytes_read = 0;
		uint32_t bytes_transferred = 0L;

		/**
		* Let's retrieve the fixed header first
		*/
		recv_parms_t parms =
		{
			.socket = broker->socket.fnet_socket,
			.buffer = packet,
			.length = 1,
			.flags  = 0
		};
		if ( broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_RECV,
				                   	     &parms,
										 sizeof(recv_parms_t),
										 NULL,
										 0L,
										 &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
		{
			if ( bytes_transferred )
			{
				/**
				* Read and decode the remaining length field
				*/
				uint32_t multiplier = 1;
				uint32_t remainingLength = 0L;

				do
				{
					bytes_read += bytes_transferred;
					parms.buffer = packet + bytes_read;
					bytes_transferred = 0L;
					broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_RECV,
					                    	    &parms,
												sizeof(recv_parms_t),
												NULL,
												0L,
												&bytes_transferred );
					if (bytes_transferred == 0)
						return 0;

					remainingLength += (uint32_t)( packet[bytes_read] & 0x7F ) * multiplier;
					multiplier *= 128;
					if ( multiplier > (128L*128L*128L) )
						return 0;
				} while ( (packet[bytes_read] & 0x80) != 0 );

				/**
				* Now let's retrieve the rest of the packet
				*/
				bytes_read   += bytes_transferred;
				parms.buffer = packet + bytes_read;
				parms.length = remainingLength;
				if ( broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_RECV,
						                   	     &parms,
												 sizeof(recv_parms_t),
												 NULL,
												 0L,
												 &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
				{
					result = ( bytes_transferred && (parms.length == bytes_transferred) ) ? (bytes_read + bytes_transferred) : 0;
				}
			}
			else
			{
				result = bytes_transferred;
			}
		}
	}
	return result;
}

/**
* Increment the message sequence number
*
* \param	broker			Pointer to the broker context
*
* \returns	new message sequence number
*/
uint16_t mqtt_increment_sequence ( mqttBrokerCtx_t* broker )
{
	broker->seq = ( broker->seq >= 65535 ) ? 1 : (broker->seq+1);
	return broker->seq;
}

/**
* Resolve a server address
*
* Use DNS lookup to resolve a server host name to an IP address
*
* \param	broker			Pointer to the broker context
*
* \returns	none
*/
void mqtt_resolve_hostname ( mqttBrokerCtx_t* broker )
{
	dns_init_parms_t dns_params =
	{
		.addr_family = AF_INET,
		.host_name   = broker->serverAddr,
		.callback    = mqtt_dns_resolved_addr_handler,
		.param       = (uint32_t)broker,
	};
	memset ( &dns_params.dns_server_addr, 0, sizeof(struct sockaddr) );
	broker->netsrv->iocontrol ( IOCTL_FNET_STACK_DNS_INIT,
	                    	    &dns_params,
								sizeof(dns_init_parms_t),
								NULL,
								0L,
								NULL );
}

/**
* MQTT service task connect state handler
*
* Task routine called when in the connect state to run the connection process
*
* \param	broker			Pointer to the broker context
*
* \returns	none
*/
void mqtt_task_connect ( mqttBrokerCtx_t* broker )
{
	if ( broker->netsrv != NULL )
	{
		/**
		* Create the network socket
		*/
		socket_parms_t socket_parms = { AF_INET, SOCK_STREAM, IPPROTO_IP };
		uint32_t bytes_returned     = 0L;
		broker->socket.fnet_socket  = FNET_ERR;
		service_status_t result = broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET,
				                                              &socket_parms,
														      sizeof(socket_parms_t),
														      &broker->socket,
														      sizeof(socket_desc_t),
														      &bytes_returned );
		if ( (result == SERVICE_STATUS_SUCCESS) && (bytes_returned == sizeof(socket_desc_t)) )
		{
			/**
			* Connect to the server
			*/
			connect_parms_t connect_parms = { broker->socket.fnet_socket, &broker->resolvedServerAddr, sizeof(struct sockaddr) };
			if ( broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_CONNECT,
					                         &connect_parms,
									         sizeof(connect_parms_t),
									         NULL,
									         0L,
									         NULL ) == SERVICE_STATUS_SUCCESS )
			{
				/**
				* Issue the MQTT connect command
				*/
				time_delay (1000);
				if ( mqtt_connect ( broker ) > 0 )
				{
					/**
					* Move to the online state
					*/
					broker->state = MQTT_STATE_ONLINE;
				    broker->status |= MQTT_STATUS_ONLINE;
				    broker->status |= (broker->alive) ? MQTT_KEEP_ALIVE_ENABLED : 0;
				    return;
				}
			}
		}
	}

	/*
	* Encountered a problem, notify client and go to idle state
	*/
	if ( broker->cbs.svcMqttEvent )
	{
		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_ERROR, NULL, 0 );
	}
	broker->state = MQTT_STATE_IDLE;
}

/**
* MQTT service task online state handler
*
* Task routine called when in the online state to handle incoming messages
*
* \param	broker			Pointer to the broker context
*
* \returns	none
*/
void mqtt_task_online ( mqttBrokerCtx_t* broker )
{
	int size =  mqtt_recv_packet ( broker, broker->recvBuffer, broker->bufferSize, 0 );
	if ( size > 0 )
	{
        uint8_t msgType = MQTTParseMessageType(broker->recvBuffer);
        switch ( msgType )
        {
            case MQTT_MSG_ERROR:
            	if ( broker->cbs.svcMqttEvent )
            	{
            		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_ERROR, NULL, 0 );
            	}
            	broker->state = MQTT_STATE_DISCONNECT;
                break;
            case MQTT_MSG_CONNACK:
            {
                mqttConnAck_t* ack = (mqttConnAck_t*)broker->recvBuffer;
                if ( ack->returnCode == MQTT_CONNACK_ACCEPTED )
                {
				    broker->status |= MQTT_STATUS_SESSION_ACTIVE;
	            	if ( broker->cbs.svcMqttEvent )
	            	{
	            		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_CONNACK, ack, sizeof(mqttConnAck_t) );
	            	}
                }
                else
                {
                	broker->state = MQTT_STATE_DISCONNECT;
                }
            }
                break;
            case MQTT_MSG_PUBLISH:
            	if ( broker->cbs.svcMqttRecv )
            	{
            		const uint8_t* message = NULL;
            		const uint8_t* topic   = NULL;
            		uint32_t message_len   = (uint32_t) mqtt_parse_pub_msg_ptr (broker->recvBuffer, &message);
            		uint32_t topic_len     = (uint32_t) mqtt_parse_pub_topic_ptr (broker->recvBuffer, &topic);
            		uint16_t message_id	   = mqtt_parse_msg_id (broker->recvBuffer);
            		uint8_t dup            = MQTTParseMessageDuplicate(broker->recvBuffer);
            		uint8_t qos            = MQTTParseMessageQos(broker->recvBuffer);
            		uint8_t retain         = MQTTParseMessageRetain(broker->recvBuffer);
            		(*broker->cbs.svcMqttRecv)( broker, (const char*)topic, topic_len, (const char*)message, message_len, dup, qos, retain, message_id );
            	}
            	break;
            case MQTT_MSG_PUBACK:
            	mqtt_increment_sequence(broker);
            	if ( broker->cbs.svcMqttEvent )
            	{
            		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_PUBACK, NULL, 0 );
            	}
                break;
            case MQTT_MSG_PUBCOMP:
            	mqtt_increment_sequence(broker);
            	if ( broker->cbs.svcMqttEvent )
            	{
            		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_PUBCOMP, NULL, 0 );
            	}
                break;
            case MQTT_MSG_SUBACK:
            	mqtt_increment_sequence(broker);
            	if ( broker->cbs.svcMqttEvent )
            	{
            		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_SUBACK, NULL, 0 );
            	}
                break;
            case MQTT_MSG_UNSUBACK:
            	mqtt_increment_sequence(broker);
            	if ( broker->cbs.svcMqttEvent )
            	{
            		(*broker->cbs.svcMqttEvent)( broker, MQTT_CL_EVT_UNSUBACK, NULL, 0 );
            	}
                break;
            case MQTT_MSG_PUBREC:
            case MQTT_MSG_PINGRESP:
            default:
                break;
        }
	}
	if (size == FNET_ERR )
	{
		/**
		* Connection lost
		*/
	    broker->status &= ~MQTT_STATUS_SESSION_ACTIVE;
    	broker->state = MQTT_STATE_DISCONNECT;
	}
}

/**
* MQTT service task disconnect state handler
*
* Task routine called when in the disconnect state to run the disconnect process
*
* \param	broker			Pointer to the broker context
*
* \returns	none
*/
void mqtt_task_disconnect ( mqttBrokerCtx_t* broker )
{
	/**
	* Disconnect from the MQTT broker
	*/
	if ( broker->status & MQTT_STATUS_SESSION_ACTIVE )
	{
		mqtt_disconnect ( broker );
	    broker->status &= ~MQTT_STATUS_SESSION_ACTIVE;
	}

	/**
	* Close the connection with the server
	*/
	if ( broker->status & MQTT_STATUS_ONLINE )
	{
		if ( broker->netsrv != NULL )
		{
			shutdown_parms_t shutdown_parms = { broker->socket.fnet_socket, 0 };
			broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SHUTDOWN,
		  	                   	   	    &shutdown_parms,
										sizeof(shutdown_parms_t),
										NULL,
										0L,
										NULL );
		}
	    broker->status &= ~MQTT_STATUS_ONLINE;
	    broker->status &= ~MQTT_KEEP_ALIVE_ENABLED;
	}

	/**
	* Destroy the network socket
	*/
	close_parms_t close_parms = { broker->socket.fnet_socket };
	broker->netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_CLOSE,
  	                   	   	    &close_parms,
								sizeof(close_parms_t),
								NULL,
								0L,
								NULL );
	broker->socket.fnet_socket = FNET_ERR;

	/**
	* Notify client and transition to the idle state
	*/
	if ( broker->cbs.svcMqttDisconn )
	{
		(*broker->cbs.svcMqttDisconn)( broker );
	}
	broker->state = MQTT_STATE_IDLE;
}
