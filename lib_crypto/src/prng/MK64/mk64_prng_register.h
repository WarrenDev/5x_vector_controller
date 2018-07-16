/**
* mk64_prng_register.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64 crypto PRNG register definitions.
*/

#ifndef SRC_PRNG_MK64_MK64_PRNG_REGISTER_H_
#define SRC_PRNG_MK64_MK64_PRNG_REGISTER_H_

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* PRNG crypto registration routines
	*/
	crypto_status_t mk64_prng_register(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_PRNG_MK64_MK64_PRNG_REGISTER_H_ */
