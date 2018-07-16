
/**
* uuid_array.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definition of the UUID array item.
*/
#ifndef UUID_ARRAY_H
#define UUID_ARRAY_H

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define	UUID_SIZE		16

	typedef void* uuid_entry_ptr_t;

	typedef struct uuid_entry_def
	{
		uint32_t		handle;
		uint32_t		length;
		uint8_t			uuid[UUID_SIZE];
	} uuid_entry_t, *p_uuid_entry_t;

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // UUID_ARRAY_H
