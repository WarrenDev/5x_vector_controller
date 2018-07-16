
/**
* string_binary_search.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief string binary search module header.
*/
#ifndef STRING_BINARY_SEARCH_H
#define STRING_BINARY_SEARCH_H

#include <aef/cutils/string_array.h>
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define ENTRY_NOT_FOUND        -1          // Not found

/**
*	string binary search error codes
*/
typedef enum string_binary_search_error_code
{
	STRING_ENTRY_NOT_FOUND = -1,		// Entry not found
	STRING_DUPLICATE_ENTRY = -2,		// Duplicate entry
	STRING_NO_MEMORY = -3,				// The string array is full
	STRING_TOO_LONG = -4				// String length too long
} string_binary_search_error_code_t;

/**
*	string binary search mode
*/
typedef enum string_binary_search_mode
{
	STRING_MODE_SEARCH = 0x0000,		// Search mode
	STRING_MODE_INSERT = 0x0001			// Insert mode
} string_binary_search_mode_t;

/**
*  string iterator callback function definition
*/
typedef void* (*string_item_callback)(string_entry_t* item, uint32_t);

/**
*	Function prototypes
*/
int32_t			string_binary_search	(string_entry_t arr[], void* string, int32_t left, int32_t right, string_binary_search_mode_t mode);
int32_t			string_binary_insert	(string_entry_t arr[], string_entry_t *new_entry, int32_t left, int32_t right);
string_entry_t* string_binary_retrieve	(string_entry_t arr[], void* string, int32_t left, int32_t right);
void* 			string_table_iterator 	(string_entry_t arr[], uint32_t count, string_item_callback callback, uint32_t param);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // STRING_BINARY_SEARCH_H
