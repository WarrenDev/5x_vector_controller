
/**
* crypto_aes_register.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto AES block cipher register implementation.
*/


#include <aef/crypto/block_cipher.h>
#include <aef/crypto/crypto_registry.h>
#include <aef/crypto/crypto_status.h>

extern const block_cipher_vtable_t aes_128_ecb_vtable;
extern const block_cipher_vtable_t aes_192_ecb_vtable;
extern const block_cipher_vtable_t aes_256_ecb_vtable;
extern const block_cipher_vtable_t aes_128_cbc_vtable;
extern const block_cipher_vtable_t aes_192_cbc_vtable;
extern const block_cipher_vtable_t aes_256_cbc_vtable;

/**
* Register the AES 128 ECB crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t aes_128_ecb_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_ECB);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(BLOCK_CIPHER_ID_AES_128_ECB, (vtable_ptr_t)&aes_128_ecb_vtable));

} // aes_128_ecb_register

/**
* Register the AES 192 ECB crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t aes_192_ecb_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_ECB);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(BLOCK_CIPHER_ID_AES_192_ECB, (vtable_ptr_t)&aes_192_ecb_vtable));

} // aes_192_ecb_register

/**
* Register the AES 256 ECB crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t aes_256_ecb_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_ECB);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(BLOCK_CIPHER_ID_AES_256_ECB, (vtable_ptr_t)&aes_256_ecb_vtable));

} // aes_256_ecb_register

/**
* Register the AES 128 CBC crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t aes_128_cbc_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_128_CBC);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(BLOCK_CIPHER_ID_AES_128_CBC, (vtable_ptr_t)&aes_128_cbc_vtable));

} // aes_128_cbc_register

/**
* Register the AES 192 CBC crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t aes_192_cbc_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_192_CBC);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(BLOCK_CIPHER_ID_AES_192_CBC, (vtable_ptr_t)&aes_192_cbc_vtable));

} // aes_192_cbc_register

/**
* Register the AES 256 CBC crypto driver.
*
* \returns CRYPTO_STATUS_SUCCESS if the item was successfully added.
*
*          CRYPTO_FAILURE_GENERAL on failure to add the item to the registry.
*/
crypto_status_t aes_256_cbc_register(void)
{

	vtable_handle_t handle = crypto_registry_get_vtable(BLOCK_CIPHER_ID_AES_256_CBC);

	return ((handle != NULL) ? CRYPTO_STATUS_SUCCESS
		: crypto_registry_add(BLOCK_CIPHER_ID_AES_256_CBC, (vtable_ptr_t)&aes_256_cbc_vtable));

} // aes_256_cbc_register

