
/**
* mpu9250_driver_core.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  MPU9250 TFT LCD stream driver core implementation.
*
*/

#include <stdlib.h>
#include "mpu9250_driver_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/osal/time_delay.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines
*   Routines used only within this code module
*/
static
driver_status_t
mpu9250_core_initialize (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size);

static
driver_status_t
mpu9250_core_deinit (stream_driver_ctx_t* ctx);

static
driver_status_t
mpu9250_core_reset (stream_driver_ctx_t* ctx);

static
driver_status_t
mpu9250_core_read_id (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static
driver_status_t
mpu9250_core_read_id2 (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static
driver_status_t
mpu9250_core_read_accel (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static
driver_status_t
mpu9250_core_read_gyro (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static
driver_status_t
mpu9250_core_read_mag (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static
driver_status_t
mpu9250_core_read_temp (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

static const stream_driver_vtable_t* i2c_drv = NULL;
static stream_driver_ctx_t* i2c_ctx = NULL;

#define MPU9250_NAME		"DEV_MPU9250"
#define MPU9250_TRANSPORT	DRV_I2C_A

/**
* Initialize the MPU-9250 device driver..
*
* \param    ctx				Pointer to the driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the MPU9250 device.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t mpu9250_core_init (mpu9250_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		device_manager_vtable_t* device_manager = system_get_device_manager();
		i2c_drv = device_manager->getdevice(MPU9250_TRANSPORT);
		return ( i2c_drv != NULL ) ? DRIVER_STATUS_SUCCESS : DRIVER_FAILURE_INITIALIZATION;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Open the MPU-9250 device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to open the MPU9250 device.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
mpu9250_core_open (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		if ( i2c_drv != NULL )
		{
			i2c_ctx = i2c_drv->open (i2c_drv->getname(), 0, 0 );
			return ( i2c_ctx != NULL ) ? DRIVER_STATUS_SUCCESS : DRIVER_FAILURE_GENERAL;
		}
		return DRIVER_FAILURE_GENERAL;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Close the MPU-9250 device
*
* \param	ctx			Pointer to a driver context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unable to close the driver.
*           DRIVER_FAILURE_INVALID_PARAMETER if the context is invalid.
*/
driver_status_t
mpu9250_core_close (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		return i2c_drv->close ( ctx );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
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
mpu9250_core_read (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read )
{
	if ( (ctx != NULL) && (data_buffer != NULL) )
	{
		return i2c_drv->read ( i2c_ctx, data_buffer, size, bytes_read );
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
mpu9250_core_write (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written)
{
	if ( (ctx != NULL) && (data_buffer != NULL) )
	{
		return i2c_drv->write ( i2c_ctx, data_buffer, size, bytes_written );
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
mpu9250_core_ioctl (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	driver_status_t status = DRIVER_FAILURE_GENERAL;

	if ( ctx != NULL )
	{
		if ( bytes_read != NULL )
			*bytes_read = 0;

		switch ( code )
		{
			case IOCTL_MPU9250_INITIALIZE:
				status = mpu9250_core_initialize(ctx, input_buffer, input_size);
				break;
			case IOCTL_MPU9250_DEINIT:
				status = mpu9250_core_deinit (ctx);
				break;
			case IOCTL_MPU9250_RESET:
				status = mpu9250_core_reset (ctx);
				break;
			case IOCTL_MPU9250_READ_ID:
				status = mpu9250_core_read_id (ctx, output_buffer, output_size, bytes_read);
				break;
			case IOCTL_MPU9250_READ_ID2:
				status = mpu9250_core_read_id2 (ctx, output_buffer, output_size, bytes_read);
				break;
			case IOCTL_MPU9250_READ_ACCEL:
				status = mpu9250_core_read_accel (ctx, output_buffer, output_size, bytes_read);
				break;
			case IOCTL_MPU9250_READ_GYRO:
				status = mpu9250_core_read_gyro (ctx, output_buffer, output_size, bytes_read);
				break;
			case IOCTL_MPU9250_READ_MAG:
				status = mpu9250_core_read_mag (ctx, output_buffer, output_size, bytes_read);
				break;
			case IOCTL_MPU9250_READ_TEMP:
				status = mpu9250_core_read_temp (ctx, output_buffer, output_size, bytes_read);
				break;
		}
		return status;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Initialize the MPU-9250 interface.
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_initialize (stream_driver_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL )
	{
		uint32_t bytes_written;
		uint8_t value = 0x02;
		i2c_transfer_t transfer_control;

		/*
		 * Enable pass-through to allow communications with the internal AK8963
		 */
		transfer_control.address     = MPU9250_ADDRESS;
		transfer_control.reg         = INT_PIN_CFG;
		transfer_control.data_buffer = &value;
		transfer_control.size        = 1;
		mpu9250_core_write ( ctx, &transfer_control, sizeof(i2c_transfer_t), &bytes_written );
		return DRIVER_STATUS_SUCCESS;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* De-initialize the device
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_deinit (stream_driver_ctx_t* ctx)
{
	return DRIVER_STATUS_SUCCESS;
}

/**
* Reset the MPU-9250 device
*
* \param    ctx				Pointer to the device context
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_reset (stream_driver_ctx_t* ctx)
{
	if ( ctx != NULL )
	{
		uint32_t bytes_written = 0L;
		uint8_t value = 0x80;
		i2c_transfer_t transfer_control =
		{
			.address     = MPU9250_ADDRESS,
			.reg         = PWR_MGMT_1,
			.data_buffer = &value,
			.size        = 1,
			.flags       = kI2C_TransferDefaultFlag,
		};
		return mpu9250_core_write ( ctx, &transfer_control, sizeof(i2c_transfer_t), &bytes_written );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read the MPU-9250 device id (who am I)
*
* \param    ctx				Pointer to the device context
* \param	output_buffer	Pointer to the output buffer
* \param	output_size		Size of the output buffer
* \param	byte_read		Number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_read_id (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= 1 )
	{
		i2c_transfer_t transfer_control =
		{
			.address     = MPU9250_ADDRESS,
			.reg         = WHO_AM_I_MPU9250,
			.data_buffer = output_buffer,
			.size        = 1,
			.flags       = kI2C_TransferDefaultFlag,
		};
		return mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), bytes_read );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read the MPU-9250 magnetometer id (AK8963 who am I)
*
* \param    ctx				Pointer to the device context
* \param	output_buffer	Pointer to the output buffer
* \param	output_size		Size of the output buffer
* \param	byte_read		Number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_read_id2 (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= 1 )
	{
		uint8_t value = 0x02;
		i2c_transfer_t transfer_control;
		transfer_control.address     = MPU9250_ADDRESS;
		transfer_control.reg         = INT_PIN_CFG;
		transfer_control.data_buffer = &value;
		transfer_control.size        = 1;
		transfer_control.flags       = kI2C_TransferDefaultFlag;
		mpu9250_core_write ( ctx, &transfer_control, sizeof(i2c_transfer_t), bytes_read );

		transfer_control.address     = AK8963_ADDRESS;
		transfer_control.reg         = AK8963_WHO_AM_I;
		transfer_control.data_buffer = output_buffer;
		transfer_control.size        = 1;
		transfer_control.flags       = kI2C_TransferDefaultFlag;
		return mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), bytes_read );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read MPU-9250 accelerometer data
*
* \param    ctx				Pointer to the device context
* \param	output_buffer	Pointer to the output buffer
* \param	output_size		Size of the output buffer
* \param	byte_read		Number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_read_accel (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= 6 )
	{
		i2c_transfer_t transfer_control =
		{
			.address     = MPU9250_ADDRESS,
			.reg         = ACCEL_XOUT_H,
			.data_buffer = output_buffer,
			.size        = 6,
			.flags       = kI2C_TransferDefaultFlag,
		};
		return mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), bytes_read );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read MPU-9250 gyroscope data
*
* \param    ctx				Pointer to the device context
* \param	output_buffer	Pointer to the output buffer
* \param	output_size		Size of the output buffer
* \param	byte_read		Number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_read_gyro (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= 6 )
	{
		i2c_transfer_t transfer_control =
		{
			.address     = MPU9250_ADDRESS,
			.reg         = GYRO_XOUT_H,
			.data_buffer = output_buffer,
			.size        = 6,
			.flags       = kI2C_TransferDefaultFlag,
		};
		return mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), bytes_read );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read MPU-9250 magnetometer data
*
* \param    ctx				Pointer to the device context
* \param	output_buffer	Pointer to the output buffer
* \param	output_size		Size of the output buffer
* \param	byte_read		Number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_read_mag (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= 6 )
	{
		uint8_t status = 0x00;
		uint32_t bytes_returned;
		i2c_transfer_t transfer_control =
		{
			.address     = AK8963_ADDRESS,
			.reg         = AK8963_ST1,
			.data_buffer = &status,
			.size        = 1,
			.flags       = kI2C_TransferDefaultFlag,
		};
		mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), &bytes_returned );
		if ( status & 0x01 )
		{
			uint8_t rawData[7];
			transfer_control.reg = AK8963_XOUT_L;
			transfer_control.data_buffer = rawData;
			transfer_control.size = 7;
			mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), &bytes_returned );
			if ( !(rawData[6] & 0x08) )
			{
				if ( bytes_read != NULL )
					*bytes_read = 6;
				memcpy ( output_buffer, rawData, 6 );
				return DRIVER_STATUS_SUCCESS;
			}
			if ( bytes_read != NULL )
				*bytes_read = 0;
		}
		return DRIVER_FAILURE_GENERAL;
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

/**
* Read MPU-9250 temperature
*
* \param    ctx				Pointer to the device context
* \param	output_buffer	Pointer to the output buffer
* \param	output_size		Size of the output buffer
* \param	byte_read		Number of bytes returned
*
* \returns  DRIVER_STATUS_SUCCESS if successful.
*           DRIVER_FAILURE_GENERAL if unsuccessful.
*           DRIVER_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
driver_status_t
mpu9250_core_read_temp (stream_driver_ctx_t* ctx, void* output_buffer, uint32_t output_size, uint32_t* bytes_read)
{
	if ( ctx != NULL && output_buffer != NULL && output_size >= 2 )
	{
		i2c_transfer_t transfer_control =
		{
			.address     = MPU9250_ADDRESS,
			.reg         = TEMP_OUT_H,
			.data_buffer = output_buffer,
			.size        = 2,
		};
		return mpu9250_core_read ( ctx, &transfer_control, sizeof(i2c_transfer_t), bytes_read );
	}
	return DRIVER_FAILURE_INVALID_PARAMETER;
}

