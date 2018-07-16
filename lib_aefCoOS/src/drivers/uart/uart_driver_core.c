
/**
* uart_driver_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  UART stream driver core implementation.
*
*/

#include "uart_driver_core.h"

static
driver_status_t
uart_core_ioctl_initialize (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size);

static
driver_status_t
uart_core_ioctl_getchar (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static
driver_status_t
uart_core_ioctl_putchar (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_read);

static
driver_status_t
uart_core_ioctl_charpresent (stream_driver_ctx_t* ctx, uint32_t* bytes_read);

static
driver_status_t
uart_core_ioctl_flush (stream_driver_ctx_t* ctx);

static
driver_status_t
uart_core_ioctl_waitfordone (stream_driver_ctx_t* ctx);

static
driver_status_t
uart_core_ioctl_setmode (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size);

/**
* Open the UART device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the UART.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
uart_core_open (stream_driver_ctx_t* ctx, uint32_t mode, uint32_t options)
{
	if ( ctx != NULL )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
		if ( uart_ctx->ref_count == 0 )
		{
			uart_config_t config;
			uart_config_parms_t* uart_config = uart_ctx->params;

			UART_GetDefaultConfig(&config);

			config.enableRx = TRUE;
			config.enableTx = TRUE;
			if ( uart_config != NULL )
			{
				config.baudRate_Bps    = uart_config->baud;
				config.parityMode      = (uart_parity_mode_t)uart_config->parity;
				config.stopBitCount    = (uart_stop_bit_count_t)(uart_config->stop_bits-1);
			}

			if ( uart_ctx->mode == UARTMODE_LINE)
			{
				uart_ctx->event_handle = uart_config->event_handle;
			}

			UART_Init(uart_ctx->base, &config, CLOCK_GetFreq(uart_ctx->srcClock_Hz));

		    /*
		    * Enable RX interrupt.
		    */
		    UART_EnableInterrupts(uart_ctx->base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
		    EnableIRQ(uart_ctx->interrupt);
			uart_ctx->ref_count++;
			return DRIVER_STATUS_SUCCESS;
		}
		else
		{
			if ( uart_ctx->mode == mode && uart_ctx->params == (void*)options )
			{
				uart_ctx->ref_count++;
				return DRIVER_STATUS_SUCCESS;
			}
		}
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Close the UART device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the UART.
*/
driver_status_t
uart_core_close (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
		if ( uart_ctx->ref_count )
		{
			uart_ctx->ref_count--;
			if ( uart_ctx->ref_count == 0 )
			{
				UART_Deinit( uart_ctx->base );
			    UART_DisableInterrupts( uart_ctx->base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable );
			    DisableIRQ( uart_ctx->interrupt);
			}
		}
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read data from the device driver instance.
*
* \param    ctx				Pointer to the device context
* \param    data_buffer		Pointer to the data buffer
* \param    size			Size of the data buffer
* \param    bytes_read		Pointer to the actual number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to read from the device driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
driver_status_t
uart_core_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	if ( (ctx != NULL) && (data_buffer != NULL) )
	{
		uint8_t* data = (uint8_t*)data_buffer;
		uint32_t bytes_returned = 0;
	    uint16_t i = size;

	    while(i > 0)
	    {
	    	uart_core_ioctl_getchar (ctx, data, i, &bytes_returned);
	    	if ( bytes_returned )
	    	{
	    		data++;
	    		i--;
	    	}
	    }
	    if ( bytes_read != NULL)
			*bytes_read = (size-i);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
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
uart_core_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	if ( (ctx != NULL) && (data_buffer != NULL) )
	{
		uint8_t* data = (uint8_t*)data_buffer;
		uint32_t result = 0;
		uint32_t written = 0;

	    for(written=0; written<size; written++)
	    {
	    	uart_core_ioctl_putchar (ctx, data++, 1L, &result);
	    	if (result == 0 )
	    		break;
	    }
	    if ( bytes_written != NULL )
			*bytes_written = written;
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
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
uart_core_ioctl (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	driver_status_t result = DRIVER_FAILURE_GENERAL;

	if ( bytes_read != NULL )
		*bytes_read = 0L;

	switch ( code )
	{
		case IOCTL_UART_INITIALIZE:
			result = uart_core_ioctl_initialize (ctx, input_buffer, input_size);
			break;
		case IOCTL_UART_GETCHAR:
			result = uart_core_ioctl_getchar (ctx,output_buffer,output_size,bytes_read);
			break;
		case IOCTL_UART_PUTCHAR:
			result = uart_core_ioctl_putchar (ctx,input_buffer,input_size,bytes_read);
			break;
		case IOCTL_UART_CHARPRESENT:
			result = uart_core_ioctl_charpresent (ctx,bytes_read);
			break;
		case IOCTL_UART_FLUSH:
			result = uart_core_ioctl_flush (ctx);
			break;
		case IOCTL_UART_WAITFORDONE:
			result = uart_core_ioctl_waitfordone (ctx);
			break;
		case IOCTL_UART_SETMODE:
			result = uart_core_ioctl_setmode (ctx,input_buffer,input_size);
			break;
		default:
			break;
	}
	return result;
}

/**
* Initialize the UART peripheral device
*
* \param    ctx				Pointer to the device context
* \param    input_buffer	Pointer to the input buffer
* \param    input_size		Input buffer size
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
*           DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are incorrect.
*/
driver_status_t
uart_core_ioctl_initialize (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( (ctx != NULL) && (input_buffer != NULL) && (input_size == sizeof(uart_config_parms_t)) )
	{
//		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
//		uart_config_parms_t* parms = (uart_config_parms_t*)input_buffer;

		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Wait for a character to arrive in the UART and retrieve it.
*
* \param    ctx				Pointer to the device context
* \param    output_buffer	Pointer to the output buffer to write the character
* \param    output_size		Output buffer size, should be 1
* \param    bytes_read		Pointer to the actual bytes returned, will be 1
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
*           DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are incorrect.
*/
driver_status_t
uart_core_ioctl_getchar (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( (ctx != NULL) && (output_buffer != NULL) && (output_size >= 1) )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
		uint32_t actual_bytes_read = 0;
        if ( ! buf_isempty(uart_ctx->rx_buffer) )
        {
        	*(uint8_t*)output_buffer =  buf_get_byte(uart_ctx->rx_buffer);
		    UART_EnableInterrupts(uart_ctx->base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
        	actual_bytes_read = 1;
        }
		if ( bytes_read != NULL )
			*bytes_read = actual_bytes_read;
        if ( actual_bytes_read )
        {
    		return DRIVER_STATUS_SUCCESS;
        }
        return DRIVER_FAILURE_GENERAL;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Wait for space in the UART FIFO and transfer a character.
*
* \param    ctx				Pointer to the device context
* \param    input_buffer	Pointer to the input buffer containing the char to transfer
* \param    input_size		Input buffer size, should be 1
* \param    bytes_read		Pointer to the actual bytes transferred, will be 1
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
*           DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are incorrect.
*/
driver_status_t
uart_core_ioctl_putchar (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size, uint32_t* bytes_read)
{
	if ( (ctx != NULL) && (input_buffer != NULL) && (input_size >= 1) )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;

        if ( buf_isempty(uart_ctx->tx_buffer) && (kUART_TxFifoEmptyFlag & UART_GetStatusFlags(uart_ctx->base)) )
        {
            UART_WriteByte(uart_ctx->base, *(uint8_t*)input_buffer);
        }
        else
        {
	        while(buf_isfull(uart_ctx->tx_buffer))        // Spin wait while full
	            ;
        	buf_put_byte(uart_ctx->tx_buffer, *(uint8_t*)input_buffer);
        }
    	UART_EnableInterrupts(uart_ctx->base, kUART_TxDataRegEmptyInterruptEnable);
		if ( bytes_read != NULL )
			*bytes_read = 1;
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Check to see if a character has been received.
*
* \param    ctx				Pointer to the device context
* \param    bytes_read		Pointer to the result (>0 = characters available, 0 = no characters)
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
*           DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are incorrect.
*/
driver_status_t
uart_core_ioctl_charpresent (stream_driver_ctx_t* ctx, uint32_t* bytes_read)
{
	if ( (ctx != NULL) && (bytes_read != NULL) )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
		*bytes_read = buf_len(uart_ctx->rx_buffer);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Write all buffered bytes to the device.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
* 			Error indicator on error;
*/
driver_status_t
uart_core_ioctl_flush (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
	    buf_reset(uart_ctx->tx_buffer, uart_ctx->buffer_size);
	    buf_reset(uart_ctx->rx_buffer, uart_ctx->buffer_size);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Wait for all bytes to be transferred. This is a blocking call.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
* 			Error indicator on error.
*/
driver_status_t
uart_core_ioctl_waitfordone (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
        while ( ! buf_isempty(uart_ctx->tx_buffer) ) ;
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Wait for space in the UART FIFO and transfer a character.
*
* \param    ctx				Pointer to the device context
* \param    input_buffer	Pointer to mode parameter memory
* \param    input_size		Input buffer size
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
*           DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are incorrect.
*/
driver_status_t
uart_core_ioctl_setmode (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		if ( input_buffer != NULL && input_size == sizeof(uart_mode_parms_t) )
		{
			uart_mode_parms_t* parms = (uart_mode_parms_t*)input_buffer;
			uart_driver_ctx_t* uart_ctx = (uart_driver_ctx_t*)ctx->ctx;
			uart_ctx->mode = parms->mode;
			if( parms->event_handle != NULL )
			{
				uart_ctx->event_handle = parms->event_handle;
			}
			return DRIVER_STATUS_SUCCESS;
		}
		return DRIVER_FAILURE_INVALID_PARAMETER;
	}
	return DRIVER_FAILURE_GENERAL;
}
