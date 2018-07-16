
/**
* system_management.c
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementation of routines used for AEF system management call support.
*/

#include "string.h"
#include "aef/embedded/system/system_management.h"
#include "aef/embedded/osal/thread.h"
#include "aef/embedded/osal/time_delay.h"
#include "aef/embedded/osal/critical_section.h"
#include "bsp.h"

#define SM_THREAD_DELAY		100

static sys_management_func_ctrl_t* func_table = NULL;
static uint32_t funcTableSize = (sizeof(sys_management_func_ctrl_t) * MAX_SYS_MANAGEMENT_FUNCS);

static thread_ctx_t	sm_thread_ctx;
static uint32_t sm_thread_stackSize = 1024;
static uint32_t sm_thread_quantum   = DEFAULT_QUANTUM;
static uint32_t sm_thread_priority  = TASK_PRIORITY_NORMAL;

static critical_section_ctx_t sm_thread_cs;

static void sm_thread_task (void* arg);

/**
* Initialize the system management function container
*
* \param 	none
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
system_status_t system_management_function_core_init (void)
{
	/**
	* Setup system management function control table
	*/
	func_table = (sys_management_func_ctrl_t*) malloc ( funcTableSize);
	if ( func_table != NULL )
	{
		memset (func_table, 0, funcTableSize);
		critical_section_create (&sm_thread_cs);

		if ( thread_create ( &sm_thread_ctx,
							 sm_thread_stackSize,
							 sm_thread_quantum,
							 sm_thread_priority,
							 0,
							 sm_thread_task,
							 0 ) == SYSTEM_STATUS_SUCCESS )
		{
			return SYSTEM_STATUS_SUCCESS;
		}
		critical_section_destroy (&sm_thread_cs);
		free ( func_table );
	}
	return SYSTEM_FAILURE_GENERAL;
}

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
system_status_t system_management_func_attach (char* name, void* instance, system_management_func_t func)
{
	if ( func != NULL )
	{
		critical_section_acquire ( &sm_thread_cs );
		sys_management_func_ctrl_t* func_entry = func_table;
		for ( uint32_t index = 0; index < MAX_SYS_MANAGEMENT_FUNCS; func_entry++, index++ )
		{
			if ( func_entry->instance == NULL && func_entry->func == NULL )
			{
				func_entry->name           = name;
				func_entry->instance       = instance;
				func_entry->func           = func;
				func_entry->interval 	   = 0;
				func_entry->interval_count = 0;
				func_entry->state          = SMF_STATE_ENABLED;
				critical_section_release ( &sm_thread_cs );
				thread_resume ( &sm_thread_ctx );
				return SYSTEM_STATUS_SUCCESS;
			}
		}
		critical_section_release ( &sm_thread_cs );
		return SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* Detach a system management function
*
* \param	func		System management function pointer
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
system_status_t system_management_func_detach (system_management_func_t func)
{
	if ( func != NULL )
	{
		critical_section_acquire ( &sm_thread_cs );
		sys_management_func_ctrl_t* func_entry = func_table;
		for ( uint32_t index = 0; index < MAX_SYS_MANAGEMENT_FUNCS; func_entry++, index++ )
		{
			if ( func_entry->func == func )
			{
				func_entry->name           = NULL;
				func_entry->instance       = NULL;
				func_entry->func	       = NULL;
				func_entry->interval 	   = 0;
				func_entry->interval_count = 0;
				func_entry->state          = SMF_STATE_DISABLED;
				critical_section_release ( &sm_thread_cs );
				return SYSTEM_STATUS_SUCCESS;
			}
		}
		critical_section_release ( &sm_thread_cs );
		return SYSTEM_FAILURE_GENERAL;
	}
	return SYSTEM_FAILURE_INVALID_PARAMETER;
}

/**
* System management thread task
*
* Run registered system management functions.
*
* \param	arg		Pointer to task instance data
*
* \returns  none
*/
void sm_thread_task (void* arg)
{
	sys_management_func_ctrl_t* func_entry;

	THREAD_RUN_LOOP
	{
		critical_section_acquire ( &sm_thread_cs );
		func_entry = func_table;
		for ( uint32_t index = 0; index < MAX_SYS_MANAGEMENT_FUNCS; func_entry++, index++ )
		{
			if ( func_entry->func != NULL && func_entry->state == SMF_STATE_ENABLED )
			{
				if ( func_entry->interval == 0)
				{
					(*func_entry->func) (func_entry->instance);
				}
				else
				{
					if ( func_entry->interval_count == 0 )
					{
						(*func_entry->func) (func_entry->instance);
						func_entry->interval_count = func_entry->interval;
					}
					else
					{
						func_entry->interval_count -= (func_entry->interval_count > SM_THREAD_DELAY) ? SM_THREAD_DELAY : func_entry->interval_count;
					}
				}
			}
		}
		critical_section_release ( &sm_thread_cs );
        time_delay (SM_THREAD_DELAY);
	}
}

