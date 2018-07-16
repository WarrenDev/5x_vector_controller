
/**
* event.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL event implementation.
*
*/

#include <aef/embedded/osal/event.h>

static void event_core_internal_reset ( uint32_t event_mask );
static void event_core_internal_set ( uint32_t event_mask );

/**
* Create and initialize an event
*
* \param    ctx				Pointer to a context to initialize
* \param	name			Pointer to the event name
* \param	Manual_reset	Manual reset event
* \param	initial_state	Initial state of the event
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the event
*/
system_status_t event_create (event_ctx_t* ctx, char* name, bool manual_reset, bool initial_state)
{
	if ( ctx != Co_NULL )
	{
		OS_FlagID event = CoCreateFlag ( !manual_reset, initial_state );
		if ( event != E_CREATE_FAIL )
		{
			ctx->event_mask |= (1 << event);
			ctx->name = name;
			ctx->manual_reset = manual_reset;
			return SYSTEM_STATUS_SUCCESS;
		}
		return SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Destroy an event
*
* \param    ctx		Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the event object
* 			SYSTEM_FAILURE_INVALID_PARAMETER on a null context pointer
*/
system_status_t event_destroy (event_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		for ( OS_FlagID event = 0; event < GROUP_MAX_EVENTS; event++ )
		{
			if ( ctx->event_mask & (1<<event) )
			{
				CoDelFlag ( event, OPT_DEL_ANYWAY );
			}
		}
		return SYSTEM_STATUS_SUCCESS;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

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
system_status_t event_accept_single (event_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		for ( OS_FlagID event = 0; event < GROUP_MAX_EVENTS; event++ )
		{
			if ( ctx->event_mask & (1<<event) )
			{
				StatusType status = CoAcceptSingleFlag (event);
				switch ( status )
				{
					case E_OK:				return SYSTEM_STATUS_SUCCESS;
					case E_FLAG_NOT_READY:	return SYSTEM_FAILURE_NOT_READY;
				}
			}
		}
		return SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Accept all specified event bits to be set
*
* Check to see if any of the specified events have been signaled.
* If so then the thread owns the event.
*
* \param    ctx				Pointer to a event context
* \param	value			Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_accept_all (event_ctx_t* ctx, uint32_t* value)
{
	if ( ctx != Co_NULL )
	{
		StatusType result;
		uint32_t flags = CoAcceptMultipleFlags ( ctx->event_mask, OPT_WAIT_ALL, &result );

		if ( result == E_OK )
		{
			if ( value )
			{
				*value = flags;
			}
			if ( ctx->manual_reset )
			{
				event_core_internal_reset ( flags );
			}
			return result;
		}
		if ( result == E_FLAG_NOT_READY )
		{
			return SYSTEM_FAILURE_NOT_READY;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Accept any of the specified event bits to be set
*
* Check to see if all of the specified events have been signaled.
* If so then the thread owns the event.
*
* \param    ctx				Pointer to a event context
* \param	value			Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_accept_any (event_ctx_t* ctx, uint32_t* value)
{
	if ( ctx != Co_NULL )
	{
		StatusType result;
		uint32_t flags = CoAcceptMultipleFlags ( ctx->event_mask, OPT_WAIT_ANY, &result );

		if ( result == E_OK )
		{
			if ( value )
			{
				*value = flags;
			}
			if ( ctx->manual_reset )
			{
				event_core_internal_reset ( flags );
			}
			return SYSTEM_STATUS_SUCCESS;
		}
		if ( result == E_FLAG_NOT_READY )
		{
			return SYSTEM_FAILURE_NOT_READY;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Wait for single event to be set
*
* \param    ev				Pointer to a event context
* \param	timeout			Timeout value in ms.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			Error value on failure or timeout
*/
system_status_t event_wait_single (event_ctx_t* ctx, uint32_t timeout)
{
	if ( ctx != Co_NULL )
	{
		for ( OS_FlagID event = 0; event < GROUP_MAX_EVENTS; event++ )
		{
			if ( ctx->event_mask & (1<<event) )
			{
				StatusType status = CoWaitForSingleFlag (event, timeout);
				switch ( status )
				{
					case E_OK:		return SYSTEM_STATUS_SUCCESS;
					case E_TIMEOUT:	return SYSTEM_FAILURE_TIMEOUT;
				}
			}
		}
		return SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Wait for all specified event bits to be set
*
* \param    ctx		Pointer to a event context
* \param	timeout	Timeout value in ms.
* \param	value	Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_INVALID_PARAMETER on a null context pointer
* 			Error value on failure or timeout
*/
system_status_t event_wait_all (event_ctx_t* ctx, uint32_t timeout, uint32_t* value)
{
	if ( ctx != Co_NULL )
	{
		StatusType result;
		uint32_t flags = CoWaitForMultipleFlags ( ctx->event_mask, OPT_WAIT_ALL, timeout, &result );

		if ( result == E_OK )
		{
			if ( value )
			{
				*value = flags;
			}
			if ( ctx->manual_reset )
			{
				event_core_internal_reset ( flags );
			}
			return SYSTEM_STATUS_SUCCESS;
		}
		if ( result == E_TIMEOUT )
		{
			return SYSTEM_FAILURE_TIMEOUT;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Wait for any of the specified event bits to be set
*
* \param    ctx		Pointer to a event context
* \param	mask	Mask of event bits to wait for
* \param	timeout	Timeout value in ms.
* \param	value	Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_INVALID_PARAMETER on a null context pointer
* 			Error value on failure or timeout
*/
system_status_t event_wait_any (event_ctx_t* ctx, uint32_t timeout, uint32_t* value)
{
	if ( ctx != Co_NULL )
	{
		StatusType result;
		uint32_t flags = CoWaitForMultipleFlags ( ctx->event_mask, OPT_WAIT_ANY, timeout, &result );

		if ( result == E_OK )
		{
			if ( value )
			{
				*value = flags;
			}
			if ( ctx->manual_reset )
			{
				event_core_internal_reset ( flags );
			}
			return SYSTEM_STATUS_SUCCESS;
		}
		if ( result == E_TIMEOUT )
		{
			return SYSTEM_FAILURE_TIMEOUT;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Clear the specified event bits
*
* \param    ctx		Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILUER_INVALID_PARAMETER on a null context pointer
* 			Error value on failure
*/
system_status_t event_reset(event_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		event_core_internal_reset ( ctx->event_mask );
		return SYSTEM_STATUS_SUCCESS;
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Set the specified event bits
*
* \param    ctx		Pointer to a event context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_INVALID_PARAMETER on a null context pointer
* 			Error value on failure
*/
system_status_t event_signal(event_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		event_core_internal_set ( ctx->event_mask );
		return SYSTEM_STATUS_SUCCESS;
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Get the current event mask value
*
* \param    ctx		Pointer to a event context
* \param	value	Pointer to value which will be loaded with the current event mask value.
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_INVALID_PARAMETER on a null context pointer
* 			Error value on failure
*/
system_status_t event_get_value(event_ctx_t* ctx, uint32_t* value)
{
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Internal routine to set the specified event bits
*
* \param    event_mask		Event bit mask
*
* \returns  none
*/
void event_core_internal_set ( uint32_t event_mask )
{
	for ( OS_FlagID event = 0; event < GROUP_MAX_EVENTS; event++ )
	{
		if ( event_mask & (1<<event) )
		{
			CoSetFlag ( event );
		}
	}
}

/**
* Internal routine to clear the specified event bits
*
* \param    event_mask		Event bit mask
*
* \returns  none
*/
void event_core_internal_reset ( uint32_t event_mask )
{
	for ( OS_FlagID event = 0; event < GROUP_MAX_EVENTS; event++ )
	{
		if ( event_mask & (1<<event) )
		{
			CoClearFlag ( event );
		}
	}
}

