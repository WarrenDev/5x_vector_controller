
/**
* rgbled_driver_core.h
*
* \copyright
* Copyright 2016 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief  RGB LED stream driver core implementation.
*
*/

#include <stdlib.h>
#include "fsl_gpio.h"
#include "fsl_ftm.h"
#include "rgbled_driver_core.h"
#include "aef/embedded/osal/time.h"
#include "aef/embedded/osal/time_delay.h"
#include "aef/embedded/osal/thread.h"
#include "MK64F12.h"

/**
* Internal routines
*   Routines used only within this code module
*/
static
driver_status_t
rgbled_core_configure (rgbled_driver_ctx_t* ctx, void* config);

static
driver_status_t
rgbled_core_reset (rgbled_driver_ctx_t* ctx);

static
driver_status_t
rgbled_core_enable (rgbled_driver_ctx_t* ctx);

static
driver_status_t
rgbled_core_disable (rgbled_driver_ctx_t* ctx);

static
driver_status_t
rgbled_core_set_led (rgbled_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size);

static
void
rgbled_core_set_led_color ( rgbled_color_t color);

static
void
rgbled_core_set_led_intensity (rgbled_id_t rgb_led, uint16_t* duty_cycle);

static
void
rgbled_core_hardware_init (void);

#define PWM_FREQUENCY 			((float)(12.0))

/**
* LED GPIO control macros
*/
#define BIT(n)					(1<<(n))

#define LED_CTRL_RED_PIN		10
#define LED_CTRL_RED_BIT		BIT(LED_CTRL_RED_PIN)

#define LED_CTRL_GREEN_PIN		11
#define LED_CTRL_GREEN_BIT		BIT(LED_CTRL_GREEN_PIN)

#define LED_CTRL_BLUE_PIN		5
#define LED_CTRL_BLUE_BIT		BIT(LED_CTRL_BLUE_PIN)

#define RED_LEDS_OFF()			( GPIOB->PCOR |= LED_CTRL_RED_BIT )
#define RED_LEDS_ON()			( GPIOB->PSOR |= LED_CTRL_RED_BIT )
#define GREEN_LEDS_OFF()		( GPIOB->PCOR |= LED_CTRL_GREEN_BIT )
#define GREEN_LEDS_ON()			( GPIOB->PSOR |= LED_CTRL_GREEN_BIT )
#define BLUE_LEDS_OFF()			( GPIOA->PCOR |= LED_CTRL_BLUE_BIT )
#define BLUE_LEDS_ON()			( GPIOA->PSOR |= LED_CTRL_BLUE_BIT )

static uint32_t peripheral_bus_clock = 0L;
static uint32_t mod_value = 0L;

/**
* Open the RGBLED device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the RGBLED device.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
rgbled_core_open (stream_driver_ctx_t* ctx)
{
	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return rgbled_core_configure ((rgbled_driver_ctx_t*)ctx->ctx,(void*)((rgbled_driver_ctx_t*)ctx->ctx)->params);
}

/**
* Close the RGBLED device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the UART.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
rgbled_core_close (stream_driver_ctx_t* ctx)
{
	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return DRIVER_STATUS_SUCCESS;
}

/**
* Read data from the device driver instance.
*
* \param    ctx				Pointer to the device context
* \param    data_buffer		Pointer to the data buffer
* \param    size			Size of the data buffer in bytes
* \param    bytes_read		Pointer to the actual number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to read from the device driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
driver_status_t
rgbled_core_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	if ( (ctx == NULL) || (data_buffer == NULL) )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return DRIVER_STATUS_SUCCESS;
}

/**
* Write data to the device driver instance.
*
* \param    ctx				Pointer to the device context
* \param    data_buffer		Pointer to the data to write
* \param    size			Size of the data to write
* \param    bytes_written	Pointer to the actual number of bytes written
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to write to the device driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
driver_status_t
rgbled_core_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	if ( (ctx == NULL) || (data_buffer == NULL) )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return DRIVER_STATUS_SUCCESS;
}

/**
* Send a command to a device driver instance identified by context.
* This function requires an implementation of open and close.
*
* \param    ctx				Pointer to the device context
* \param    code			I/O control code to perform
* \param    input_buffer	Pointer to the input buffer
* \param    input_size		Input buffer size
* \param    output_buffer	Pointer to the output buffer
* \param    output_size		Output buffer size
* \param    bytes_read		Pointer to the actual bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
*           DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are incorrect.
*/
driver_status_t
rgbled_core_ioctl (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	driver_status_t status = DRIVER_FAILURE_GENERAL;

	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	if ( bytes_read != NULL )
		*bytes_read = 0;

	switch ( code )
	{
		case IOCTL_RGBLED_CONFIGURE:
			status = rgbled_core_configure ((rgbled_driver_ctx_t*)ctx->ctx, input_buffer);
			break;
		case IOCTL_RGBLED_RESET:
			status = rgbled_core_reset((rgbled_driver_ctx_t*)ctx->ctx);
			break;
		case IOCTL_RGBLED_ENABLE:
			status = rgbled_core_enable((rgbled_driver_ctx_t*)ctx->ctx);
			break;
		case IOCTL_RGBLED_DISABLE:
			status = rgbled_core_disable((rgbled_driver_ctx_t*)ctx->ctx);
			break;
		case IOCTL_RGBLED_SET_LED:
			status = rgbled_core_set_led((rgbled_driver_ctx_t*)ctx->ctx, input_buffer, input_size);
			break;
		default:
			break;
	}

	return status;
}

