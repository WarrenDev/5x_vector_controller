
/**
* CSMSComm.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CSMSComm class.
*
*/

#include "CSMSComm.h"

#define SMS_CMD					"AT+CMGS="
#define SMS_REQUEST_ADDR 		"192.168.3.21"
#define SMS_REQUEST_PORT 		6005
#define SMS_RESPONSE_ADDR 		"192.168.3.20"
#define SMS_RESPONSE_PORT		6004

CSMSComm* CSMSComm::instance = nullptr;

/**
* Constructor
*/
CSMSComm::CSMSComm(fnet_address_family_t addrFamily, fnet_socket_type_t type, uint32_t protocol)
	: CClientSocket ( addrFamily, type, protocol )
	, m_time(0L)
	, m_bufferLength(0)
{
	memset ( &m_addr, 0, sizeof(struct sockaddr) );
	fnet_inet_ptos ( "255.255.255.255", &m_addr );
	m_addr.sa_family = addrFamily;
	m_addr.sa_port   = FNET_HTONS(SMS_REQUEST_PORT);
}

/**
* Destructor
*/
CSMSComm::~CSMSComm()
{
}

/**
* sendSMS - Routine to send a SMS target message to the central controller
*
* /param destination		Message destination
* /param message			Message to send
* /param message_length		Size of message
*
* /return nune
*/
void CSMSComm::sendSMS ( const char* destination, const char* message, uint16_t message_length )
{
	m_CLock.acquire();

	m_bufferLength = sprintf ( m_buffer, "AT+CMGS=\"%s\",%s", destination, message );
	if ( m_bufferLength )
	{
		SendPacketTo ( (uint8_t*)m_buffer, m_bufferLength, (struct sockaddr*)&m_addr, sizeof(struct sockaddr));
	}
	m_CLock.release();
}

/**
* getInstance - Return an instance of CSMSComm
*/
CSMSComm* CSMSComm::getInstance(void)
{
	if ( instance == nullptr )
	{
		instance = new CSMSComm;
	}
	return instance;
}

