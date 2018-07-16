
/**
* CEventObject.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CEventObject class
*
*/

#ifndef CEVENTOBJECT_H_
#define CEVENTOBJECT_H_

#include "aef/embedded/osal/event.h"
#include <string.h>

/***********************************************************
 * CEventObject class definition
 */
class CEventObject {

public:

	/*******************************************************
	 * CEventObject constructor
	 */
	CEventObject ( char *pName = (char *)"DefaultName", BOOL ManualReset = false )
		: m_uiError(0)
		, m_bManualReset(ManualReset)
	{
		strcpy ( m_szEventName, pName );
		m_uiError = event_create ( &m_ctx, m_szEventName, ManualReset );
	}

	/*******************************************************
	 * CEventObject destructor
	 */
	virtual ~CEventObject ( )
	{ 
		event_destroy ( &m_ctx );
	}

	/*******************************************************
	 * Get the error state of the event object
	 */
	uint32_t GetState ( void )
	{
		return m_uiError;
	}

	/*******************************************************
	 * Wait for all specified event bits to be set
	 */
	uint32_t WaitAll ( uint32_t mask, uint32_t timeOut, uint32_t* value )
	{
		return event_wait_all ( &m_ctx, mask, timeOut, value );
	}
	
	/*******************************************************
	 * Wait for any of the specified event bits to be set
	 */
	uint32_t WaitAny ( uint32_t mask, uint32_t timeOut, uint32_t* value )
	{
		return event_wait_any ( &m_ctx, mask, timeOut, value );
	}
	
	/*******************************************************
	 * Clear an event bit
	 */
	uint8_t Reset ( uint32_t mask )
	{ 
		return event_reset ( &m_ctx, mask );
	}
	
	/*******************************************************
	 * Set an event bit
	 */
	uint8_t Signal ( uint32_t mask )
	{
		return event_signal ( &m_ctx, mask );
	}

private:

	event_ctx_t		m_ctx;
	uint32_t		m_uiError;					// Error status
	BOOL			m_bManualReset;				// Manual reset
	char			m_szEventName[32];			// Event object name

} ;

#endif /* CEVENTOBJECT_H_ */
