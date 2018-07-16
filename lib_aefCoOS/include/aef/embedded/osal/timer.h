
/**
* timer.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL timer definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_TIMER_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_TIMER_H_

#include <aef/embedded/system/system_status.h>
#include <stdint.h>
#include <CoOS.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

typedef enum timer_type
{
	SEF_TIMER_ONE_SHOT = 1,
	SEF_TIMER_PERIODIC
} timer_type_t;

/**
* The timer context.
*/
typedef struct timer_ctx_def
{
	OS_TCID		timer_id;
	uint32_t	timer_period;
} timer_ctx_t;

/**
* Create and initialize a timer
*
* \param    ctx				Pointer to a context to initialize
* \param	type			Timer type - one shot or periodic
* \param	data			Timer data
* \param	period			Timer period
* \param	callback		Timer callback function pointer
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the message queue
*/
system_status_t timer_create_timer (timer_ctx_t* ctx, timer_type_t type, void* data, uint32_t period, vFUNCPtr callback);

/**
* Destroy a timer
*
* \param    ctx				Pointer to the timer context to destroy
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t timer_destroy_timer (timer_ctx_t* ctx);

/**
* Start a timer
*
* \param    ctx				Pointer to the timer context to destroy
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t timer_start_timer (timer_ctx_t* ctx);

/**
* Stop a timer
*
* \param    ctx				Pointer to the timer context to destroy
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t timer_stop_timer (timer_ctx_t* ctx);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_TIMER_H_ */
