
/**
* thread.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL thread definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_THREAD_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_THREAD_H_

#include <aef/embedded/system/system_status.h>
#include <stdint.h>
#include <CoOS.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

#define	DEFAULT_STACK_SIZE			512
#define	DEFAULT_TASK_PRIORITY		32
#define DEFAULT_QUANTUM				10

/**
* Application level task priority definitions
*/
#define TASK_PRIORITY_NORMAL		DEFAULT_TASK_PRIORITY
#define TASK_PRIORITY_ABOVE_NORMAL	(TASK_PRIORITY_NORMAL-1)
#define TASK_PRIORITY_HIGH_PRIORITY	(TASK_PRIORITY_NORMAL-2)
#define TASK_PRIORITY_TIME_CRITICAL	(TASK_PRIORITY_NORMAL-3)
#define TASK_PRIORITY_REAL_TIME		(TASK_PRIORITY_NORMAL-4)
#define TASK_PRIORITY_BELOW_NORMAL	(TASK_PRIORITY_NORMAL+1)
#define TASK_PRIORITY_LOW_PRIORITY	(TASK_PRIORITY_NORMAL+2)
#define	TASK_PRIORITY_BACKGROUND	(TASK_PRIORITY_NORMAL+3)
#define	TASK_PRIORITY_IDLE			(TASK_PRIORITY_NORMAL+4)

#define	THREAD_RUN_LOOP				while (true)

/**
* The thread context.
*/
typedef struct thread_ctx_def
{
	OS_TID	_task_id;
	OS_STK*	_stack_ptr;
	uint32_t _stack_size;
	uint32_t _attributes;
	uint32_t _previous_priority;
	uint32_t _current_priority;
	uint32_t _previous_quantum;
} thread_ctx_t;

/**
* Create and initialize a thread
*
* \param	ctx				Pointer to the context to initialize
* \param	stack_size		Thread stack size in words
* \param	quantum			Thread time slice in ms
* \param	priority		Thread priority
* \param	attributes		Thread attributes
* \param	address			Thread function pointer
* \param	parameters		Thread function parameter
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_create(thread_ctx_t* ctx, uint32_t stack_size, uint32_t quantum, uint32_t priority, uint32_t attributes, FUNCPtr address, uint32_t parameter);

/**
* Destroy the thread
*
* \param    thrd			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_destroy(thread_ctx_t* ctx);

/**
* Start the thread
*
* \param    thrd			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_start(thread_ctx_t* ctx);

/**
* Resume the thread that has been suspended.
*
* \param    thrd			Pointer to the thread context
*
* \returns  None
*/
void
thread_resume(thread_ctx_t* ctx);

/**
* Suspend the thread
*
* \param    None
*
* \returns  None
*/
void
thread_suspend(thread_ctx_t* ctx);

/**
* Set the running priority of the thread.
*
* \param    thrd			Pointer to the thread context
* \param	priority		New thread priority
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_set_priority(thread_ctx_t* thrd, uint32_t priority);

/**
* Set the quantum (time slice) of the thread
*
* \param    thrd			Pointer to the thread context
* \param	quantum			New thread quantum
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_set_quantum(thread_ctx_t* thrd, uint32_t quantum);

/**
* Lock the thread so that it will not be preempted.
* This will raise the thread priority to real-time.
*
* \param    thrd			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_lock(thread_ctx_t* thrd);

/**
* Unlock the thread so that it can be preempted.
* This will restore the thread to its previous priority before the
* lock call.
*
* \param    thrd			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_unlock(thread_ctx_t* thrd);

/**
* Retrieve the id of the thread.
*
* \param    None
*
* \returns  The thread id.
*/
uint32_t
thread_get_id(thread_ctx_t* ctx);

/**
* Retrieve the current priority of the thread.
*
* \param    thrd			Pointer to the thread context
*
* \returns  Current thread priority
*/
uint32_t
thread_get_priority(thread_ctx_t* thrd);

/**
* Put the active task at the end of the ready queue
*
* \param    None
*
* \returns  Current thread priority
*/
void
thread_sched_yield(void);

/**
* Enable preemption of the  current task
*
* \param    None
*
* \returns  None
*/
void
thread_start_preemption(void);

/**
* Disable preemption of the  current task
*
* \param    None
*
* \returns  None
*/
void
thread_stop_preemption(void);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_THREAD_H_ */
