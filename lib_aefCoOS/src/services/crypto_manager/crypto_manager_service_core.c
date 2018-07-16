
/**
* crypto_manager_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Logger service core implementation.
*
*/

#include "crypto_manager_service_core.h"

#include <aef/crypto/ll_crypto_block_cipher.h>
#include <aef/crypto/ll_crypto_digital_signature.h>
#include <aef/crypto/ll_crypto_key_agreement.h>
#include <aef/crypto/ll_crypto_manager.h>
#include <aef/crypto/ll_crypto_memcmp.h>
#include <aef/crypto/ll_crypto_message_authentication.h>
#include <aef/crypto/ll_crypto_message_digest.h>
#include <aef/crypto/ll_crypto_prng.h>
#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>

/**
* Internal routines.
*/
static service_status_t crypto_manager_core_start (service_ctx_t* ctx);
static service_status_t crypto_manager_core_restart (service_ctx_t* ctx);
static service_status_t crypto_manager_core_stop (service_ctx_t* ctx);
static service_status_t crypto_manager_core_status (service_ctx_t* ctx);
static service_status_t crypto_manager_core_pause (service_ctx_t* ctx);
static service_status_t crypto_manager_core_continue (service_ctx_t* ctx);

static service_status_t crypto_manager_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t size);
static service_status_t crypto_manager_core_deinitialize (service_ctx_t* ctx, void* input_buffer, uint32_t size);
static service_status_t crypto_core_prng_id (service_ctx_t* ctx, void* input_buffer, uint32_t size);
static service_status_t crypto_core_prng_init (service_ctx_t* ctx, void* input_buffer, uint32_t size);
static service_status_t crypto_core_prng_reseed (service_ctx_t* ctx, void* input_buffer, uint32_t size);
static service_status_t crypto_core_prng_read (service_ctx_t* ctx, void* input_buffer, uint32_t size);
static service_status_t crypto_core_prng_selftest (service_ctx_t* ctx, void* input_buffer, uint32_t size);

