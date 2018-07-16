
/**
* uartc_driver_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief UARTC driver implementation.
*/

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>

#include "uart_driver_core.h"

#define DRIVER_NAME			(char*)"IUARTC"
#define UART_BASE			UART2
#define UART_INT			UART2_RX_TX_IRQn
#define UART_CLKSRC 		UART2_CLK_SRC
#define DEFAULT_BAUD_RATE	115200
#define DEFAULT_OPEN_PARMS	0
#define BUFFER_SIZE 		256

static device_driver_id_t	driver_id = DRV_IUART_C;
static device_runlevel_t    driver_runlevel = DRV_RUNLEVEL1;
static stream_driver_ctx_t	stream_ctx;
static uart_driver_ctx_t	uart_ctx;

/**
* Circular buffers for transmit and receive
*/
static uint8_t _tx_buffer[sizeof(RingBuffer) + BUFFER_SIZE] __attribute__ ((aligned(4)));
static uint8_t _rx_buffer[sizeof(RingBuffer) + BUFFER_SIZE] __attribute__ ((aligned(4)));
static RingBuffer* const tx_buffer = (RingBuffer*) &_tx_buffer;
static RingBuffer* const rx_buffer = (RingBuffer*) &_rx_buffer;

/**
* UART2_RX_TX_IRQHandler
*/
void UART2_RX_TX_IRQHandler(void)
{
    uint32_t status_flags = UART_GetStatusFlags(UART_BASE);

	/**
	 * If there is received data, read it into the receive buffer.  If the
	 * buffer is full, disable the receive interrupt.
	 */
	if( (status_flags & (kUART_RxDataRegFullFlag | kUART_RxOverrunFlag)) && !buf_isfull(rx_buffer) )
	{
		uint8_t uart_byte = UART_ReadByte(UART_BASE);
    	buf_put_byte( rx_buffer, uart_byte );

    	/**
    	 * Handle line mode
    	 */
    	if ( (uart_ctx.event_handle != NULL ) && (uart_ctx.mode == UARTMODE_LINE) && (uart_byte == CHAR_EOL || uart_byte == CHAR_CONT) )
    	{
    		event_signal ( uart_ctx.event_handle );
    	}
	}

    /**
     * If transmit data register empty, and data in the transmit buffer,
     * send it.  If it leaves the buffer empty, disable the transmit interrupt.
     */
	if ( (status_flags & kUART_TxDataRegEmptyFlag) && !buf_isempty(tx_buffer) )
	{
        UART_WriteByte(UART_BASE, buf_get_byte(tx_buffer));
        if(buf_isempty(tx_buffer))
        	UART_DisableInterrupts(UART_BASE, kUART_TxDataRegEmptyInterruptEnable);
	}
}

/**
* Get the id tag of the device driver.
*
* \param    none
*
* \returns  id tag of the device driver
*/
static
device_driver_id_t uart_getid (void)
{
	return driver_id;
}

/**
* Get the name tag of the device driver.
*
* \param    none
*
* \returns  Pointer to the device driver name tag
*/
static
char* uart_getname (void)
{
	return stream_ctx.name;
}

/**
* Get the run level of the device driver.  The run level determines the
* system load order.
*
* \param    none
*
* \returns  Run level of the device drover
*/
static
device_runlevel_t uart_runlevel (void)
{
	return driver_runlevel;
}

/**
* Close the device driver instance identified by context.
*
* \param    ctx		Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the device driver.
*/
static
driver_status_t uart_close (stream_driver_ctx_t* ctx)
{
	return uart_core_close (ctx);
}

/**
* De-initialize the device driver instance identified by context.
* This function is required by drivers loaded by the device manager.
*
* \param    ctx		Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to de-init the device driver.
*/
static
driver_status_t uart_deinit (stream_driver_ctx_t* ctx)
{
	return DRIVER_STATUS_SUCCESS;
}

