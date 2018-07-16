
/**
* ll_crypto_memcmp.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto memory compare routine definitions.
*/

#ifndef LL_CRYPTO_MEMCMP_H
#define LL_CRYPTO_MEMCMP_H

#include "stddef.h"
#include "stdint.h"

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* low-level crypto memory comparison routines
	*/
	uint8_t ll_crypto_memcmp	(const uint8_t* lhs, const uint8_t* rhs, uint16_t length);
	uint8_t ll_crypto_byte_cmp	(const uint8_t* array, uint8_t byte, uint16_t length);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // LL_CRYPTO_MEMCMP_H
