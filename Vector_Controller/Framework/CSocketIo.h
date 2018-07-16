
/**
* CSocketIo.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CSocketIo class.
* 		  Uses the FNET network stack service.
*
*/

#ifndef FRAMEWORK_CSOCKETIO_H_
#define FRAMEWORK_CSOCKETIO_H_

#include <stdint.h>
#include "aef/embedded/service/service_manager.h"
#include "aef/embedded/service/fnet_stack/fnet_stack_service.h"
#include "fnet.h"

#ifndef INADDR_ANY
# define INADDR_ANY					((unsigned long int) 0x00000000)
#endif /* INADDR_ANY */

#ifndef INADDR_NONE
# define INADDR_NONE				0xffffffff
#endif /* INADDR_NONE */

#define	SOCKETIO_ERROR				FNET_ERR
#define	SOCKETIO_NOERROR			FNET_OK

enum
{
	SOCKERROR_INITFAILED		= (-2),
	SOCKERROR_INVALIDSOCKET		= (-3),
	SOCKERROR_BADHOSTNAME		= (-4),
	SOCKERROR_BINDFAILED		= (-5),
	SOCKERROR_NOTCONNECTED		= (-6),
	SOCKERROR_SSLCONTEXT		= (-7),
	SOCKERROR_SSLNEW			= (-8),
	SOCKERROR_SSLLOADCERT		= (-9)
};

enum
{
	SOCKSTATUS_INITIALIZED		= 0x0001,
	SOCKSTATUS_CONNECTED		= 0x0002,
	SOCKSTATUS_LISTENING		= 0x0004
};

enum
{
	NETSTATUS_UNINITIALIZED		= 0x0000,
	NETSTATUS_INITIALIZED		= 0x0001,
	NETSTATUS_BINDED			= 0x0002
};

enum
{
	NETBIT_NOERRORS				= 0x00000000,
	NETBIT_INIT_FAILURE			= 0x00000001,
	NETBIT_DHCP_FAILURE			= 0x00000002,
	NETBIT_IPADDR_FAILURE		= 0x00000004
};

/**
* CSocketIo class definition
*/
class CSocketIo
{
public:

	/**
	* CSocketIo class construction / destruction
	*/
	CSocketIo (fnet_address_family_t ProtocolFamily = AF_INET, fnet_socket_type_t Type = SOCK_STREAM, uint32_t Protocol = IPPROTO_TCP);
	CSocketIo (fnet_socket_t socket, fnet_address_family_t ProtocolFamily = AF_INET, fnet_socket_type_t Type = SOCK_STREAM, uint32_t Protocol = IPPROTO_TCP);
	virtual ~CSocketIo();

	/**
	* Socket management
	*/
	int CreateSocket ( void );
	int CloseSocket  ( void );
	int SetOptions   ( fnet_protocol_t level, fnet_socket_options_t name, void* value, fnet_size_t size );
	int GetOptions   ( fnet_protocol_t level, fnet_socket_options_t name, void* value, fnet_size_t* size );

	/**
	* Server API methods
	*/
	int BindAddr ( struct sockaddr* addr, uint16_t size );
	int ListenTo ( uint16_t backlog );
	CSocketIo* AcceptRequest ( struct sockaddr* addr, uint32_t* size );

	/**
	* Client API methods
	*/
	int ConnectTo (struct sockaddr* addr, uint32_t size );

	/**
	* Data transmission methods
	*/
	int SendPacket     ( uint8_t* buffer, fnet_size_t size, fnet_flag_t flags );
	int RecvPacket     ( uint8_t* buffer, fnet_size_t size, fnet_flag_t flags );
	int	SendPacketTo   ( uint8_t* buffer, fnet_size_t size, sockaddr* addr, uint32_t addrsize );
	int	RecvPacketFrom ( uint8_t* buffer, fnet_size_t size, sockaddr* addr, uint32_t* addrsize );

	/**
	* Termination methods
	*/
	int EndSession ( fnet_sd_flags_t flag = SD_READ );

	/**
	* Setter / Getter methods
	*/
	fnet_socket_t GetSocket ( void );

	int GetMACAddr 	( uint8_t* macAddr );
	int GetIP4Addr 	( uint32_t* ip4Addr );

private:

	/**
	* Socket configuration
	*/
	fnet_socket_t 		  m_socket;
	fnet_address_family_t m_ProtocolFamily;
	fnet_socket_type_t 	  m_Type;
	uint32_t 			  m_Protocol;

	/**
	* FNET network service
	*/
	service_vtable_t* 	  m_netsrv;

} ;

#endif /* FRAMEWORK_CSOCKETIO_H_ */
