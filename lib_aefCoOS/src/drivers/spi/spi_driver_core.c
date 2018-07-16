
/**
* spi_driver_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  SPI stream driver core implementation.
*
*/

#include "spi_driver_core.h"

static
driver_status_t
spi_core_ioctl_flush (stream_driver_ctx_t* ctx);

/**
* Open the SPI device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the SPI.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
spi_core_open (stream_driver_ctx_t* ctx)
{
	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return DRIVER_STATUS_SUCCESS;
}

/**
* Close the SPI device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the SPI.
*/
driver_status_t
spi_core_close (stream_driver_ctx_t* ctx)
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
* \param    size			Size of the data buffer
* \param    bytes_read		Pointer to the actual number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to read from the device driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
driver_status_t
spi_core_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	if ( (ctx == NULL) || (data_buffer == NULL) )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	*bytes_read = 0L;
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
spi_core_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	if ( (ctx == NULL) || (data_buffer == NULL) )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	*bytes_written = 0L;
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
spi_core_ioctl (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	driver_status_t result;

	switch ( code )
	{
		case IOCTL_SPI_FLUSH:
			result = spi_core_ioctl_flush (ctx);
			break;
	}
	return result;
}

/**
* De-assert chip select to the device.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
* 			Error indicator on error;
*/
driver_status_t
spi_core_ioctl_flush (stream_driver_ctx_t* ctx)
{
	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return DRIVER_STATUS_SUCCESS;
}


