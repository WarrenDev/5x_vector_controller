/**
* CConsole.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CConsole class.
*
*/

#include "CConsole.h"

#define CONSOLE_OUTPUT_PORT		13000

CConsole* CConsole::instance = nullptr;

/**
* Constructor
*/
CConsole::CConsole(fnet_address_family_t addrFamily, fnet_socket_type_t type, uint32_t protocol)
	: CClientSocket ( addrFamily, type, protocol )
	, m_time(0L)
	, m_bufferLength(0)
{
	memset ( &m_addr, 0, sizeof(struct sockaddr) );
	fnet_inet_ptos ( "255.255.255.255", &m_addr );
	m_addr.sa_family = addrFamily;
	m_addr.sa_port   = FNET_HTONS(CONSOLE_OUTPUT_PORT);
}

/**
* Destructor
*/
CConsole::~CConsole()
{
}

/**
* getInstance - Return an instance of CConsole
*/
CConsole* CConsole::getInstance(void)
{
	if ( instance == nullptr )
	{
		instance = new CConsole;
	}
	return instance;
}

/**
* write - Routine to write information to the console
*
* /param message			Message to send to the console
* /param message_length	Size of message
*
* /return nune
*/
void CConsole::write ( const char* message, uint16_t message_length )
{
	write ( CON_INFORMATION, message, message_length );
}

/**
* write - Routine to write a specific message type to the console
*
* /param type				Message type
* /param message			Message to send to the console
* /param message_length	Size of message
*
* /return nune
*/
void CConsole::write ( consoleMessageType_t type, const char* message, uint16_t message_length )
{
	m_CLock.acquire();
	m_bufferLength = 0L;
	m_time = time_get_ticks(nullptr);
	switch (type)
	{
		case CON_INFORMATION:
			m_bufferLength = sprintf ( m_buffer, "%.012ld INFORMATION: %s", (uint32_t)m_time, message );
			break;
		case CON_TRACE:
			m_bufferLength = sprintf ( m_buffer, "%.012ld TRACE: %s", (uint32_t)m_time, message );
			break;
		case CON_DEBUG:
			m_bufferLength = sprintf ( m_buffer, "%.012ld DEBUG: %s", (uint32_t)m_time, message );
			break;
		case CON_WARNING:
			m_bufferLength = sprintf ( m_buffer, "%.012ld WARNING: %s", (uint32_t)m_time, message );
			break;
		case CON_ERROR:
			m_bufferLength = sprintf ( m_buffer, "%.012ld ERROR: %s", (uint32_t)m_time, message );
			break;
	}
	if ( m_bufferLength )
	{
		SendPacketTo ( (uint8_t*)m_buffer, m_bufferLength, (struct sockaddr*)&m_addr, sizeof(struct sockaddr));
	}
	m_CLock.release();
}
