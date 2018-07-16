
/**
* bsp.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions that make up the board support package
*/

#ifndef INCLUDE_BSP_H_
#define INCLUDE_BSP_H_

#include <CoOS.h>
#include "system_MK64F12.h"

#define BOARD_XTAL0_CLK_HZ 		50000000U
#define BOARD_XTAL32K_CLK_HZ 	32768U
#define BOARD_CORE_CLOCK		120000000U

#define free(mem_block)			CoKfree(mem_block)
#define malloc(size)			CoKmalloc(size)

#ifndef NULL
#define NULL					Co_NULL
#endif

#ifndef TRUE
#define TRUE					Co_TRUE
#endif

#ifndef FALSE
#define FALSE					Co_FALSE
#endif

void __init_bsp(void);

#endif /* INCLUDE_BSP_H_ */
