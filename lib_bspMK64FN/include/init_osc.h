
/**
* init_bsp.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Oscillator initialization definitions for the board support package
*/

#ifndef INCLUDE_INIT_OSC_H_
#define INCLUDE_INIT_OSC_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void BOARD_BootClockVLPR(void);
void BOARD_BootClockRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* INCLUDE_INIT_OSC_H_ */