/**
* Initialize the RGBLED interface.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
rgbled_core_init (rgbled_driver_ctx_t* ctx)
{
	rgbled_core_hardware_init ();
	return DRIVER_STATUS_SUCCESS;
}

/**
* Configure the RGBLED output bit stream of the driver context.
*
* \param    ctx				Pointer to the device context
* \param	config			Pointer to the RGBLED configuration structure
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
rgbled_core_configure (rgbled_driver_ctx_t* ctx, void* config)
{
	if ( ctx != NULL )
	{
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Reset the RGBLED driver context.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
rgbled_core_reset (rgbled_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Enable the RGBLED device, taking it out of sleep mode.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
rgbled_core_enable (rgbled_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Disable the RGBLED device, putting it in sleep mode.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
rgbled_core_disable (rgbled_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Set the output state of the RGB LED device.
*
* \param    ctx				Pointer to the device context
* \param	input_buffer	POinter to RGB LED state information
* \param	input_size		Size of RGB LED state information
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
rgbled_core_set_led(rgbled_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && input_buffer != NULL && input_size == sizeof(rgbled_driver_control_t) )
	{
		rgbled_driver_control_t* control = (rgbled_driver_control_t*)input_buffer;

		rgbled_core_set_led_color(control->color_control);
		rgbled_core_set_led_intensity(control->led_control, control->intensity);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Set the RGB LED color
*
* \param    rgb_led			RGB LED map
* \param	duty_cycle		Duty cycle
*
* \returns  None
*/
void rgbled_core_set_led_color ( rgbled_color_t color)
{
	switch ( color )
	{
		case COLOR_BLACK:	// All off
			RED_LEDS_OFF();	GREEN_LEDS_OFF(); BLUE_LEDS_OFF();
			break;
		case COLOR_WHITE:	// All on
			RED_LEDS_ON(); GREEN_LEDS_ON();	BLUE_LEDS_ON();
			break;
		case COLOR_RED:		// Red on
			RED_LEDS_ON(); GREEN_LEDS_OFF(); BLUE_LEDS_OFF();
			break;
		case COLOR_GREEN:	// Green on
			RED_LEDS_OFF();	GREEN_LEDS_ON(); BLUE_LEDS_OFF();
			break;
		case COLOR_BLUE:	// Blue on
			RED_LEDS_OFF();	GREEN_LEDS_OFF(); BLUE_LEDS_ON();
			break;
		case COLOR_ORANGE:	// Red on/Green on
			RED_LEDS_ON(); GREEN_LEDS_ON();	BLUE_LEDS_OFF();
			break;
		case COLOR_PURPLE:	// Red on/Blue on
			RED_LEDS_ON(); GREEN_LEDS_OFF(); BLUE_LEDS_ON();
			break;
		case COLOR_TEAL:
			RED_LEDS_OFF();	GREEN_LEDS_ON(); BLUE_LEDS_ON();
			break;
		case COLOR_CYAN:
		case COLOR_MAGENTA:
		default:
			break;
	}

}

/**
* Set the intensity for a LED
*
* \param    rgb_led_map		RGB LED map
* \param	duty_cycle		Duty cycle
*
* \returns  None
*/
void rgbled_core_set_led_intensity ( rgbled_id_t rgb_led_map, uint16_t* duty_cycle )
{
	uint16_t index = 0;
	uint16_t value;

	if ( rgb_led_map & RGBLED_1 )
	{
		index = 0;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_2 )
	{
		index = 1;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_3 )
	{
		index = 2;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_4 )
	{
		index = 3;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_5 )
	{
		index = 4;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_6 )
	{
		index = 5;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_7 )
	{
		index = 6;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}

	if ( rgb_led_map & RGBLED_8 )
	{
		index = 7;
		value = abs( *(duty_cycle+index) - 100 );
		FTM0->CONTROLS[index].CnV = (uint32_t)((value * .01) * (float)mod_value);
	}
}

