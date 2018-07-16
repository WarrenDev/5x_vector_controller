
/**
* CVIDLComm.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CVIDLComm class.
*
*/

#include "CVIDLComm.h"

#define VIDL_OUTPUT_PORT		6002
#define VIDL_ID					"WARDEV001"

CVIDLComm* CVIDLComm::instance = nullptr;

/**
* Constructor
*/
CVIDLComm::CVIDLComm(fnet_address_family_t addrFamily, fnet_socket_type_t type, uint32_t protocol)
	: CClientSocket ( addrFamily, type, protocol )
	, m_time(0L)
	, m_bufferLength(0)
{
	memset ( &m_addr, 0, sizeof(struct sockaddr) );
	fnet_inet_ptos ( "255.255.255.255", &m_addr );
	m_addr.sa_family = addrFamily;
	m_addr.sa_port   = FNET_HTONS(VIDL_OUTPUT_PORT);
}

/**
* Destructor
*/
CVIDLComm::~CVIDLComm()
{
}

/**
* sendVIDL - Routine to send a VIDL target message to the central controller
*
* /param message			Message to send
* /param message_length		Size of message
*
* /return nune
*/
void CVIDLComm::sendVIDL ( const char* message, uint16_t message_length )
{
	m_CLock.acquire();

	m_bufferLength = sprintf ( m_buffer, "%s%s", VIDL_ID, message );
	if ( m_bufferLength )
	{
		SendPacketTo ( (uint8_t*)m_buffer, m_bufferLength, (struct sockaddr*)&m_addr, sizeof(struct sockaddr));
	}
	m_CLock.release();
}

/**
* getInstance - Return an instance of CVIDLComm
*/
CVIDLComm* CVIDLComm::getInstance(void)
{
	if ( instance == nullptr )
	{
		instance = new CVIDLComm;
	}
	return instance;
}

