
/**
* thread.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Thread implementation.
*
*/

#include <aef/embedded/osal/thread.h>
#include "bsp.h"

/**
* Create and initialize a thread
*
* \param	ctx				Pointer to the context to initialize
* \param    thread_name		Thread name
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
thread_create(thread_ctx_t* ctx, uint32_t stack_size, uint32_t quantum, uint32_t priority, uint32_t attributes, FUNCPtr address, uint32_t parameter)
{
	if ( ctx != Co_NULL && address != Co_NULL )
	{
		OS_STK* thread_stack = (OS_STK*) malloc ( stack_size * sizeof(OS_STK) );
		if ( thread_stack == Co_NULL )
			return SYSTEM_FAILURE_GENERAL;

		ctx->_task_id = CoCreateTaskEx ( address, (void*)parameter, priority, &thread_stack[stack_size-1], stack_size, quantum, Co_TRUE );
		if ( ctx->_task_id != E_CREATE_FAIL )
		{
			ctx->_stack_ptr         = thread_stack;
			ctx->_stack_size        = stack_size;
			ctx->_attributes        = attributes;
			ctx->_previous_priority = priority;
			ctx->_current_priority  = priority;
			ctx->_previous_quantum  = quantum;
			return SYSTEM_STATUS_SUCCESS;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Destroy the thread
*
* \param    ctx			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_destroy(thread_ctx_t* ctx)
{
	if ( ctx != Co_NULL && ctx->_task_id != E_CREATE_FAIL )
	{
		CoDelTask ( ctx->_task_id );
		CoKfree   ( ctx->_stack_ptr );
		return SYSTEM_STATUS_SUCCESS;
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Start the thread
*
* \param    ctx			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_start(thread_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		thread_resume ( ctx );
		return SYSTEM_STATUS_SUCCESS;
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Resume the thread that has been suspended.
*
* \param    ctx			Pointer to the thread context
*
* \returns  Nune
*/
void
thread_resume(thread_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		CoAwakeTask ( ctx->_task_id );
	}
}

/**
* Suspend the thread
*
* \param    ctx			Pointer to the thread context
*
* \returns  None
*/
void
thread_suspend(thread_ctx_t* ctx)
{
	CoSuspendTask ( ctx->_task_id );
}

/**
* Set the running priority of the thread.
*
* \param    ctx				Pointer to the thread context
* \param	priority		New thread priority
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_set_priority(thread_ctx_t* ctx, uint32_t priority)
{
	if ( ctx != Co_NULL )
	{
		if ( CoSetPriority ( ctx->_task_id, priority ) == E_OK )
		{
			ctx->_previous_priority = ctx->_current_priority;
			ctx->_current_priority = priority;
			return SYSTEM_STATUS_SUCCESS;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Set the quantum (time slice) of the thread
*
* \param    ctx				Pointer to the thread context
* \param	quantum			New thread quantum
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_set_quantum(thread_ctx_t* ctx, uint32_t quantum)
{
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Lock the thread so that it will not be preempted.
* This will raise the thread priority to real-time.
*
* \param    ctx			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_lock(thread_ctx_t* ctx)
{
	return thread_set_priority(ctx, TASK_PRIORITY_TIME_CRITICAL);
}

/**
* Unlock the thread so that it can be preempted.
* This will restore the thread to its previous priority before the
* lock call.
*
* \param    ctx			Pointer to the thread context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if unsuccessful.
*/
system_status_t
thread_unlock(thread_ctx_t* ctx)
{
	return thread_set_priority(ctx, ctx->_previous_priority);
}

/**
* Retrieve the id of the thread.
*
* \param    none
*
* \returns  The thread id.
*/
uint32_t
thread_get_id(thread_ctx_t* ctx)
{
	return (uint32_t)ctx->_task_id;
}

/**
* Retrieve the current priority of the thread.
*
* \param    ctx			Pointer to the thread context
*
* \returns  Current thread priority
*/
uint32_t
thread_get_priority(thread_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return ctx->_current_priority;
	}
	return -1;
}

/**
* Put the active task at the end of the ready queue
*
* \param    None
*
* \returns  Current thread priority
*/
void
thread_sched_yield(void)
{
}

/**
* Enable preemption of the  current task
*
* \param    None
*
* \returns  None
*/
void
thread_start_preemption(void)
{
	CoSchedUnlock();
}

/**
* Disable preemption of the  current task
*
* \param    None
*
* \returns  None
*/
void
thread_stop_preemption(void)
{
	CoSchedLock();
}

