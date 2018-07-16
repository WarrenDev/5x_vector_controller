
/**
* mpu9250_driver_impl.c
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MPU-9250 stream interface device driver implementation.
*/

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>

#include "mpu9250_driver_core.h"

#define DRIVER_NAME			(char*)"SENSOR_MPU9250"

static device_driver_id_t	driver_id = DRV_SENSOR_MPU9250;
static device_runlevel_t    driver_runlevel = DRV_RUNLEVEL2;
static stream_driver_ctx_t	stream_ctx;
static mpu9250_driver_ctx_t	mpu9250_ctx;

/**
* Get the id tag of the device driver.
*
* \param    none
*
* \returns  id tag of the device driver
*/
static
device_driver_id_t mpu9250_getid (void)
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
char* mpu9250_getname (void)
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
device_runlevel_t mpu9250_runlevel (void)
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
driver_status_t mpu9250_close (stream_driver_ctx_t* ctx)
{
	return mpu9250_core_close (ctx);
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
driver_status_t mpu9250_deinit (stream_driver_ctx_t* ctx)
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
driver_status_t mpu9250_init (uint32_t init_parameters)
{
	stream_ctx.name      = DRIVER_NAME;
	stream_ctx.ctx       = &mpu9250_ctx;
	stream_ctx.ref_count = 0;

	return mpu9250_core_init (&mpu9250_ctx);
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
mpu9250_iocontrol
(stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	return mpu9250_core_ioctl (ctx, code, input_buffer, input_size, output_buffer, output_size, bytes_read);
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
stream_driver_ctx_t* mpu9250_open (char* name, uint32_t mode, uint32_t options)
{
	if ( strcmp(name,DRIVER_NAME) == 0 )
	{
		mpu9250_ctx.name   = name;
		mpu9250_ctx.mode   = mode;
		mpu9250_ctx.params = (void*)options;

		driver_status_t result = mpu9250_core_open (&stream_ctx);
		return ( result == DRIVER_STATUS_SUCCESS ) ? &stream_ctx : NULL;
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
driver_status_t mpu9250_powerdown (stream_driver_ctx_t* ctx)
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
driver_status_t mpu9250_powerup (stream_driver_ctx_t* ctx)
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
driver_status_t mpu9250_preclose (stream_driver_ctx_t* ctx)
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
driver_status_t mpu9250_predeinit (stream_driver_ctx_t* ctx)
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
driver_status_t mpu9250_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	return mpu9250_core_read (ctx, data_buffer, size, bytes_read );
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
driver_status_t mpu9250_seek (stream_driver_ctx_t* ctx, uint32_t position)
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
driver_status_t mpu9250_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	return mpu9250_core_write ( ctx, data_buffer, size, bytes_written );
}

/**
* The MPU-9250 stream interface driver vtable
*/
const stream_driver_vtable_t mpu9250_drv_vtable =
{
	.getid 		= mpu9250_getid,
	.getname 	= mpu9250_getname,
	.runlevel 	= mpu9250_runlevel,
	.close 		= mpu9250_close,
	.deinit 	= mpu9250_deinit,
	.init 		= mpu9250_init,
	.iocontrol 	= mpu9250_iocontrol,
	.open 		= mpu9250_open,
	.powerdown 	= mpu9250_powerdown,
	.powerup 	= mpu9250_powerup,
	.preclose 	= mpu9250_preclose,
	.predeinit 	= mpu9250_predeinit,
	.read 		= mpu9250_read,
	.seek 		= mpu9250_seek,
	.write 		= mpu9250_write
};

