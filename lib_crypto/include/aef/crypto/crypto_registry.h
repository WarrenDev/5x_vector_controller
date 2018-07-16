
/**
* crypto_registry.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author 
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions use by the crypto registry.
*/

#ifndef CRYPTO_REGISTRY_H
#define CRYPTO_REGISTRY_H

#include <aef/crypto/crypto_status.h>

#include "aef/cutils/vtable_array.h"
#include "aef/cutils/system_registry.h"

# ifdef   __cplusplus
extern "C" {
# endif

typedef void* vtable_handle_t;

	////////////////////////////////////////////////////////
	// Crypto registry routines
	crypto_status_t	crypto_registry_create			(void);
	crypto_status_t	crypto_registry_destroy			(void);
	crypto_status_t	crypto_registry_add				(uint32_t crypto_id, vtable_ptr_t item_ptr);
	vtable_entry_t*	crypto_registry_get_item		(uint32_t crypto_id);
	vtable_handle_t	crypto_registry_get_vtable		(uint32_t crypto_id);
	uint32_t		crypto_registry_get_item_count	(void);


# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // CRYPTO_REGISTRY_H
