
/**
* critical_section.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Critical section implementation.
*
*/

#include <aef/embedded/osal/critical_section.h>

/**
* Create and initialize a critical section
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the critical section object
* 			SYSTEM_FAILUER_INVALID_PARAMETER on a null context pointer
*/
system_status_t critical_section_create (critical_section_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		ctx->mutex = CoCreateMutex();
		return ( ctx->mutex != -1 ) ? SYSTEM_STATUS_SUCCESS : SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Destroy a critical section
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the critical section object
* 			SYSTEM_FAILUER_INVALID_PARAMETER on a null context pointer
* 			MQX error values
*/
system_status_t critical_section_destroy (critical_section_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Enter the critical section.  This is blocking call.
*
* \param    ctx		Pointer to a critical section context.  This context
* 					must have been created by a call to critical_section_create().
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the critical section object
* 			SYSTEM_FAILUER_INVALID_PARAMETER on a null context pointer
* 			CoOS error values
*/
system_status_t critical_section_acquire (critical_section_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoEnterMutexSection ( ctx->mutex );
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Try to enter the critical section
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the critical section object
* 			SYSTEM_FAILUER_INVALID_PARAMETER on a null context pointer
* 			CoOS error values
*/
system_status_t critical_section_try_acquire (critical_section_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoEnterMutexSection ( ctx->mutex );
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Exit the critical section.
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the critical section object
* 			SYSTEM_FAILUER_INVALID_PARAMETER on a null context pointer
* 			CoOS error values
*/
system_status_t critical_section_release (critical_section_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoLeaveMutexSection ( ctx->mutex );
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

