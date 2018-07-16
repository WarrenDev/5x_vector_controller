
/**
* CThread.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CThread class.
*
*/

#ifndef FRAMEWORK_CTHREAD_H_
#define FRAMEWORK_CTHREAD_H_

#include <stdint.h>
#include <string.h>
#include <aef/embedded/osal/thread.h>

/***********************************************************
 * CThread class definition
 */
class CThread
{

public:

	/*******************************************************
	 * CThread constructor
	 */
	CThread ( uint32_t StackSize = DEFAULT_STACK_SIZE, uint32_t Priority = DEFAULT_TASK_PRIORITY,
			  uint32_t Quantum = DEFAULT_QUANTUM )
	{
		thread_create ( &m_ctx, StackSize, Quantum, Priority, 0, entryPoint, (uint32_t)this );
	}

	/*******************************************************
	 * CThread destructor
	 */
	virtual ~CThread ()
	{
		thread_destroy ( &m_ctx );
	}

	/*******************************************************
	 * Thread management routines
	 */
	virtual	void	 Start		 ( void )			{ Resume(); 									}
	virtual void	 Resume		 ( void )			{ thread_resume ( &m_ctx );						}
	virtual void 	 Suspend 	 ( void )			{ thread_suspend ( &m_ctx );					}
	virtual void	 TaskLock	 ( void )			{ thread_lock ( &m_ctx );						}
	virtual	void	 TaskUnlock	 ( void )			{ thread_unlock ( &m_ctx );						}
	virtual uint32_t GetId		 ( void ) 			{ return thread_get_id ( &m_ctx ); 				}
	virtual uint32_t GetPriority ( void ) 			{ return thread_get_priority ( &m_ctx );		}
	virtual void	 SetPriority ( uint32_t uiPriority )
	{
		thread_set_priority ( &m_ctx, uiPriority );
	}

protected:

	/*******************************************************
	 * Abstract base class, you must derive
	 */
	virtual void threadRun ( void ) = 0;

private:

	static void entryPoint ( void* pArg )
	{
		CThread* pCThread = static_cast<CThread*>(pArg);
		pCThread->threadRun ();
		return;
	}

	thread_ctx_t	m_ctx;

} ;

#endif
