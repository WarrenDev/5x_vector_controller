/**
* CConsole.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CConsole class.
*
*/

#ifndef FRAMEWORK_CCONSOLE_H_
#define FRAMEWORK_CCONSOLE_H_

#include <stdint.h>
#include "derivative.h" /* include peripheral declarations */
#include "stdio.h"
#include "CClientSocket.h"
#include "CCriticalSection.h"
#include "aef/embedded/osal/time.h"
#include "bsp.h"
#include "frdmk64f.h"

typedef enum
{
	CON_INFORMATION	= 0,		// Information message
	CON_TRACE		= 1,		// Trace message
	CON_DEBUG		= 2,		// Debug message
	CON_WARNING		= 3,		// Warning message
	CON_ERROR		= 4,		// Error message
} consoleMessageType_t;

/**
 * CConsole - System console class
 *
 */
class CConsole : public CClientSocket
{
public:

	virtual ~CConsole();

	/**
	 * getInstance - Return an instance of CConsole
	 */
	static CConsole* getInstance(void);

	/**
	 * write - Routine to write information to the console
	 *
	 * /param message			Message to send to the console
	 * /param message_length	Size of message
	 *
	 * /return nune
	 */
	void write ( const char* message, uint16_t message_length );

	/**
	 * write - Routine to write a specific message type to the console
	 *
	 * /param type				Message type
	 * /param message			Message to send to the console
	 * /param message_length	Size of message
	 *
	 * /return nune
	 */
	void write ( consoleMessageType_t type, const char* message, uint16_t message_length = 0 );

private:

	/**
	* Constructor
	*/
	CConsole(fnet_address_family_t addrFamily = AF_INET, fnet_socket_type_t type = SOCK_DGRAM, uint32_t protocol = 0);

	struct sockaddr 	m_addr;
	CCriticalSection 	m_CLock;
	uint64_t			m_time;
	uint16_t 			m_bufferLength;
	char 				m_buffer[512];
	static CConsole* 	instance;

};

extern CConsole* systemConsole;

#endif /* FRAMEWORK_CCONSOLE_H_ */
