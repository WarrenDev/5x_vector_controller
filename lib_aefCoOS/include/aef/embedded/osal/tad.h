
/**
*  tad.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Task-aware debugging support definitions.
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_TAD_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_TAD_H_

#include <CoOS.h>
#include <stdint.h>
#include <stdbool.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

#define MAX_TAD_MESSAGE_LENGTH		128
#define MEM_TYPE_MAX_STRING_LENGTH 	32

typedef struct _mqx_tad_stack_def
{
	uint32_t	StackBase;
	uint32_t	StackLimit;
	uint32_t	StackUsed;
	int32_t		PercentUsed;
	bool		overflow;
} mqxTadStack_t;

/**
 * @brief   Callback function prototype to return a TAD message.
 *
 * @param   mesage	Pointer to buffer containing the message
 * @param   size	Length of the message
 *
 * @return  None
 */
typedef void (*pfn_message_output_cb_t)( char* message, uint8_t size );

/**
 * @brief   Calculate memory block usage
 *
 * @param   pfn_callback	Message output callback
 *
 * @return  None
 */
void memory_block_usage	(pfn_message_output_cb_t pfn_callback);

/**
 * @brief   Calculate task stack usage
 *
 * @param   pfn_callback	Message output callback
 *
 * @return  None
 */
void task_stack_usage (pfn_message_output_cb_t pfn_callback);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_TAD_H_ */
