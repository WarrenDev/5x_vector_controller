
/**
* ll_crypto_manager.c
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
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/ll_crypto_manager.h>
#include "stddef.h"

/**
* Initialize the crypto sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if initialization was successful.
*
*          CRYPTO_FAILURE_GENERAL if initialization failed.
*/
crypto_status_t  ll_crypto_init(void)
{

	return crypto_manager_init();

} // crypto_manager_init

/**
* De-initialize the crypto sub-system.
*
* \returns CRYPTO_STATUS_SUCCESS if destruction was successful.
*
*          CRYPTO_FAILURE_GENERAL if destruction failed.
*/
crypto_status_t  ll_crypto_deinit(void)
{

	return crypto_manager_deinit();

} // crypto_manager_deinit
