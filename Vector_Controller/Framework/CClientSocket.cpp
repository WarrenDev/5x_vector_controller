
/**
* CClientSocketIo.h
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

#include "CClientSocket.h"

/**
* CClientSocket class constructor
*/
CClientSocket::CClientSocket ( fnet_address_family_t addrFamily, fnet_socket_type_t type, uint32_t protocol )
	: CSocketIo (addrFamily, type, protocol)
{

	memset ( &m_addr, 0, sizeof(struct sockaddr_in) );
	m_addr.sin_family      = AF_INET;
	m_addr.sin_port        = 0;
	m_addr.sin_addr.s_addr = INADDR_ANY;

	/* Set socket options - Turn on keep-alive, disable Nagle */

}

/**
* CClientSocket class destructor
*/
CClientSocket::~CClientSocket()
{
	TerminateConnection();
}

/**
* CClientSocket::CreateConnection
*
* Establish a connection on this socket
*
* \param none
*
* \return 1 if successful
*         0 on error
*/
int	CClientSocket::CreateConnection	( void )
{
	return 0;
}

/**
* CClientSocket::TerminateConnection
*
* Terminate a connection on this socket
*
* \param none
*
* \return 1 if successful
*         0 on error
*/
int	CClientSocket::TerminateConnection ( void )
{
	return EndSession();
}
