
/**
* event.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL event definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_EVENT_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_EVENT_H_

#include <aef/embedded/system/system_status.h>
#include <stdint.h>
#include <stdbool.h>
#include <CoOS.h>
#include "bsp.h"

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus


#define EVENT_WAIT_INFINITE		0L
#define GROUP_MAX_EVENTS		32

/**
* The event context.
*/
typedef struct event_ctx_def
{
	char*		name;
	uint32_t	event_mask;
	bool		manual_reset;
} event_ctx_t;

/**
* Create and initialize an event
*
* \param    ctx				Pointer to a context to initialize
* \param	name			Pointer to the event name
* \param	Manual_reset	Manual reset event
* \param	initial_state	Initial state of the event
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the event object
*/
system_status_t event_create (event_ctx_t* ctx, char* name, bool manual_reset, bool initial_state);

/**
* Destroy an event
*
* \param    ctx				Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the event object
*/
system_status_t event_destroy (event_ctx_t* ctx);

/**
* Accept single event
*
* Check to see if the event has been signaled.
* If it has then the thread owns the event.
*
* \param    ctx				Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful, the event was set
* 			SYSTEM_FAILURE_NOT_READY if event is not set
* 			SYSTEM_FAILURE_GENERAL if invalid event or error
*/
system_status_t event_accept_single (event_ctx_t* ctx);

/**
* Wait for single event to be set
*
* \param    ctx				Pointer to a event context
* \param	timeout			Timeout value in ms.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_wait_single (event_ctx_t* ctx, uint32_t timeout);

/**
* Accept all specified event bits to be set
*
* \param    ctx				Pointer to a event context
* \param	value			Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_accept_all (event_ctx_t* ctx, uint32_t* value);

/**
* Accept any of the specified event bits to be set
*
* \param    ctx				Pointer to a event context
* \param	value			Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_accept_any (event_ctx_t* ctx, uint32_t* value);

/**
* Wait for all specified event bits to be set
*
* \param    ctx				Pointer to a event context
* \param	timeout			Timeout value in ms.
* \param	value			Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_wait_all (event_ctx_t* ctx, uint32_t timeout, uint32_t* value);

/**
* Wait for any of the specified event bits to be set
*
* \param    ctx				Pointer to a event context
* \param	timeout			Timeout value in ms.
* \param	value			Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_wait_any (event_ctx_t* ctx, uint32_t timeout, uint32_t* value);

/**
* Clear the specified event bits
*
* \param    ctx				Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t event_reset(event_ctx_t* ctx);

/**
* Set the specified event bits
*
* \param    ctx		Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t event_signal(event_ctx_t* ctx);

/**
* Get the current event mask value
*
* \param    ctx		Pointer to a event context
* \param	value	Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure
*/
system_status_t event_get_value(event_ctx_t* ctx, uint32_t* value);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_EVENT_H_ */
