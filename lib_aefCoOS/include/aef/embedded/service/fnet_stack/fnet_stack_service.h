/**
* fnet_stack_service.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the FNET Network Stack system service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_FNET_STACK_FNET_STACK_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_FNET_STACK_FNET_STACK_SERVICE_H_

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

#include <aef/embedded/service/srvddk/uefsrvddk.h>
#include <fnet.h>

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
* FNET Network Stack service base function codes
*/
#define FNET_STACK_INITIALIZE					0x800
#define FNET_STACK_RELEASE						0x801
#define FNET_STACK_GET_MACADDR					0x802
#define FNET_STACK_GET_IP4ADDR					0x803
#define FNET_STACK_DNS_INIT						0x804
#define FNET_STACK_SOCKET						0x810
#define FNET_STACK_SOCKET_BIND					0x811
#define FNET_STACK_SOCKET_LISTEN				0x812
#define FNET_STACK_SOCKET_ACCEPT				0x813
#define FNET_STACK_SOCKET_CONNECT				0x814
#define FNET_STACK_SOCKET_RECV					0x815
#define FNET_STACK_SOCKET_RECV_FROM				0x816
#define FNET_STACK_SOCKET_SEND					0x817
#define FNET_STACK_SOCKET_SEND_TO				0x818
#define FNET_STACK_SOCKET_SHUTDOWN				0x819
#define FNET_STACK_SOCKET_CLOSE					0x81A
#define FNET_STACK_SOCKET_SETOPT				0x81B
#define FNET_STACK_SOCKET_GETOPT				0x81C
#define FNET_STACK_SOCKET_GET_NAME				0x81D
#define FNET_STACK_SOCKET_GET_PEER_NAME			0x81E

/*
* FNET Network Stack service I/O Control codes
*/
#define IOCTL_FNET_STACK_INITIALIZE				SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_GET_MACADDR			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_GET_MACADDR,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_GET_IP4ADDR			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_GET_IP4ADDR,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_DNS_INIT				SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_DNS_INIT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_RELEASE				SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_RELEASE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET					SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_BIND			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_BIND,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_LISTEN			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_LISTEN,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_ACCEPT			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_ACCEPT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_CONNECT			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_CONNECT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_RECV			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_RECV,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_RECV_FROM		SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_RECV_FROM,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_SEND			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_SEND,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_SEND_TO			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_SEND_TO,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_SHUTDOWN		SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_SHUTDOWN,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_CLOSE			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_CLOSE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_SETOPT			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_SETOPT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_GETOPT			SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_GETOPT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_GET_NAME		SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_GET_NAME,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_FNET_STACK_SOCKET_GET_PEER_NAME	SRVIOCTLCODE(SERVICE_TYPE_NETWORK,FNET_STACK_SOCKET_GET_PEER_NAME,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

typedef struct _netif_init_parms_def
{
    fnet_netif_desc_t   			netif_desc;
    fnet_mac_addr_t     			netif_mac_addr;
    fnet_ip4_addr_t     			netif_ip4_addr;
    fnet_ip4_addr_t     			netif_ip4_subnet_mask;
    fnet_ip4_addr_t     			netif_ip4_gateway;
    fnet_ip4_addr_t     			netif_ip4_dns;
} netif_init_params_t;

typedef struct socket_desc_def
{
	fnet_socket_t					socket;
} socket_desc_t;

typedef struct socket_parms_def
{
	fnet_address_family_t			family;
	fnet_socket_type_t				type;
	uint32_t						protocol;
} socket_parms_t;

typedef struct bind_parms_def
{
	fnet_socket_t					socket;
	const struct sockaddr*			name;
	uint32_t						namelen;
} bind_parms_t;

typedef struct listen_parms_def
{
	fnet_socket_t					socket;
	uint32_t						backlog;
} listen_parms_t;

typedef struct accept_parms_def
{
	fnet_socket_t					socket;
	struct sockaddr*				addr;
	uint32_t*						addrlen;
} accept_parms_t;

typedef struct connect_parms_def
{
	fnet_socket_t					socket;
	struct sockaddr*				name;
	uint32_t						namelen;
} connect_parms_t;

typedef struct recv_parms_def
{
	fnet_socket_t					socket;
	uint8_t*						buffer;
	fnet_size_t						length;
	fnet_flag_t						flags;
} recv_parms_t;

typedef struct recv_from_parms_def
{
	fnet_socket_t					socket;
	uint8_t*						buffer;
	fnet_size_t						length;
	fnet_flag_t						flags;
	struct sockaddr*				from_addr;
	fnet_size_t*					from_length;
} recv_from_parms_t;

typedef struct send_parms_def
{
	fnet_socket_t					socket;
	uint8_t*						buffer;
	fnet_size_t						length;
	fnet_flag_t						flags;
} send_parms_t;

typedef struct send_to_parms_def
{
	fnet_socket_t					socket;
	uint8_t*						buffer;
	fnet_size_t						length;
	fnet_flag_t						flags;
	const struct sockaddr*			to_addr;
	fnet_size_t 					to_length;
} send_to_parms_t;

typedef struct shutdown_parms_def
{
	fnet_socket_t					socket;
	fnet_sd_flags_t					how;
} shutdown_parms_t;

typedef struct close_parms_def
{
	fnet_socket_t					socket;
} close_parms_t;

typedef struct setopt_parms_def
{
	fnet_socket_t					socket;
	fnet_protocol_t					level;
	fnet_socket_options_t			optname;
	const void*						optval;
	fnet_size_t						optvallen;
} setopt_parms_t;

typedef struct getopt_parms_def
{
	fnet_socket_t					socket;
	fnet_protocol_t					level;
	fnet_socket_options_t			optname;
	void*							optval;
	fnet_size_t*					optvallen;
} getopt_parms_t;

typedef struct name_parms_def
{
	fnet_socket_t					socket;
	struct sockaddr*				name;
	uint32_t*						namelen;
} name_parms_t;

typedef struct macaddr_parms_def
{
	fnet_uint8_t					hw_addr[6];
} macaddr_parms_t;

typedef struct ip4addr_parms_def
{
	fnet_ip4_addr_t					ip4_addr;
} ip4addr_parms_t;

typedef struct dns_init_parms_def
{
	struct sockaddr					dns_server_addr;
	char* 							host_name;
	fnet_address_family_t			addr_family;
	fnet_dns_callback_resolved_t	callback;
	uint32_t						param;
} dns_init_parms_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_FNET_STACK_FNET_STACK_SERVICE_H_ */
