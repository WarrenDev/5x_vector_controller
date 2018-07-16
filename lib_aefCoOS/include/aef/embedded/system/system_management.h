
/**
* system_management.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used for AEF system management call support.
*/

#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_MANAGEMENT_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_MANAGEMENT_H_

#include <stdint.h>
#include <aef/embedded/system/system_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define	MAX_SYS_MANAGEMENT_FUNCS	25

/**
* Function pointer definition for system management thread function
*/
typedef void (*system_management_func_t) (void* instance);

/**
* System management function state
*/
#define SMF_STATE_DISABLED		0x00000000
#define SMF_STATE_ENABLED		0x00000001

/**
* System management function structure definition
*/
typedef struct _system_management_func_ctrl_def
{
	uint32_t state;
	uint32_t interval;
	uint32_t interval_count;
	char* name;
	void* instance;
	system_management_func_t func;
} sys_management_func_ctrl_t;

/**
* Initialize the system management function container
*
* \param 	none
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
system_status_t system_management_function_core_init (void);

/**
* Attach a system management function
*
* \param	name		System management function name
* \param 	instance	System management function instance data
* \param	func		System management function pointer
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
system_status_t system_management_func_attach (char* name, void* instance, system_management_func_t func);

/**
* Detach a system management function
*
* \param	func		System management function pointer
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
system_status_t system_management_func_detach (system_management_func_t func);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_MANAGEMENT_H_ */
