
/**
* crypto_memcmp.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions of crypto memcmp routines.
*/

#ifndef CRYPTO_MEMCMP_H
#define CRYPTO_MEMCMP_H

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

uint8_t crypto_memcmp	(const uint8_t* lhs, const uint8_t* rhs, uint16_t length);
uint8_t crypto_byte_cmp	(const uint8_t* array, uint8_t byte, uint16_t length);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_MEMCMP_H
