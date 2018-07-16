
/**
* ll_crypto_memcmp.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto memory compare implementation.
*/

#include <aef/crypto/crypto_memcmp.h>
#include <aef/crypto/crypto_status.h>
#include "stddef.h"

/**
* To prevent a timing sidechannel attack, we cannot branch on secret data.
* This memory comparison always runs in constant time for the same length of data.
* We can exploit the use of exclusive or to gather differences between bytes, and
* or these into a result.  If no differences are found, the result is 0 just like memcmp.
* If differences are found, the result is a saturation of all differences.
*/
uint8_t ll_crypto_memcmp(const uint8_t* lhs, const uint8_t* rhs, uint16_t length)
{

	return crypto_memcmp(lhs, rhs, length);

} // ll_crypto_memcmp


/**
* Compare length bytes from array against the given byte, and aggregate errors
* into the return value.  This comparison takes constant time.
*/
uint8_t ll_crypto_byte_cmp(const uint8_t* array, uint8_t byte, uint16_t length)
{

	return crypto_byte_cmp(array, byte, length);

} // ll_crypto_byte_cmp

