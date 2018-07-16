
/**
* semaphore.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Semaphore implementation.
*
*/

#include <aef/embedded/osal/semaphore.h>

/**
* Create and initialize a semaphore
*
* \param    ctx				Pointer to a context to initialize
* \param	name			Name to identify the semaphore
* \param	initial_count	Initial semaphore count
* \param	flags			Creation flags
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the semaphore object
*/
system_status_t semaphore_create (semaphore_ctx_t* ctx, char* name, uint32_t initial_count, uint32_t max_count, uint32_t flags)
{
	if ( ctx != Co_NULL && name != Co_NULL )
	{
		ctx->sem_handle = CoCreateSem ( initial_count, max_count, (uint8_t) flags );
		if ( ctx->sem_handle != -1 )
		{
			ctx->sem_name      = name;
			ctx->sem_count     = initial_count;
			ctx->sem_max_count = max_count;
			ctx->sem_flags     = flags;
			return SYSTEM_STATUS_SUCCESS;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Destroy a semaphore
*
* \param    ctx				Pointer to a semaphore context
* \param    name			Name of the semaphore to destroy
* \param	force_destroy	Destroy flag
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the semaphore object
*/
system_status_t semaphore_destroy (semaphore_ctx_t* ctx, char* name, bool force_destroy)
{
	if ( ctx != Co_NULL )
	{
		uint8_t option = (force_destroy) ? OPT_DEL_ANYWAY : OPT_DEL_NO_PEND;
		return (system_status_t) CoDelSem ( ctx->sem_handle, option );
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Wait until the semaphore is available
*
* \param    ctx		Pointer to a semaphore context
* \param	timeout	Timeout value in milliseconds
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
*/
system_status_t semaphore_wait (semaphore_ctx_t* ctx, uint32_t timeout)
{
	if ( ctx != Co_NULL )
	{
		return (system_status_t) CoPendSem ( ctx->sem_handle, timeout );
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Wait until the semaphore is available or timeout occurs
*
* \param    ctx		Pointer to a semaphore context
* \param	timeout	Time out value in ticks
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or time out.
*/
system_status_t semaphore_wait_ticks (semaphore_ctx_t* ctx, uint32_t ticks)
{
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Wait until the semaphore is available or timeout occurs
*
* \param    ctx			Pointer to a semaphore context
* \param	time_ctx	Pointer to a time context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t semaphore_wait_for(semaphore_ctx_t* ctx, void* time_ctx)
{
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Poll for the semaphore
*
* \param    ctx			Pointer to a semaphore context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t semaphore_poll(semaphore_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoAcceptSem ( ctx->sem_handle );
	}
	return SYSTEM_FAILURE_UNSUPPORTED_OPERATION;
}

/**
* Post the semaphore
*
* \param    ctx			Pointer to a semaphore context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t semaphore_post(semaphore_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoPostSem ( ctx->sem_handle );
	}
	return SYSTEM_FAILURE_GENERAL;
}

