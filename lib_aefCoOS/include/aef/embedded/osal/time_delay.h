
/**
* time_delay.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief OSAL time delay definitions.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_OSAL_TIME_DELAY_H_
#define INCLUDE_AEF_EMBEDDED_OSAL_TIME_DELAY_H_

#include <aef/embedded/osal/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <OsConfig.h>
#include <CoOS.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* Delay thread for ms
*
* \param    none
*
* \returns  none
*/
void time_delay (uint32_t ms);

/**
* Delay for a minimum number of microseconds
*
* \param    minimum_delay	microseconds to delay
*
* \returns  none
*/
void time_delay_us (uint32_t minimum_delay);

/**
* Delay thread for ticks
*
* \param    none
*
* \returns  none
*/
void time_delay_ticks (uint32_t ticks);

/**
* Delay thread for ticks using tick structure
*
* \param    none
*
* \returns  none
*/
void time_delay_for (uint32_t tick_ctx);

/**
* Delay thread until ticks using tick structure
*
* \param    none
*
* \returns  none
*/
void time_delay_until (uint32_t tick_ctx);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_OSAL_TIME_DELAY_H_ */
