
/**
* uuid_binary_search.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief UUID binary search module header.
*/
#ifndef UUID_BINARY_SEARCH_H
#define UUID_BINARY_SEARCH_H

#include <aef/cutils/uuid_array.h>
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define ENTRY_NOT_FOUND        -1          // Not found

/**
*	UUID binary search error codes
*/
typedef enum uuid_binary_search_error_code
{
	UUID_ENTRY_NOT_FOUND = -1,			// Entry not found
	UUID_DUPLICATE_ENTRY = -2,			// Duplicate entry
	UUID_NO_MEMORY = -3,				// The UUID array is full
	UUID_TOO_LONG = -4					// UUID length too long
} uuid_binary_search_error_code_t;

/**
*	UUID binary search mode
*/
typedef enum uuid_binary_search_mode
{
	UUID_MODE_SEARCH = 0x0000,			// Search mode
	UUID_MODE_INSERT = 0x0001			// Insert mode
} uuid_binary_search_mode_t;

/**
*  uuid iterator callback function definition
*/
typedef void* (*uuid_item_callback)(uuid_entry_t* item, uint32_t);

/**
*	Function prototypes
*/
int32_t			uuid_binary_search	(uuid_entry_t arr[], void* uuid, int32_t left, int32_t right, uuid_binary_search_mode_t mode);
int32_t			uuid_binary_insert	(uuid_entry_t arr[], uuid_entry_t *new_entry, int32_t left, int32_t right);
uuid_entry_t*	uuid_binary_retrieve(uuid_entry_t arr[], void* uuid, int32_t left, int32_t right);
void* 			uuid_table_iterator (uuid_entry_t arr[], uint32_t count, uuid_item_callback callback, uint32_t param);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // UUID_BINARY_SEARCH_H
