
/**
* critical_section.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL critical section definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_CRITICAL_SECTION_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_CRITICAL_SECTION_H_

#include <aef/embedded/system/system_status.h>
#include <stdint.h>
#include <CoOS.h>
#include <OsCore.h>
#include <OsMutex.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* The critical section context.
*/
typedef struct critical_section_ctx_def
{
	OS_MutexID			mutex;
	uint32_t			ref_count;
} critical_section_ctx_t;

/**
* Create and initialize a critical critical section
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to create the critical section object
*/
system_status_t critical_section_create (critical_section_ctx_t* ctx);

/**
* Destroy a critical critical section
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to destroy the critical section object
*/
system_status_t critical_section_destroy (critical_section_ctx_t* ctx);

/**
* Enter the critical critical section.  This is blocking call.
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to enter the critical section
*/
system_status_t critical_section_acquire (critical_section_ctx_t* ctx);

/**
* Try to enter the critical critical section
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to enter the critical section
*/
system_status_t critical_section_try_acquire (critical_section_ctx_t* ctx);

/**
* Exit the critical critical section.
*
* \param    ctx		Pointer to a critical section context
*
* \returns  SYSTEM_STATUS_SUCCESS if successful.
* 			SYSTEM_FAILURE_GENERAL on failure to enter the critical section
*/
system_status_t critical_section_release (critical_section_ctx_t* ctx);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_CRITICAL_SECTION_H_ */
