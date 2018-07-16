
/**
* CSMSComm.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CSMSComm class.
*
*/

#ifndef APPLICATION_CSMSCOMM_H_
#define APPLICATION_CSMSCOMM_H_

#include <stdint.h>
#include "derivative.h" /* include peripheral declarations */
#include "stdio.h"
#include "CClientSocket.h"
#include "CCriticalSection.h"
#include "aef/embedded/osal/time.h"
#include "bsp.h"
#include "frdmk64f.h"


/**
 * CSMSComm - SMS communications class
 *
 */
class CSMSComm : public CClientSocket
{
public:

	virtual ~CSMSComm();

	/**
	 * getInstance - Return an instance of CSMSCom
	 */
	static CSMSComm* getInstance(void);

	/**
	* sendSMS - Routine to send a SMS message to the central display
	*
	* /param destination		Message destination
	* /param message			Message to send
	* /param message_length		Size of message
	*
	* /return nune
	*/
	void sendSMS ( const char* destination, const char* message, uint16_t message_length );

private:

	/**
	* Constructor
	*/
	CSMSComm(fnet_address_family_t addrFamily = AF_INET, fnet_socket_type_t type = SOCK_DGRAM, uint32_t protocol = 0);

	struct sockaddr 	m_addr;
	CCriticalSection 	m_CLock;
	uint64_t			m_time;
	uint16_t 			m_bufferLength;
	char 				m_buffer[512];
	static CSMSComm* 	instance;

};

#endif /* APPLICATION_CSMSCOMM_H_ */
