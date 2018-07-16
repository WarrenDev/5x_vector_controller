
/**
* CServerSocketIo.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CClientSocket class.
* 		  Uses the FNET network stack service.
*
*/

#include "CServerSocket.h"

/***********************************************************
* CServerSocket class constructors
*/
CServerSocket::CServerSocket ( const char* server, uint16_t port, fnet_address_family_t addrFamily, fnet_socket_type_t type, uint32_t protocol )
	: CSocketIo (addrFamily, type, protocol)
	, m_serverAddrLength(sizeof(struct sockaddr))
	, m_clientAddrLength(sizeof(struct sockaddr))
	, m_ip4Addr(0)
{
	memset ( &m_serverAddr, 0, m_serverAddrLength );

	if ( server == NULL )
	{
		/**
		 * Get our IP4 address from the network interface descriptor
		 */
		char ip4AddrStr[20];
		GetIP4Addr ( &m_ip4Addr );
		fnet_inet_ntop ( addrFamily, &m_ip4Addr, ip4AddrStr, sizeof(ip4AddrStr));
		fnet_inet_ptos ( ip4AddrStr, &m_serverAddr );
	}
	else
	{
		fnet_inet_ptos ( server, &m_serverAddr );
	}
	m_serverAddr.sa_port = FNET_HTONS(port);

	BindAddr ( (struct sockaddr*)&m_serverAddr, m_serverAddrLength );

	/**
	 * Set socket options - Turn on keep-alive, disable Nagle
	 */

}

/***********************************************************
* CServerSocket class destructor
*/
CServerSocket::~CServerSocket()
{
}

/***********************************************************
* CServerSocket::ListenForClient
*
* Listen for client connection requests
*
* \param	backlog
*
* \return 1 if successful
*         0 on error
*/
int	CServerSocket::ListenForClient ( uint16_t backlog )
{
	return ListenTo ( backlog );
}

/***********************************************************
* CServerSocket::AcceptConnection
*
* Accept the client connection requests
*
* \param	none
*
* \return CSocetIo class pointer of connection socket if successful
*         nullptr on error
*/
CSocketIo* CServerSocket::AcceptConnection ( void )
{
	return AcceptRequest ( &m_clientAddr, &m_clientAddrLength );
}

/***********************************************************
* CServerSocket::TerminateListen
*
* Terminate the listen socket
*
* \param	none
*
* \return 1 if successful
*         0 on error
*/
int CServerSocket::TerminateListen ( void )
{
	return SOCKETIO_NOERROR;
}

/***********************************************************
* CServerSocket::TerminateConnection
*
* Terminate a connection on this socket
*
* \param	Socket		Socket identifier of the connection
*
* \return 1 if successful
*         0 on error
*/
int CServerSocket::TerminateConnection ( CSocketIo* Socket )
{
	return SOCKETIO_NOERROR;
}
