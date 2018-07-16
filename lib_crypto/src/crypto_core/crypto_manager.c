
/**
* crypto_manager.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto manager implementation.
*/

#include <aef/crypto/crypto_manager.h>
#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

/**
* Initialize the crypto sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if initialization was successful.
*
*          CRYPTO_FAILURE_GENERAL if initialization failed.
*/
crypto_status_t  crypto_manager_init ( void )
{

	/**
	* Create the crypto registry
	*/
	return crypto_registry_create();

} // crypto_manager_init

/**
* Destroy the crypto sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if destruction was successful.
*
*          CRYPTO_FAILURE_GENERAL if destruction failed.
*/
crypto_status_t  crypto_manager_deinit(void)
{

	/**
	* Destroy the crypto registry
	*/
	return crypto_registry_destroy();

} // crypto_manager_deinit
