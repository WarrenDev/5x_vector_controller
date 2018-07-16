
/**
* crypto_memcmp.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementations of crypto memcmp routines.
*/


#include <aef/crypto/crypto_memcmp.h>

/**
* To prevent a timing sidechannel attack, we cannot branch on secret data.
* This memory comparison always runs in constant time for the same length of data.
* We can exploit the use of exclusive or to gather differences between bytes, and
* or these into a result.  If no differences are found, the result is 0 just like memcmp.
* If differences are found, the result is a saturation of all differences.
*/
uint8_t crypto_memcmp(const uint8_t* lhs, const uint8_t* rhs, uint16_t length)
{
	uint8_t result = 0;

	while (length--)
		result |= *(lhs++) ^ *(rhs++);

	return result;
}

/**
* Compare length bytes from array against the given byte, and aggregate errors
* into the return value.  This comparison takes constant time.
*/
uint8_t crypto_byte_cmp(const uint8_t* array, uint8_t byte, uint16_t length)
{
	uint8_t ret = 0;

	while (length--)
		ret |= *(array++) ^ byte;

	return ret;
}
