
/**
* string_array.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definition of the string array item.
*/
#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define MAX_STRING_SIZE	64

	typedef void* string_entry_ptr_t;

	typedef struct string_entry_def
	{
		uint32_t		handle;
		uint32_t		length;
		uint8_t			string[MAX_STRING_SIZE];
	} string_entry_t, *p_string_entry_t;

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // STRING_ARRAY_H
