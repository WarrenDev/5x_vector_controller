
/**
* timer.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL timer implementation.
*
*/

#include <aef/embedded/osal/timer.h>

/**
* Create and initialize a timer
*
* \param    ctx				Pointer to a context to initialize
* \param	mode			Timer mode
* \param	data			Timer data
* \param	period			Timer period
* \param	callback		Timer callback function pointer
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the message queue
*/
system_status_t timer_create_timer (timer_ctx_t* ctx, timer_type_t type, void* data, uint32_t period, vFUNCPtr callback)
{
	if ( ctx != Co_NULL )
	{
		uint8_t timer_type = (type == SEF_TIMER_ONE_SHOT) ? TMR_TYPE_ONE_SHOT : TMR_TYPE_PERIODIC;
		ctx->timer_id = CoCreateTmr ( timer_type, period, period, callback );
		if ( ctx->timer_id != E_CREATE_FAIL )
		{
			ctx->timer_period = period;
			return SYSTEM_STATUS_SUCCESS;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Destroy a timer
*
* \param    ctx				Pointer to the timer context to destroy
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t timer_destroy_timer (timer_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoDelTmr ( ctx->timer_id );
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Start a timer
*
* \param    ctx				Pointer to the timer context to start
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t timer_start_timer (timer_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoStartTmr ( ctx->timer_id );
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Stop a timer
*
* \param    ctx				Pointer to the timer context to stop
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t timer_stop_timer (timer_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoStopTmr ( ctx->timer_id );
	}
	return SYSTEM_FAILURE_GENERAL;
}

