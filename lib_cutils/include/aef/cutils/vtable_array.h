	
/**
* vtable_array.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definition of the vtable array.
*/
#ifndef VTABLE_ARRAY_H
#define VTABLE_ARRAY_H

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

	typedef void* vtable_ptr_t;

	typedef struct vtable_entry_def
	{
		uint32_t		handle;
		uint32_t		vtableId;
		vtable_ptr_t	vtablePtr;
	} vtable_entry_t, *p_vtable_entry_t;

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // VTABLE_ARRAY_H
