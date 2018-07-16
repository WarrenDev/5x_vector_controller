
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

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_TIME_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_TIME_H_

#include <stdint.h>
#include <stdbool.h>
#include <CoOS.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* Get time in seconds/milliseconds
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
void time_get (void* time_ctx);

/**
* Get time in ticks
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
uint64_t time_get_ticks (void* time_ctx);

/**
* Get time difference in microseconds
*
* \param    time_ctx	Pointer to a time context
*
* \returns  Time difference in microseconds
*/
int32_t time_diff_microseconds (void* end_time_ctx, void* start_time_ctx, bool* overflow);

/**
* Get time in seconds/milliseconds
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
void time_get_elapsed (void* time_ctx);

/**
* Get time in ticks
*
* \param    time_ctx	Pointer to a time context
*
* \returns  none
*/
void time_get_elapsed_ticks (void* time_ctx);

/**
* Get microseconds send the last periodic timer interrupt.
*
* \param    none
*
* \returns  none
*/
uint16_t time_get_microseconds (void);

/**
* Get microseconds since processor start.
*
* \param    none
*
* \returns  none
*/
uint64_t time_get_elapsed_microseconds (void);

/**
* Get nanoseconds send the last periodic timer interrupt.
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_nanoseconds (void);

/**
* Get resolution of the periodic timer interrupt.
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_resolution (void);

/**
* Get the number of hardware ticks since the last tick
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_hwticks (void);

/**
* Get the number of hardware ticks per tick
*
* \param    none
*
* \returns  none
*/
uint32_t time_get_hwticks_per_tick (void);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_TIME_H_ */
