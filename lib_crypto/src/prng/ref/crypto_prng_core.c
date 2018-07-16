
/**
* crypto_prng_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto PRNG core implementation.
*/

#include "crypto_prng_core.h"

/**
* increment_counter
*
*/
void increment_counter (uint8_t counter[])
{
	int position = 0;
	int length = PRNG_COUNTER_BITS / 8;
	uint8_t newValue;

	do 
	{
		newValue = ++counter[position++];
	} while (newValue == 0 && position < length);
}

/**
* is_counter_zero
*
*/
uint8_t is_counter_zero (uint8_t counter[])
{
	uint8_t size = PRNG_COUNTER_BITS / 8;
	uint8_t index = 0;

	for (index = 0; index < size; index++) 
	{
		if (counter[index] != 0) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**
* twoLeastSignificantBytes
*
*/
crypto_status_t twoLeastSignificantBytes (uint8_t result[], long value)
{
	result[0] = (uint8_t)(value & 0xff);
	result[1] = (uint8_t)((value & 0xff00) >> 8);

	return CRYPTO_STATUS_SUCCESS;
}

/**
* prng_ceil
*
*/
int prng_ceil (int value, int divisor)
{
	return (value / divisor) + (value % divisor == 0 ? 0 : 1);
}
