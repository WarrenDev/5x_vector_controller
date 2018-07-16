
/**
* vtable_binary_search.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief vtable binary search module header.
*/
#ifndef VTABLE_BINARY_SEARCH_H
#define VTABLE_BINARY_SEARCH_H

#include <aef/cutils/vtable_array.h>
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define ENTRY_NOT_FOUND        -1          // Not found

/**
 *	vtable binary search error codes
 */
typedef enum vtable_binary_search_error_code
{
    VT_ENTRY_NOT_FOUND      = -1,       // Entry not found
    VT_DUPLICATE_ENTRY      = -2,       // Duplicate entry
    VT_NO_MEMORY            = -3        // The vtable array is full
} vtable_binary_search_error_code_t;

/**
 *	vtable binary search mode
 */
typedef enum vtable_binary_search_mode
{
    VT_MODE_SEARCH          = 0x0000,   // Search mode
    VT_MODE_INSERT          = 0x0001    // Insert mode   
} vtable_binary_search_mode_t;

/**
*  vtable iterator callback function definition
*/
typedef void* (*vtable_item_callback)(vtable_entry_t* item, uint32_t param);

/**
 *	Function prototypes
 */
int32_t			vtable_binary_search    ( vtable_entry_t arr[], uint32_t value, int32_t left, int32_t right, vtable_binary_search_mode_t mode );
int32_t			vtable_binary_insert    ( vtable_entry_t arr[], vtable_entry_t *new_entry, int32_t left, int32_t right );
vtable_entry_t* vtable_binary_retrieve	( vtable_entry_t arr[], uint32_t value, int32_t left, int32_t right );
void* 			vtable_table_iterator 	( vtable_entry_t arr[], uint32_t count, vtable_item_callback callback, uint32_t param );

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // VTABLE_BINARY_SEARCH_H
