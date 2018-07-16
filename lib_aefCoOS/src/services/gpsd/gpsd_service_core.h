
/**
* gpsd_service_core.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  GPSD service core definitions.
*
*/

#ifndef SRC_SERVICES_GPSD_GPSD_SERVICE_CORE_H_
#define SRC_SERVICES_GPSD_GPSD_SERVICE_CORE_H_

#include <aef/embedded/service/gpsd/gpsd_service.h>
#include <aef/embedded/service/service_interface.h>
#include <aef/embedded/service/service_ioctl.h>
#include <stdint.h>
#include "CoOS.h"


# ifdef   __cplusplus
extern "C" {
# endif

/**
* Initialize the GPSD service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_init (service_ctx_t* ctx);

/**
* De-initialize the GPSD service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t gpsd_core_deinit (service_ctx_t* ctx);

/**
* Send a command to a system GPSD service instance.
*
* \param    ctx					Pointer to the service context
* \param    code				I/O control code to perform
* \param    input_buffer		Pointer to the input buffer
* \param    input_size			Input buffer size
* \param    output_buffer		Pointer to the output buffer
* \param    output_size			Output buffer size
* \param    bytes_transferred	Pointer to the number of bytes read or written
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
service_status_t
gpsd_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICES_GPSD_GPSD_SERVICE_CORE_H_ */