/**
* Initialize a device driver instance.
* This function is required by drivers loaded by the device manager.
*
* \param    init_parameters		Initialization parameters
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to init the device driver.
*/
static
driver_status_t uart_init (uint32_t init_parameters)
{
	stream_ctx.name = DRIVER_NAME;
	stream_ctx.ctx  = &uart_ctx;

	uart_ctx.base        = UART_BASE;
	uart_ctx.interrupt   = UART_INT;
	uart_ctx.srcClock_Hz = UART_CLKSRC;
	uart_ctx.params      = NULL;
	uart_ctx.mode		 = UARTMODE_RAW;
	uart_ctx.ref_count   = 0;

	uart_ctx.tx_buffer   = tx_buffer;
	uart_ctx.rx_buffer   = rx_buffer;
	uart_ctx.buffer_size = BUFFER_SIZE;

	/**
	* Initialize transmit and receive circular buffers
	*/
    buf_reset(uart_ctx.tx_buffer, uart_ctx.buffer_size);
    buf_reset(uart_ctx.rx_buffer, uart_ctx.buffer_size);

    /* Initialize UART2 pins */
    /* Enable the port clock */
//    CLOCK_EnableClock(kCLOCK_PortC);
    /* Affects PORTB_PCR16 register */
//    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
//    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    /*
    * Configure UART operational parameters
    */
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
*           DRIVER_FAILURE_GENERAL if unable perform the command.
*/
static
driver_status_t
uart_iocontrol
(stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	return uart_core_ioctl (ctx, code, input_buffer, input_size, output_buffer, output_size, bytes_read);
}

/**
* Open the device driver for read, writing, or both
*
* \param	name			Pointer to the device name
* \param    mode			Open mode
* \param    options 		Open options
*
* \returns  stream_driver_ctx_t pointer if successful.
*           NULL if unable to open the device driver.
*/
static
stream_driver_ctx_t* uart_open (char* name, uint32_t mode, uint32_t options)
{
	if ( strcmp(name,DRIVER_NAME) == 0 )
	{
		uart_ctx.mode   = mode;
		uart_ctx.params = (void*)options;

		driver_status_t result = uart_core_open (&stream_ctx, mode, options);
		if ( result == DRIVER_STATUS_SUCCESS )
			return &stream_ctx;
	}
	return NULL;
}

/**
* Power down the device driver instance identified by context.
* This function is optional.
*
* \param    ctx		Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to power down the device driver.
*/
static
driver_status_t uart_powerdown (stream_driver_ctx_t* ctx)
{
	return DRIVER_STATUS_SUCCESS;
}

/**
* Power up the device driver instance identified by context.
* This function is optional.
*
* \param    ctx		Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to power up the device driver.
*/
static
driver_status_t uart_powerup (stream_driver_ctx_t* ctx)
{
	return DRIVER_STATUS_SUCCESS;
}

/**
* Mark the closing instance as invalid and wake sleeping threads.
* This function is optional.
*
* \param    ctx		Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to pre close the device driver.
*/
static
driver_status_t uart_preclose (stream_driver_ctx_t* ctx)
{
	return DRIVER_STATUS_SUCCESS;
}

/**
* Mark the device driver instance as invalid and wake sleeping threads.
* This function is required if the preclose function is implemented.
*
* \param    ctx		Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to pre de-init the device driver.
*/
static
driver_status_t uart_predeinit (stream_driver_ctx_t* ctx)
{
	return DRIVER_STATUS_SUCCESS;
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
*           DRIVER_FAILURE_GENERAL if unable to close the device driver.
*/
static
driver_status_t uart_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	return uart_core_read (ctx, data_buffer, size, bytes_read );
}

/**
* Seek to a specific position or offset in the device driver instance.
*
* \param    ctx				Pointer to the device context
* \param    position		Byte offset to move to
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the device driver.
*           DRIVER_FAILURE_UNSUPPORTED_OPERATION if not implemented.
*/
static
driver_status_t uart_seek (stream_driver_ctx_t* ctx, uint32_t position)
{
	return DRIVER_FAILURE_UNSUPPORTED_OPERATION;
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
*           DRIVER_FAILURE_GENERAL if unable to close the device driver.
*/
static
driver_status_t uart_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	return uart_core_write ( ctx, data_buffer, size, bytes_written );
}

/**
* The UARTC stream interface driver vtable
*/
const stream_driver_vtable_t uartc_vtable =
{
		.getid 		= uart_getid,
		.getname 	= uart_getname,
		.runlevel 	= uart_runlevel,
		.close 		= uart_close,
		.deinit 	= uart_deinit,
		.init		= uart_init,
		.iocontrol	= uart_iocontrol,
		.open		= uart_open,
		.powerdown	= uart_powerdown,
		.powerup	= uart_powerup,
		.preclose	= uart_preclose,
		.predeinit	= uart_predeinit,
		.read		= uart_read,
		.seek 		= uart_seek,
		.write 		= uart_write
};