/**
* Initialize the crypto_manager service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_init (service_ctx_t* ctx)
{
	if ( ll_crypto_init() == CRYPTO_STATUS_SUCCESS )
	{
	//	ll_crypto_register_block_cipher_aes_128_cbc ();
	//	ll_crypto_register_digital_signature_ed25519 ();
	//	ll_crypto_register_digital_signature_ed25519_donna ();
	//	ll_crypto_register_key_agreement_curve25519 ();
	//	ll_crypto_register_message_authentication_256 ();
	//	ll_crypto_register_message_authentication_512 ();
	//	ll_crypto_register_sha256 ();
	//	ll_crypto_register_sha512 ();
		ll_crypto_register_mk64_prng ();
		ctx->state = SERVICE_START_PENDING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* De-initialize the crypto_manager service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_deinit (service_ctx_t* ctx)
{
	return ll_crypto_deinit ();
}

/**
* Send a command to a system service instance.
*
* \param    ctx				Pointer to the service context
* \param    code			I/O control code to perform
* \param    input_buffer	Pointer to the input buffer
* \param    input_size		Input buffer size
* \param    output_buffer	Pointer to the output buffer
* \param    output_size		Output buffer size
* \param    bytes_read		Pointer to the actual bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
service_status_t
crypto_manager_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( ctx->state == SERVICE_UNINITIALIZED )
		return SERVICE_FAILURE_OFFLINE;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = crypto_manager_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = crypto_manager_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = crypto_manager_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = crypto_manager_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = crypto_manager_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = crypto_manager_core_continue(ctx);
			break;
		case IOCTL_CRYPTO_INITIALIZE:
			status = crypto_manager_core_initialize(ctx, input_buffer, input_size);
			break;
		case IOCTL_CRYPTO_DEINITIALIZE:
			status = crypto_manager_core_deinitialize (ctx, input_buffer, input_size);
			break;
		case IOCTL_CRYPTO_PRNG_ID:
			status = crypto_core_prng_id (ctx, input_buffer, input_size);
			break;
		case IOCTL_CRYPTO_PRNG_INIT:
			status = crypto_core_prng_init (ctx, input_buffer, input_size);
			break;
		case IOCTL_CRYPTO_PRNG_RESEED:
			status = crypto_core_prng_reseed (ctx, input_buffer, input_size);
			break;
		case IOCTL_CRYPTO_PRNG_READ:
			status = crypto_core_prng_read (ctx, input_buffer, input_size);
			break;
		case IOCTL_CRYPTO_PRNG_SELFTEST:
			status = crypto_core_prng_selftest (ctx, input_buffer, input_size);
			break;
		default:
			break;
	}

	return status;
}

/**
* Start service control function.
* Starts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_start (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state != SERVICE_UNINITIALIZED )
	{
		ctx->state = SERVICE_RUNNING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Restart service control function.
* Restarts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_restart (service_ctx_t* ctx)
{
	return SERVICE_FAILURE_GENERAL;
}

/**
* Stop service control function.
* Stops the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		ctx->state = SERVICE_STOPPED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Status service control function.
* Retrieves status of the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_status (service_ctx_t* ctx)
{
	return SERVICE_FAILURE_GENERAL;
}

/**
* Pause service control function.
* Pauses the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_pause (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		ctx->state = SERVICE_PAUSED;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Continue service control function.
* Continues the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t crypto_manager_core_continue (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_PAUSED )
	{
		ctx->state = SERVICE_RUNNING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the hardware crypto_manager.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to init information
* \param	size			Size of init information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
crypto_manager_core_initialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* De-initialize the hardware crypto_manager.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to de-init information
* \param	size			Size of de-init information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
crypto_manager_core_deinitialize (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Get the PRNG driver ID.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to the parameter information
* \param	size			Size of the parameter information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t crypto_core_prng_id (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( input_buffer != NULL && input_size == sizeof(prng_id_parms_t) )
		{
			prng_id_parms_t* parms = (prng_id_parms_t*)input_buffer;
			parms->id = ll_crypto_prng_get_id ();
			return SERVICE_STATUS_SUCCESS;
		}
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the PRNG driver.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to the parameter information
* \param	size			Size of the parameter information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t crypto_core_prng_init (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( input_buffer != NULL && input_size == sizeof(prng_init_parms_t) )
		{
			prng_init_parms_t* parms = (prng_init_parms_t*)input_buffer;
			crypto_status_t result =
			ll_crypto_prng_init ( parms->entropy, parms->entropySize, parms->personal, parms->personalSize, parms->ctx, parms->ctx_size );
			if ( result == CRYPTO_STATUS_SUCCESS )
			{
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_INVALID_PARAMETER;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* Reseed the PRNG.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to the parameter information
* \param	size			Size of the parameter information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t crypto_core_prng_reseed (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( input_buffer != NULL && input_size == sizeof(prng_reseed_parms_t) )
		{
			prng_reseed_parms_t* parms = (prng_reseed_parms_t*)input_buffer;
			crypto_status_t result =
			ll_crypto_prng_reseed ( parms->ctx, parms->entropy, parms->entropySize );
			if ( result == CRYPTO_STATUS_SUCCESS )
			{
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_INVALID_PARAMETER;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* Read the PRNG driver.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to the parameter information
* \param	size			Size of the parameter information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t crypto_core_prng_read (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( input_buffer != NULL && input_size == sizeof(prng_read_parms_t) )
		{
			prng_read_parms_t* parms = (prng_read_parms_t*)input_buffer;
			crypto_status_t result =
			ll_crypto_prng_read ( parms->ctx, parms->buffer, parms->size );
			if ( result == CRYPTO_STATUS_SUCCESS )
			{
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_INVALID_PARAMETER;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* Perform a self-test of the PRNG driver.
*
* \param    ctx				Pointer to the service context
* \param    input_buffer	Pointer to the parameter information
* \param	size			Size of the parameter information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t crypto_core_prng_selftest (service_ctx_t* ctx, void* input_buffer, uint32_t input_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( input_buffer != NULL && input_size == sizeof(prng_selftest_parms_t) )
		{
			prng_selftest_parms_t* parms = (prng_selftest_parms_t*)input_buffer;
			crypto_status_t result =
			ll_crypto_prng_selftest ( parms->ctx, parms->scratch, parms->scratch_size );
			if ( result == CRYPTO_STATUS_SUCCESS )
			{
				return SERVICE_STATUS_SUCCESS;
			}
			return SERVICE_FAILURE_GENERAL;
		}
		return SERVICE_FAILURE_INVALID_PARAMETER;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}
