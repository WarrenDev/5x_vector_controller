
/**
* CServerSocketIo.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CServerSocket class.
* 		  Uses the FNET network stack service.
*
*/

#ifndef FRAMEWORK_CSERVERSOCKET_H_
#define FRAMEWORK_CSERVERSOCKET_H_

#include "CSocketIo.h"

/**
* CServerSocket class definition
*/
class CServerSocket : public CSocketIo
{

public:

	/**
	* CServerSocket class construction / destruction
	*/
	CServerSocket ( const char* server, uint16_t port, fnet_address_family_t addrFamily = AF_INET, fnet_socket_type_t type = SOCK_STREAM, uint32_t protocol = IPPROTO_IP );
	virtual ~CServerSocket ( );

	/**
	* Connection management
	*/
	int			ListenForClient		( uint16_t backlog );
	CSocketIo*	AcceptConnection	( void );
	int 		TerminateListen 	( void );
	int			TerminateConnection ( CSocketIo* Socket );

	uint32_t	GetIp4Addr			( void )	{ return m_ip4Addr;		}

private:

    struct sockaddr	m_serverAddr;
    struct sockaddr	m_clientAddr;
    fnet_size_t		m_serverAddrLength;
    fnet_size_t		m_clientAddrLength;
    uint32_t		m_ip4Addr;
};

#endif /* FRAMEWORK_CSERVERSOCKET_H_ */
