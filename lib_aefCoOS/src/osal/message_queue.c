
/**
* message_queue.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL message queue implementation.
*/

#include <aef/embedded/osal/message_queue.h>

/**
* Create and initialize a message queue
*
* \param    ctx				Pointer to a context to initialize
* \param	type			Message queue type (0 for MQX)
* \param	message_size	Length of messages in the queue (MQX only)
* \param	queue_size		Number of messages in the queue
* \param	queue			Pointer to message queue information (queue id for MQX)
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the message queue
*/
system_status_t message_queue_create (message_queue_ctx_t* ctx, uint32_t type, uint32_t message_size, uint32_t queue_size, void* queue)
{
	if ( ctx != Co_NULL )
	{
		ctx->queue_id = CoCreateQueue ( &queue, queue_size, (uint8_t)type );
		if ( ctx->queue_id != E_CREATE_FAIL )
		{
			ctx->queue_size = queue_size;
			return SYSTEM_STATUS_SUCCESS;
		}
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Destroy a message queue
*
* \param    ctx				Pointer to a context to destroy
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t message_queue_destroy (message_queue_ctx_t* ctx)
{
	if ( ctx != Co_NULL )
	{
		return CoDelQueue ( ctx->queue_id, OPT_DEL_ANYWAY );
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Post a message to a message queue
*
* \param    ctx				Pointer to a message queue context
* \param	destination		Destination queue number (MQX only)
* \param	message			Pointer to the message to post
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to post the message to the message queue
*/
system_status_t message_queue_post (message_queue_ctx_t* ctx, uint32_t destination, void* message)
{
	if ( ctx != Co_NULL && message != Co_NULL )
	{
		return (system_status_t) CoPostQueueMail ( ctx->queue_id, message );
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Poll for a message in a message queue.
* This call does not wait.  It returns immediately.
*
* \param    ctx				Pointer to a context to initialize
* \param	message			Pointer message area
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if the message queue is empty or error
*/
system_status_t message_queue_poll (message_queue_ctx_t* ctx, void** message)
{
	if ( ctx != Co_NULL && message != Co_NULL )
	{
		StatusType result;
		*message = CoAcceptQueueMail ( ctx->queue_id, &result );
		return (system_status_t) result;
	}
	return SYSTEM_FAILURE_GENERAL;
}

/**
* Wait for a message in a message queue
*
* \param    ctx				Pointer to a context to initialize
* \param	timeout			Number of milliseconds to wait
* \param	message			Pointer message area
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if the message queue is empty, timeout, or error
*/
system_status_t message_queue_receive (message_queue_ctx_t* ctx, uint32_t timeout, void** message)
{
	if ( ctx != Co_NULL && message != Co_NULL )
	{
		StatusType result;
		*message = CoPendQueueMail ( ctx->queue_id, timeout, &result );
		return (system_status_t)result;
	}
	return SYSTEM_FAILURE_GENERAL;
}

