
/**
* time_delay.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief UEF OSAL time definitions.
*
*/

#include <aef/embedded/osal/time.h>
#include <stdbool.h>

/**
* Get time in seconds/milliseconds
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
void time_get (void* time_ctx)
{
//	_time_get ((TIME_STRUCT_PTR)time_ctx);
}

/**
* Get time in ticks
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
uint64_t time_get_ticks (void* time_ctx)
{
	return CoGetOSTime();
}

/**
* Get time difference in microseconds
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
int32_t time_diff_microseconds (void* end_time_ctx, void* start_time_ctx, bool* overflow)
{
	return 0L;
}

/**
* Get time in seconds/milliseconds
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
void time_get_elapsed (void* time_ctx)
{
//	_time_get_elapsed ((TIME_STRUCT_PTR)time_ctx);
}

/**
* Get time in ticks
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
void time_get_elapsed_ticks (void* time_ctx)
{
}

/**
* Get microseconds since the last periodic timer interrupt.
*
* \param    none
*
* \returns  none
*/
uint16_t time_get_microseconds (void)
{
	return 0;
}

/**
* Get microseconds since processor start.
*
* \param    none
*
* \returns  none
*/
uint64_t time_get_elapsed_microseconds (void)
{
	return ((CoGetOSTime() / CFG_SYSTICK_FREQ) * 1000 * 1000);
}

/**
* Get nanoseconds send the last periodic timer interrupt.
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_nanoseconds (void)
{
	return 0L;
}

/**
* Get resolution of the periodic timer interrupt.
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_resolution (void)
{
	return CFG_SYSTICK_FREQ;
}

/**
* Get the number of hardware ticks since the last tick
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_hwticks (void)
{
	return CoGetOSTime();
}

/**
* Get the number of hardware ticks per tick
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_hwticks_per_tick (void)
{
	return 0L;
}
