
/**
* string_binary_search.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief String binary search routine.
*/

#include <aef/cutils/string_binary_search.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
* string_binary_search - Searches for a string value in a sorted array of vtable entry structures
*  arr is an array of string entry structures to search in
*  string is searched string
*  left is an index of left boundary
*  right is an index of right boundary
* Returns the position of the string if present in the array
*   or -1, if not found when mode is STRING_MODE_SEARCH.
* Returns the insert position of the string in the array. Duplicates are not be allowed
*/
int32_t string_binary_search(string_entry_t arr[], void* string, int32_t left, int32_t right, string_binary_search_mode_t mode)
{

	int result;
	int middle = 0;
	int dir = 0;

	while (left <= right)
	{
		dir = 0;
		middle = (left + right) / 2;
		result = strncmp((const char*)arr[middle].string, (const char*)string, MAX_STRING_SIZE);
		if ( result == 0 )
			return (((mode == STRING_MODE_SEARCH) ? middle : STRING_DUPLICATE_ENTRY));
		else
		{
			if ( result > 0 )
				right = middle - 1;
			else
			{
				left = middle + 1;
				dir = 1;
			}
		}
	} // while

	return (((mode == STRING_MODE_SEARCH) ? STRING_ENTRY_NOT_FOUND : (middle + dir)));

} // string_binary_search

/*
* string_binary_insert - Insert a new string entry item into a sorted string entry item array
*   arr is an array of string entry items to insert in
*   new_string is string entry item to insert
*   left is an index of left boundary
*   right is an index of right boundary
* Returns the insert position of the string entry item in the array. Duplicates are not allowed
*/
int32_t string_binary_insert(string_entry_t arr[], string_entry_t *new_entry, int32_t left, int32_t right)
{

	int32_t index = string_binary_search(arr, new_entry->string, left, right, STRING_MODE_INSERT);

	if (index >= 0)
	{
		memmove(arr + index + 1, arr + index, sizeof(string_entry_t) * ((right + 1) - index));
		memcpy(arr + index, new_entry, sizeof(string_entry_t));
	}

	return index;

} // string_binary_insert

/*
* string_binary_retrieve - retrieve a string entry item from the sorted string entry item array
*   arr is an array of string entry items to insert in
*   string is string entry item to retrieve
*   left is an index of left boundary
*   right is an index of right boundary
* Returns the pointer of a string entry item if present in the array or NULL if not found.
*/
string_entry_t* string_binary_retrieve(string_entry_t arr[], void* string, int32_t left, int32_t right)
{

	int32_t index = string_binary_search(arr, string, left, right, STRING_MODE_SEARCH);

	return ((index >= 0) ? (string_entry_t*)(arr + index) : NULL);

} // string_binary_retrieve

/*
* string_table_iterator - iterate through a binary table and call a callback routine on each item.
* If the callback routine returns NULL, the iteration continues.
* If the callback routine returns a non-NULL value, the iteration terminates.
*
*  /param  arr is an array of string entry items
*  /param  count is the number of items in the table
*  /param  callback is a pointer to the callback routine
*  /param  param is the data to pass to the callback routine
*
* Returns the value returned by the callback routine if non-NULL, else NULL.
*/
void* string_table_iterator (string_entry_t arr[], uint32_t count, string_item_callback callback, uint32_t param)
{
	if ( callback != NULL )
	{
		uint32_t index;
		void* result;
		for (index = 0; index < count; index++)
		{
			result = (*callback)((arr + index), param);
			if ( result )
				return result;
		}
	}
	return NULL;
}
