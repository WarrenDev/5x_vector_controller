
/**
* crypto_sha2_register.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto SHA2 message digest register implementation.
*/


#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>
#include <aef/crypto/message_digest.h>

extern const message_digest_vtable_t sha2_224_vtable;
extern const message_digest_vtable_t sha2_256_vtable;
extern const message_digest_vtable_t sha2_384_vtable;
extern const message_digest_vtable_t sha2_512_vtable;

/**
* Register the SHA2 224 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t sha2_224_register(void)
{
	
	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_224);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_DIGEST_SHA2_224, (vtable_ptr_t)&sha2_224_vtable));

} // sha2_224_register

/**
* Register the SHA2 256 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t sha2_256_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_256);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_DIGEST_SHA2_256, (vtable_ptr_t)&sha2_256_vtable));

} // sha2_256_register

/**
* Register the SHA2 384 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t sha2_384_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_384);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_DIGEST_SHA2_384, (vtable_ptr_t)&sha2_384_vtable));

} // sha2_384_register

/**
* Register the SHA2 512 crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t sha2_512_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(MESSAGE_DIGEST_SHA2_512);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(MESSAGE_DIGEST_SHA2_512, (vtable_ptr_t)&sha2_512_vtable));

} // sha2_512_register

