
/**
* semaphore.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL semaphore definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_SEMAPHORE_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_SEMAPHORE_H_

#include <aef/embedded/system/system_status.h>
#include <stdint.h>
#include <stdbool.h>
#include <CoOS.h>
#include <OsMutex.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus


/**
* The semaphore context.
*/
typedef struct semaphore_ctx_def
{
	OS_EventID	sem_handle;
	uint32_t	sem_count;
	uint32_t	sem_max_count;
	uint32_t	sem_flags;
	char*		sem_name;
} semaphore_ctx_t;

/**
* Create and initialize a semaphore
*
* \param    ctx				Pointer to a context to initialize
* \param	name			Name to identify the semaphore
* \param	initial_count	Initial semaphore count
* \param	max_count		maximum semaphore count
* \param	flags			Creation flags
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the semaphore object
*/
system_status_t semaphore_create (semaphore_ctx_t* ctx, char* name, uint32_t initial_count, uint32_t max_count, uint32_t flags);

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
system_status_t semaphore_destroy (semaphore_ctx_t* ctx, char* name, bool force_destroy);

/**
* Wait until the semaphore is available
*
* \param    ctx		Pointer to a semaphore context
* \param	timeout	Timeout value in milliseconds
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t semaphore_wait (semaphore_ctx_t* ctx, uint32_t timeout);

/**
* Wait until the semaphore is available or timeout occurs
*
* \param    ctx		Pointer to a semaphore context
* \param	timeout	Time out value in ticks
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t semaphore_wait_ticks (semaphore_ctx_t* ctx, uint32_t ticks);

/**
* Wait until the semaphore is available or timeout occurs
*
* \param    ctx			Pointer to a semaphore context
* \param	time_ctx	Pointer to a time context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t semaphore_wait_for(semaphore_ctx_t* ctx, void* time_ctx);

/**
* Poll for the semaphore
*
* \param    ctx			Pointer to a semaphore context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t semaphore_poll(semaphore_ctx_t* ctx);

/**
* Post the semaphore
*
* \param    ctx			Pointer to a semaphore context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t semaphore_post(semaphore_ctx_t* ctx);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_SEMAPHORE_H_ */
