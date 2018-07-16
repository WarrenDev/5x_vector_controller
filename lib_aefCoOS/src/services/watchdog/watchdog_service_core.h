
/**
* watchdog_service_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Watchdog service core definitions.
*
*/

#ifndef SRC_SERVICES_WATCHDOG_WATCHDOG_SERVICE_CORE_H_
#define SRC_SERVICES_WATCHDOG_WATCHDOG_SERVICE_CORE_H_

#include <aef/embedded/service/service_interface.h>
#include <aef/embedded/service/service_ioctl.h>
#include <aef/embedded/service/watchdog/watchdog_service.h>
#include <stdint.h>
#include <string.h>
#include "CoOS.h"
#include "string.h"

#define HIWORD(w)				((uint16_t)(((uint32_t)(w) >> 16) & 0xFFFF))
#define LOWORD(w)				((uint16_t)(w))

/**
* Watchdog timer register write values
*/
#define	WD_UNLOCK_1				0xC520
#define WD_UNLOCK_2				0xD928
#define WD_REFRESH_1			0xA602
#define WD_REFRESH_2			0xB480

#define WD_TIMEOUT_DEFAULT		0x4B4C4B4C

#define WD_MAX_INSTANCES		20

#define INSTANCE_REFRESH		0x01
#define INSTANCE_DISABLED		0x80

# ifdef   __cplusplus
extern "C" {
# endif

/**
* Initialize the watchdog service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t watchdog_core_init (service_ctx_t* ctx);

/**
* De-initialize the watchdog service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t watchdog_core_deinit (service_ctx_t* ctx);

/**
* Send a command to a system service instance.
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
watchdog_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICES_WATCHDOG_WATCHDOG_SERVICE_CORE_H_ */
