
/**
* time_delay.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL time delay implementation.
*
*/

#include <aef/embedded/osal/time_delay.h>

/**
* Delay thread for ms
*
* \param    none
*
* \returns  none
*/
void time_delay (uint32_t ms)
{
	uint32_t ticks = (ms*CFG_SYSTICK_FREQ + 500)/1000;
	CoTickDelay (ticks);
}

/**
* Delay for a minimum number of microseconds
*
* \param    minimum_delay	microseconds to delay (Range: 0 - 4999)
*
* \returns  none
*/
void time_delay_us (uint32_t minimum_delay)
{
}

/**
* Delay thread for ticks
*
* \param    none
*
* \returns  none
*/
void time_delay_ticks (uint32_t ticks)
{
	CoTickDelay (ticks);
}

/**
* Delay thread for ticks using tick structure
*
* \param    none
*
* \returns  none
*/
void time_delay_for (uint32_t tick_ctx)
{
//	_time_delay_for(tick_ctx);
}

/**
* Delay thread until ticks using tick structure
*
* \param    none
*
* \returns  none
*/
void time_delay_until (uint32_t tick_ctx)
{
//	_time_delay_until(tick_ctx);
}

