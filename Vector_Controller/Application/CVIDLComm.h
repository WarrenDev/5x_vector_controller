
/**
* CVIDLComm.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CVIDLComm class.
*
*/

#ifndef APPLICATION_CVIDLCOMM_H_
#define APPLICATION_CVIDLCOMM_H_

#include <stdint.h>
#include "derivative.h" /* include peripheral declarations */
#include "stdio.h"
#include "CClientSocket.h"
#include "CCriticalSection.h"
#include "aef/embedded/osal/time.h"
#include "bsp.h"
#include "frdmk64f.h"


/**
 * CVIDLComm - VIDL communications class
 *
 */
class CVIDLComm : public CClientSocket
{
public:

	virtual ~CVIDLComm();

	/**
	 * getInstance - Return an instance of CVIDLCom
	 */
	static CVIDLComm* getInstance(void);

	/**
	* sendVIDL - Routine to send a VIDL target message to the central controller
	*
	* /param message			Message to send
	* /param message_length		Size of message
	*
	* /return nune
	*/
	void sendVIDL ( const char* message, uint16_t message_length );

private:

	/**
	* Constructor
	*/
	CVIDLComm(fnet_address_family_t addrFamily = AF_INET, fnet_socket_type_t type = SOCK_DGRAM, uint32_t protocol = 0);

	struct sockaddr 	m_addr;
	CCriticalSection 	m_CLock;
	uint64_t			m_time;
	uint16_t 			m_bufferLength;
	char 				m_buffer[512];
	static CVIDLComm* 	instance;

};

#endif /* APPLICATION_CVIDLCOMM_H_ */
