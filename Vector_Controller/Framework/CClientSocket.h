
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

#ifndef FRAMEWORK_CCLIENTSOCKET_H_
#define FRAMEWORK_CCLIENTSOCKET_H_

#include "CSocketIo.h"

/**
* CClientSocket class definition
*/
class CClientSocket : public CSocketIo
{

public:

	/**
	* CClientSocket class construction / destruction
	*/
	CClientSocket ( fnet_address_family_t addrFamily = AF_INET, fnet_socket_type_t type = SOCK_STREAM, uint32_t protocol = IPPROTO_TCP );
	virtual ~CClientSocket ( );

	/**
	* Connection management
	*/
	int	CreateConnection	( void );
	int	TerminateConnection ( void );

private:

	struct sockaddr_in m_addr;
};

#endif /* FRAMEWORK_CCLIENTSOCKET_H_ */
