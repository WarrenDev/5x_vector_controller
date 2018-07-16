
/**
* i2ca_driver_impl.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief I2CA driver implementation.
*/

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>

#include "fsl_port.h"
#include "i2c_driver_core.h"

#define DRIVER_NAME			(char*)"I2CA"
#define I2C_BASE			I2C0
#define I2C_CLOCK_NAME		kCLOCK_I2c0
#define DEFAULT_OPEN_PARMS	(char*)0

static device_driver_id_t	driver_id = DRV_I2C_A;
static device_runlevel_t    driver_runlevel = DRV_RUNLEVEL1;
static stream_driver_ctx_t	stream_ctx;
static i2c_driver_ctx_t		i2c_ctx;

/**
* Get the id tag of the device driver.
*
* \param    none
*
* \returns  id tag of the device driver
*/
static
device_driver_id_t i2c_getid (void)
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
char* i2c_getname (void)
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
device_runlevel_t i2c_runlevel (void)
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
driver_status_t i2c_close (stream_driver_ctx_t* ctx)
{
	return i2c_core_close (ctx);
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
driver_status_t i2c_deinit (stream_driver_ctx_t* ctx)
{
    /**
    *  Disable the port clock
    */
    CLOCK_DisableClock(kCLOCK_PortE);
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
driver_status_t i2c_init (uint32_t init_parameters)
{
	stream_ctx.name      = DRIVER_NAME;
	stream_ctx.ctx       = &i2c_ctx;
	stream_ctx.ref_count = 0;

	i2c_ctx.base       = I2C_BASE;
	i2c_ctx.clock_name = I2C_CLOCK_NAME;
	i2c_ctx.params 	   = NULL;

	/**
	* Declare and initialize for pull up configuration
	*/
    port_pin_config_t pinConfig = {0};
    pinConfig.pullSelect = kPORT_PullUp;
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */

    /**
    *  Enable the port clock
    */
    CLOCK_EnableClock(kCLOCK_PortE);

    /**
    * I2C0 pull up resistor setting
    */
    PORT_SetPinConfig(PORTE, 24U, &pinConfig);
    PORT_SetPinConfig(PORTE, 25U, &pinConfig);

    /**
    * I2C0 PIN_MUX Configuration
    */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt5);
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt5);

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
i2c_iocontrol
(stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	return i2c_core_ioctl (ctx, code, input_buffer, input_size, output_buffer, output_size, bytes_read);
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
stream_driver_ctx_t* i2c_open (char* name, uint32_t mode, uint32_t options)
{
	if ( strcmp(name,DRIVER_NAME) == 0 )
	{
		i2c_ctx.mode   = mode;
		i2c_ctx.params = (void*)options;

		driver_status_t result = i2c_core_open (&stream_ctx);
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
driver_status_t i2c_powerdown (stream_driver_ctx_t* ctx)
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
driver_status_t i2c_powerup (stream_driver_ctx_t* ctx)
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
driver_status_t i2c_preclose (stream_driver_ctx_t* ctx)
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
driver_status_t i2c_predeinit (stream_driver_ctx_t* ctx)
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
driver_status_t i2c_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	return i2c_core_read (ctx, data_buffer, size, bytes_read );
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
driver_status_t i2c_seek (stream_driver_ctx_t* ctx, uint32_t position)
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
driver_status_t i2c_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	return i2c_core_write ( ctx, data_buffer, size, bytes_written );
}

/**
* The II2CA stream interface driver vtable
*/
const stream_driver_vtable_t ii2ca_vtable =
{
		i2c_getid,
		i2c_getname,
		i2c_runlevel,
		i2c_close,
		i2c_deinit,
		i2c_init,
		i2c_iocontrol,
		i2c_open,
		i2c_powerdown,
		i2c_powerup,
		i2c_preclose,
		i2c_predeinit,
		i2c_read,
		i2c_seek,
		i2c_write
};