/**
* Initialize hardware peripherals used by the RGBLED device driver.
*
* \param    None
*
* \returns  None
*/
void rgbled_core_hardware_init	( void )
{

	uint32_t core_clk_khz = (SystemCoreClock / 1000);
	peripheral_bus_clock  = (core_clk_khz / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1));
	mod_value = (uint32_t)((float)(peripheral_bus_clock)/PWM_FREQUENCY);

	/**
	* Enable the clock to the FTM0 module and port modules
	*/
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	/**
	* Set ALT1 for color control pins
	*/
	PORTA->PCR[5]  = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;		// LED_BLUE_ON GPIO
	PORTB->PCR[10] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;		// LED_RED_ON GPIO
	PORTB->PCR[11] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;		// LED_GREEN_ON GPIO

	/**
	* Configure LED color control pins as outputs and off
	*/
	GPIOB->PDDR |= LED_CTRL_RED_BIT | LED_CTRL_GREEN_BIT;
	GPIOA->PDDR |= LED_CTRL_BLUE_BIT;

	GPIOB->PCOR |= LED_CTRL_RED_BIT | LED_CTRL_GREEN_BIT;
	GPIOA->PCOR |= LED_CTRL_BLUE_BIT;

	/**
	* Route the output of FTM0 channels 1 - 8 to the desired pins
	*/
	PORTC->PCR[1] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTC1 - LED control 1
	PORTC->PCR[2] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTC2 - LED control 2
	PORTC->PCR[3] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTC3 - LED control 3
	PORTC->PCR[4] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTC4 - LED control 4
	PORTD->PCR[4] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTD4 - LED control 5
	PORTD->PCR[5] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTD5 - LED control 6
	PORTD->PCR[6] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTD6 - LED control 7
	PORTD->PCR[7] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;		// PTD7 - LED control 8

	/**
	* Choose EDGE-Aligned PWM: selected when QUADEN=0, DECAPEN=0, COMBINE=0, CPWMS=0, and MSnB=1
	* Properly set up Flex Timer Module
	*/
	/**
	* Disable write protection
	*/
	FTM0->MODE |= FTM_MODE_WPDIS_MASK;

	/**
	* Use only TPM-compatible registers
	*/
	FTM0->MODE &= ~FTM_MODE_FTMEN_MASK;

	/**
	* Disable Quadrature decoder mode
	*/
	FTM0->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;

	/**
	* Initialize common FTM0 registers for PWM generation
	*/
	FTM0->CNT   = 0x0; 			// FTM Counter Value - (initialize the CNT before writing to MOD) (16 bit available - bits 0-15 are count)
	FTM0->MOD   = mod_value;	// Set the Modulo resister (16 bit available - bits 0-15)
	FTM0->CNTIN = 0; 			// Set the Counter Initial Value to 0

	/**
	* Configure channel 0 PWM output
	*/
	FTM0->CONTROLS[0].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[0].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[0].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[0].CnV   = 0;

	/**
	* Configure channel 1 PWM output
	*/
	FTM0->CONTROLS[1].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[1].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[1].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[1].CnV   = 0;

	/**
	* Configure channel 2 PWM output
	*/
	FTM0->CONTROLS[2].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[2].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[2].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[2].CnV   = 0;

	/**
	* Configure channel 3 PWM output
	*/
	FTM0->CONTROLS[3].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[3].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[3].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[3].CnV   = 0;

	/**
	* Configure channel 4 PWM output
	*/
	FTM0->CONTROLS[4].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[4].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[4].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[4].CnV   = 0;

	/**
	* Configure channel 5 PWM output
	*/
	FTM0->CONTROLS[5].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[5].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[5].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[5].CnV   = 0;

	/**
	* Configure channel 6 PWM output
	*/
	FTM0->CONTROLS[6].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[6].CnSC &= ~FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[6].CnSC |= FTM_CnSC_MSB_MASK;
	FTM0->CONTROLS[6].CnV   = 0;

	/**
	* Configure channel 7 PWM output - edged-aligned high-true pulses of PWM signals
	*/
	FTM0->CONTROLS[7].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
//	FTM0->CONTROLS[7].CnSC |= FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[7].CnV   = 0;

	/**
	* Configure FTM0 prescale value and select clock source
	*/
	FTM0->SC = FTM_SC_PS(6) | FTM_SC_CLKS(1);
}

