
/**
* message_queue.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL message queue definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_MESSAGE_QUEUE_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_MESSAGE_QUEUE_H_

#include <aef/embedded/system/system_status.h>
#include <stdint.h>
#include <CoOS.h>
//#include <OsQueue.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* The message queue context.
*/
typedef struct message_queue_ctx_def
{
	OS_EventID		queue_id;
	uint32_t		queue_size;
	OS_MMID			message_pool_id;
	uint32_t		message_size;
} message_queue_ctx_t;

/**
* Create and initialize a message queue
*
* \param    ctx				Pointer to a context to initialize
* \param	type			Message queue type (0 for MQX)
* \param	message_size	Length of messages in the queue
* \param	queue_size		Number of messages in the queue
* \param	queue			Pointer to message queue information (NULL for MQX)
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the message queue
*/
system_status_t message_queue_create (message_queue_ctx_t* ctx, uint32_t type, uint32_t message_size, uint32_t queue_size, void* queue);

/**
* Destroy a message queue
*
* \param    ctx				Pointer to a context to destroy
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the message queue
*/
system_status_t message_queue_destroy (message_queue_ctx_t* ctx);

/**
* Post a message to a message queue
*
* \param    ctx				Pointer to a context to initialize
* \param	destination		Destination queue number
* \param	message			Pointer to the message to post
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to post the message to the message queue
*/
system_status_t message_queue_post (message_queue_ctx_t* ctx, uint32_t destination, void* message);

/**
* Poll for a message in a message queue.
*
* \param    ctx				Pointer to a context to initialize
* \param	message			Pointer message area
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if the message queue is empty or error
*/
system_status_t message_queue_poll (message_queue_ctx_t* ctx, void** message);

/**
* Wait for a message in a message queue
* The caller must call _msg_free() on the pointer returned after processing
* the message when it no longer needs the message instance.
*
* \param    ctx				Pointer to a context to initialize
* \param	timeout			Number of milliseconds to wait
* \param	message			Pointer message area
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL if the message queue is empty, timeout, or error
*/
system_status_t message_queue_receive (message_queue_ctx_t* ctx, uint32_t timeout, void** message);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_MESSAGE_QUEUE_H_ */
