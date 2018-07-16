
/**
* init_bsp.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Routine to initialize the board support package
*/

#include "fsl_common.h"
#include "fsl_port.h"

#include "init_osc.h"

/*
* Initialize the boards support package
*
* Configures default pin mappings for peripherals.
*/
void __init_bsp(void)
{
    /* Declare and initialise for pull up configuration */
    port_pin_config_t pinConfig = {0};
    pinConfig.pullSelect = kPORT_PullUp;
//    pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
    pinConfig.mux = kPORT_MuxAsGpio;
    pinConfig.driveStrength = 1;

    /* Initialize UART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Affects PORTB_PCR16 register */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    /* Led pin mux Configuration */
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAsGpio);

    /* Initialize the LED bulb pin */
    /* Ungate the port clock */
//    CLOCK_EnableClock(kCLOCK_PortE);
    /* Led Pulb pin mux Configuration */
//    PORT_SetPinConfig(PORTE, 24U, &pinConfig);

    /* Initialize the LED bulb pin */
    /* Ungate the port clock */
//    CLOCK_EnableClock(kCLOCK_PortD);
    /* Led Pulb pin mux Configuration */
//    PORT_SetPinConfig(PORTD, 7U, &pinConfig);

    BOARD_BootClockRUN();
}


