
/**
* i2c_driver_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  I2C stream driver core implementation.
*
*/

#include "i2c_driver_core.h"

static
driver_status_t
i2c_core_ioctl_initialize (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size);

static
driver_status_t
i2c_core_ioctl_deinit (stream_driver_ctx_t* ctx);

static
driver_status_t
i2c_core_ioctl_enable (stream_driver_ctx_t* ctx);

static
driver_status_t
i2c_core_ioctl_disable (stream_driver_ctx_t* ctx);

static
driver_status_t
i2c_core_ioctl_flush (stream_driver_ctx_t* ctx);

static
driver_status_t
i2c_core_ioctl_global_reset (stream_driver_ctx_t* ctx);

/**
* Open the I2C device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the I2C.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
i2c_core_open (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( ctx->ref_count == 0 )
		{
			i2c_driver_ctx_t* i2c_drv_ctx = (i2c_driver_ctx_t*)ctx->ctx;
			i2c_core_ioctl_enable (ctx);
			i2c_core_ioctl_initialize (ctx, i2c_drv_ctx->params, sizeof(i2c_config_t) );
		}
		ctx->ref_count++;
		return ( DRIVER_STATUS_SUCCESS );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Close the I2C device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the I2C.
*/
driver_status_t
i2c_core_close (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( --ctx->ref_count == 0)
		{
			i2c_core_ioctl_disable ( ctx );
			i2c_core_ioctl_deinit ( ctx );
		}
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read data from the device driver instance.
*
* \param    ctx				Pointer to the device context
* \param    data_buffer		Pointer to a i2c_transfer_t data structure
* \param    size			Size of i2c_transfer_t data structure
* \param    bytes_read		Pointer to the actual number of bytes read
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to read from the device driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
driver_status_t
i2c_core_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	if ( (ctx != NULL) && (data_buffer != NULL) && (size >= sizeof(i2c_transfer_t)) )
	{
		i2c_driver_ctx_t* i2c_ctx = (i2c_driver_ctx_t*)ctx->ctx;
		i2c_transfer_t* transfer_control = (i2c_transfer_t*)data_buffer;

		/**
		* Read data bytes
		*/
		i2c_master_transfer_t transfer;
		transfer.direction      = kI2C_Read;
		transfer.slaveAddress   = transfer_control->address;
		transfer.subaddress     = transfer_control->reg;
		transfer.subaddressSize = 1;
		transfer.data           = transfer_control->data_buffer;
		transfer.dataSize       = transfer_control->size;
		transfer.flags          = transfer_control->flags;
		int32_t status = I2C_MasterTransferBlocking ( i2c_ctx->base, &transfer );
		if ( status == kStatus_Success )
		{
			if ( bytes_read )
				*bytes_read = transfer.dataSize;
			return DRIVER_STATUS_SUCCESS;
		}
		if ( bytes_read )
			*bytes_read = 0;
		return DRIVER_FAILURE_GENERAL;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Write data to the device driver instance.
*
* \param    ctx				Pointer to the device context
* \param    data_buffer		Pointer to a i2c_transfer_t data structure
* \param    size			Size of the i2c_transfer_t data structure
* \param    bytes_written	Pointer to the actual number of bytes written
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to write to the device driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
driver_status_t
i2c_core_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	if ( (ctx != NULL) && (data_buffer != NULL) && (size >= sizeof(i2c_transfer_t)) )
	{
		i2c_driver_ctx_t* i2c_ctx = (i2c_driver_ctx_t*)ctx->ctx;
		i2c_transfer_t* transfer_control = (i2c_transfer_t*)data_buffer;
		i2c_master_transfer_t transfer;

		/**
		* Write data
		*/
		transfer.direction      = kI2C_Write;
		transfer.slaveAddress   = transfer_control->address;
		transfer.subaddress     = transfer_control->reg;
		transfer.subaddressSize = 1;
		transfer.data           = transfer_control->data_buffer;
		transfer.dataSize       = transfer_control->size;
		transfer.flags          = transfer_control->flags;
		int32_t status = I2C_MasterTransferBlocking ( i2c_ctx->base, &transfer );
		if ( status == kStatus_Success )
		{
			if ( bytes_written )
				*bytes_written = transfer.dataSize;
			return DRIVER_STATUS_SUCCESS;
		}
		if ( bytes_written )
			*bytes_written = 0;
		return DRIVER_FAILURE_GENERAL;
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
i2c_core_ioctl (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	driver_status_t result;

	switch ( code )
	{
		case IOCTL_I2C_INITIALIZE:
			result = i2c_core_ioctl_initialize ( ctx, input_buffer, input_size );
			break;
		case IOCTL_I2C_DEINIT:
			result = i2c_core_ioctl_deinit ( ctx );
			break;
		case IOCTL_I2C_ENABLE:
			result = i2c_core_ioctl_enable ( ctx );
			break;
		case IOCTL_I2C_DISABLE:
			result = i2c_core_ioctl_disable ( ctx );
			break;
		case IOCTL_I2C_FLUSH:
			result = i2c_core_ioctl_flush (ctx);
			break;
		case IOCTL_I2C_GLOBAL_RESET:
			result = i2c_core_ioctl_global_reset (ctx);
			break;
	}
	return result;
}

/**
* Initialize the I2C device driver.
*
* \param    ctx				Pointer to the device context
* \param	input_buffer	Pointer to initialization data
* \param	input_size		Size of the initialization data
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
* 			DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are invalid
*/
driver_status_t
i2c_core_ioctl_initialize (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		i2c_driver_ctx_t* i2c_ctx = (i2c_driver_ctx_t*)ctx->ctx;
		i2c_config_t* config = (i2c_config_t*)input_buffer;
		i2c_master_config_t masterConfig;
		uint32_t sourceClock;

	    /*
	     * Default configuration
	     *  masterConfig.baudRate_Bps = 100000U;
	     *  masterConfig.enableHighDrive = false;
	     *  masterConfig.enableStopHold = false;
	     *  masterConfig.glitchFilterWidth = 0U;
	     *  masterConfig.enableMaster = true;
	     */
	    I2C_MasterGetDefaultConfig(&masterConfig);

	    if ( config != NULL && input_size == sizeof(i2c_config_t) )
	    {
		    masterConfig.baudRate_Bps = config->clockSpeed;
	    }

	    sourceClock = CLOCK_GetFreq(I2C0_CLK_SRC);

	    I2C_MasterInit(i2c_ctx->base, &masterConfig, sourceClock);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* De-initialize the I2C device driver.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
* 			DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are invalid
*/
driver_status_t
i2c_core_ioctl_deinit (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		i2c_driver_ctx_t* i2c_ctx = (i2c_driver_ctx_t*)ctx->ctx;
		I2C_MasterDeinit(i2c_ctx->base);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Enable the I2C device driver.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
* 			DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are invalid
*/
driver_status_t
i2c_core_ioctl_enable (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		i2c_driver_ctx_t* i2c_ctx = (i2c_driver_ctx_t*)ctx->ctx;

	    /**
	    *  Enable I2C clock.
	    */
	    CLOCK_EnableClock(i2c_ctx->clock_name);
	    /**
	    *  Enable I2C peripheral.
	    */
		I2C_Enable(i2c_ctx->base, true);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Disable the I2C device driver.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to perform the command.
* 			DRIVER_FAILURE_INVALID_PARAMETER if one or more parameters are invalid
*/
driver_status_t
i2c_core_ioctl_disable (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		i2c_driver_ctx_t* i2c_ctx = (i2c_driver_ctx_t*)ctx->ctx;

	    /**
	    *  Disable I2C peripheral.
	    */
		I2C_Enable(i2c_ctx->base, false);
	    /**
	    *  Disable I2C clock.
	    */
	    CLOCK_DisableClock(i2c_ctx->clock_name);
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
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
i2c_core_ioctl_flush (stream_driver_ctx_t* ctx)
{
	if ( ctx == NULL )
		return DRIVER_FAILURE_INVALID_PARAMETER;

	return DRIVER_STATUS_SUCCESS;
}

/**
* Send the I2C global reset message.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
* 			Error indicator on error;
*/
driver_status_t
i2c_core_ioctl_global_reset (stream_driver_ctx_t* ctx)
{
	uint8_t  global_reset = 0x06;
//	uint32_t global_address = 0L;
	uint32_t bytes_written;

//	i2c_core_ioctl (ctx, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, (void*)&global_address, sizeof(uint32_t), NULL, 0L, NULL);
	i2c_core_write (ctx, (void*)&global_reset, 1L, &bytes_written);
	i2c_core_ioctl_flush (ctx);
//	i2c_core_ioctl (ctx, IO_IOCTL_I2C_STOP, NULL, 0L, NULL, 0L, NULL);

	return DRIVER_STATUS_SUCCESS;
}

