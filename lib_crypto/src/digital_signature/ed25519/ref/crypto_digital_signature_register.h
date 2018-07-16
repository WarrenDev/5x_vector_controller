
/**
* crypto_digital_signature_register.h
*
* \copyright
* Copyright 2015 Unikey Technologies, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <albert@unikey.com>
*
* \brief crypto digital signature register definitions.
*/

#ifndef CRYPTO_DIGITAL_SIGNATURE_REGISTER_H
#define CRYPTO_DIGITAL_SIGNATURE_REGISTER_H

#include <aef/crypto/crypto_status.h>

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	* Digital signature crypto registration routines
	*/
	crypto_status_t digital_signature_ed25519_register(void);

    /**
    * Digital signature Donna crypto registration routines
    */
    crypto_status_t digital_signature_ed25519_donna_register(void);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_DIGITAL_SIGNATURE_REGISTER_H
