
/**
* stream_driver.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Stream device driver interface definition.
*
*/
#ifndef  AEF_STREAM_DRIVER_HEADER_GUARD
# define AEF_STREAM_DRIVER_HEADER_GUARD

#include <aef/embedded/driver/device_driver_id.h>
#include <aef/embedded/driver/device_driver_status.h>
#include <aef/embedded/driver/device_runlevel.h>
#include <stdint.h>


/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

#define	MAX_STREAM_DRIVER_NAME_SIZE		8

    /**
    * The stream driver context is the context structure used by the
    * stream driver during operations.  This structure must be
    * initialized by calling the appropriate initialization function prior to use.
    */
	typedef struct stream_driver_ctx_def
	{
		char* name;
		void* ctx;
		uint16_t ref_count;
	} stream_driver_ctx_t;

    /**
    * The stream driver vtable type, which is used for implementing device drivers
    */
    typedef struct stream_driver_vtable
    {

        /**
        * Get the id tag of the device driver.
        *
        * \param    none
        *
        * \returns  id tag of the device driver
        */
    	device_driver_id_t (*getid) (void);

        /**
        * Get the name tag of the device driver.
        *
        * \param    none
        *
        * \returns  Pointer to the device driver name tag
        */
        char* (*getname) (void);

        /**
        * Get the run level of the device driver.  The run level determines the
        * system load order.
        *
        * \param    none
        *
        * \returns  Run level of the device drover
        */
        device_runlevel_t (*runlevel) (void);

        /**
        * Close the device driver instance identified by context.
        *
        * \param    ctx		Pointer to the device context
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to close the device driver.
        */
        driver_status_t (*close) (stream_driver_ctx_t* ctx);

        /**
        * De-initialize the device driver instance identified by context.
        * This function is required by drivers loaded by the device manager.
        *
        * \param    ctx		Pointer to the device context
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to de-init the device driver.
        */
        driver_status_t (*deinit) (stream_driver_ctx_t* ctx);

        /**
        * Initialize a device driver instance.
        * This function is required by drivers loaded by the device manager.
        *
        * \param    init_parameters		Initialization parameters
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to init the device driver.
        */
        driver_status_t (*init) (uint32_t init_parameters);

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
        driver_status_t (*iocontrol) 
            (stream_driver_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_read);

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
        stream_driver_ctx_t* (*open) (char* name, uint32_t mode, uint32_t options);

        /**
        * Power down the device driver instance identified by context.
        * This function is optional.
        *
        * \param    ctx		Pointer to the device context
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to power down the device driver.
        */
        driver_status_t (*powerdown) (stream_driver_ctx_t* ctx);

        /**
        * Power up the device driver instance identified by context.
        * This function is optional.
        *
        * \param    ctx		Pointer to the device context
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to power up the device driver.
        */
        driver_status_t (*powerup) (stream_driver_ctx_t* ctx);

        /**
        * Mark the closing instance as invalid and wake sleeping threads.
        * This function is optional.
        *
        * \param    ctx		Pointer to the device context
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to pre close the device driver.
        */
        driver_status_t (*preclose) (stream_driver_ctx_t* ctx);

        /**
        * Mark the device driver instance as invalid and wake sleeping threads.
        * This function is required if the preclose function is implemented.
        *
        * \param    ctx		Pointer to the device context
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to pre de-init the device driver.
        */
        driver_status_t (*predeinit) (stream_driver_ctx_t* ctx);

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
        driver_status_t (*read) (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_read );

        /**
        * Seek to a specific position or offset in the device driver instance.
        *
        * \param    ctx				Pointer to the device context
        * \param    position		Byte offset to move to
        *
        * \returns  DRIVER_STATUS_SUCCESS if successful.
        *           DRIVER_FAILURE_GENERAL if unable to seek to position on the device driver.
        */
        driver_status_t (*seek) (stream_driver_ctx_t* ctx, uint32_t position);

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
        driver_status_t (*write) (stream_driver_ctx_t* ctx, void* data_buffer, uint32_t size, uint32_t* bytes_written);

    } stream_driver_vtable_t;

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_STREAM_DRIVER_HEADER_GUARD
