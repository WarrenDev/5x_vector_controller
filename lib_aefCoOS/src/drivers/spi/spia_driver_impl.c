
/**
* spia_driver_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief SPIA driver implementation.
*/

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>

#include "spi_driver_core.h"

#define DRIVER_NAME			(char*)"SPIA"
#define SPI_BASE			SPI0
#define DEFAULT_OPEN_PARMS	(char*)0

static device_driver_id_t	driver_id = DRV_SPI_A;
static device_runlevel_t    driver_runlevel = DRV_RUNLEVEL1;
static stream_driver_ctx_t	stream_ctx;
static spi_driver_ctx_t		spi_ctx;

/**
* Get the id tag of the device driver.
*
* \param    none
*
* \returns  id tag of the device driver
*/
static
device_driver_id_t spi_getid (void)
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
char* spi_getname (void)
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
device_runlevel_t spi_runlevel (void)
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
driver_status_t spi_close (stream_driver_ctx_t* ctx)
{
	return spi_core_close (ctx);
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
driver_status_t spi_deinit (stream_driver_ctx_t* ctx)
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
driver_status_t spi_init (uint32_t init_parameters)
{
	stream_ctx.name = DRIVER_NAME;
	stream_ctx.ctx  = &spi_ctx;

	spi_ctx.base   = SPI_BASE;
	spi_ctx.params = NULL;

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
spi_iocontrol
(stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	return spi_core_ioctl (ctx, code, input_buffer, input_size, output_buffer, output_size, bytes_read);
}

/**
* Open the device driver for read, writing, or both
*
* \param	name			Pointer to the device name
* \param    mode			Open mode
* \param    options 		Open options - chip select (1 - 4)
*
* \returns  stream_driver_ctx_t pointer if successful.
*           NULL if unable to open the device driver.
*/
static
stream_driver_ctx_t* spi_open (char* name, uint32_t mode, uint32_t options)
{
	if ( strcmp(name,DRIVER_NAME) )
		return NULL;

	spi_ctx.mode   = mode;
	spi_ctx.params = (void*)options;

	driver_status_t result = spi_core_open (&stream_ctx);
	if ( result != DRIVER_STATUS_SUCCESS )
		return NULL;

	return &stream_ctx;
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
driver_status_t spi_powerdown (stream_driver_ctx_t* ctx)
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
driver_status_t spi_powerup (stream_driver_ctx_t* ctx)
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
driver_status_t spi_preclose (stream_driver_ctx_t* ctx)
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
driver_status_t spi_predeinit (stream_driver_ctx_t* ctx)
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
driver_status_t spi_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	return spi_core_read (ctx, data_buffer, size, bytes_read );
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
driver_status_t spi_seek (stream_driver_ctx_t* ctx, uint32_t position)
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
driver_status_t spi_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	return spi_core_write ( ctx, data_buffer, size, bytes_written );
}

/**
* The SPIA stream interface driver vtable
*/
const stream_driver_vtable_t spia_vtable =
{
		spi_getid,
		spi_getname,
		spi_runlevel,
		spi_close,
		spi_deinit,
		spi_init,
		spi_iocontrol,
		spi_open,
		spi_powerdown,
		spi_powerup,
		spi_preclose,
		spi_predeinit,
		spi_read,
		spi_seek,
		spi_write
};
