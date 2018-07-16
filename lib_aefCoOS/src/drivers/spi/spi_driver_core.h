
/**
* spi_driver_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  SPI stream driver core definitions.
*
*/

#ifndef SRC_DRIVERS_SPI_SPI_DRIVER_CORE_H_
#define SRC_DRIVERS_SPI_SPI_DRIVER_CORE_H_

#include <aef/embedded/driver/spi/spi_driver.h>
#include <aef/embedded/driver/stream_driver.h>
#include <stddef.h>
#include <stdint.h>
#include <CoOS.h>
#include <fsl_dspi.h>


# ifdef   __cplusplus
extern "C" {
# endif

/**
* The SPI driver context is the context structure used by the
* SPI driver during operations.  This structure must be
* initialized by calling the appropriate initialization function prior to use.
*/
typedef struct spi_driver_ctx_def
{
	char* name;
	SPI_Type *base;
	uint32_t mode;
	void* params;
} spi_driver_ctx_t;

/**
* Open the SPI device
*
* \param	ctx			Pointer to a driver context
* \param	name		Pointer to the device name
* \param    mode		Open mode
* \param    options 	Open options
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the SPI.
*/
driver_status_t spi_core_open (stream_driver_ctx_t* ctx);

/**
* Close the SPI device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the SPI.
*/
driver_status_t spi_core_close (stream_driver_ctx_t* ctx);

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
*/
driver_status_t
spi_core_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read );

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
*/
driver_status_t
spi_core_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written);

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
driver_status_t
spi_core_ioctl (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_DRIVERS_SPI_SPI_DRIVER_CORE_H_ */
