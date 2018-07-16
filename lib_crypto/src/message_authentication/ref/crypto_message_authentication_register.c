
/**
* crypto_message_authentication_register.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto message authentication register implementation.
*/


#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/message_authentication.h>

extern const message_authentication_vtable_t message_authentication_256_vtable;
extern const message_authentication_vtable_t message_authentication_384_vtable;
extern const message_authentication_vtable_t message_authentication_512_vtable;

/**
* Register the message authentication 256 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t message_authentication_256_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_256);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_AUTHENTICATION_256, (vtable_ptr_t)&message_authentication_256_vtable));

} // message_authentication_256_register

/**
* Register the message_authentication 384 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t message_authentication_384_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_384);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_AUTHENTICATION_384, (vtable_ptr_t)&message_authentication_384_vtable));

} // message_authentication_384_register

/**
* Register the message authentication 512 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t message_authentication_512_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_AUTHENTICATION_512);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_AUTHENTICATION_512, (vtable_ptr_t)&message_authentication_512_vtable));

} // message_authentication_512_register

