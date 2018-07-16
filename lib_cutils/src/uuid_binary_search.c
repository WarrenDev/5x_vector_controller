
/**
* uuid_binary_search.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief String binary search routine.
*/

#include <aef/cutils/uuid_binary_search.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
* uuid_binary_search - Searches for a uuid value in a sorted array of vtable entry structures
*  arr is an array of uuid entry structures to search in
*  uuid is searched uuid
*  left is an index of left boundary
*  right is an index of right boundary
* Returns the position of the uuid if present in the array
*   or -1, if not found when mode is STRING_MODE_SEARCH.
* Returns the insert position of the uuid in the array. Duplicates are not be allowed
*/
int32_t uuid_binary_search(uuid_entry_t arr[], void* uuid, int32_t left, int32_t right, uuid_binary_search_mode_t mode)
{

	int result;
	int middle = 0;
	int dir = 0;

	while (left <= right)
	{
		dir = 0;
		middle = (left + right) / 2;
		result = memcmp((const void*)arr[middle].uuid, (const void*)uuid, UUID_SIZE);
		if ( result == 0 )
			return (((mode == UUID_MODE_SEARCH) ? middle : UUID_DUPLICATE_ENTRY));
		else
		{
			if (result > 0)
				right = middle - 1;
			else
			{
				left = middle + 1;
				dir = 1;
			}
		}
	} // while
	
	return (((mode == UUID_MODE_SEARCH) ? UUID_ENTRY_NOT_FOUND : (middle + dir)));

} // uuid_binary_search

/*
* uuid_binary_insert - Insert a new uuid entry item into a sorted uuid entry item array
*   arr is an array of uuid entry items to insert in
*   new_uuid is uuid entry item to insert
*   left is an index of left boundary
*   right is an index of right boundary
* Returns the insert position of the uuid entry item in the array. Duplicates are not allowed
*/
int32_t uuid_binary_insert(uuid_entry_t arr[], uuid_entry_t *new_entry, int32_t left, int32_t right)
{

	int32_t index = uuid_binary_search(arr, new_entry->uuid, left, right, UUID_MODE_INSERT);

	if (index >= 0)
	{
		memmove(arr + index + 1, arr + index, sizeof(uuid_entry_t) * ((right + 1) - index));
		memcpy(arr + index, new_entry, sizeof(uuid_entry_t));
	}

	return index;

} // uuid_binary_insert

/*
* uuid_binary_retrieve - retrieve a uuid entry item from the sorted uuid entry item array
*   arr is an array of uuid entry items to insert in
*   uuid is uuid entry item to retrieve
*   left is an index of left boundary
*   right is an index of right boundary
* Returns the pointer of a uuid entry item if present in the array or NULL if not found.
*/
uuid_entry_t* uuid_binary_retrieve(uuid_entry_t arr[], void* uuid, int32_t left, int32_t right)
{

	int32_t index = uuid_binary_search(arr, uuid, left, right, UUID_MODE_SEARCH);

	return ((index >= 0) ? (uuid_entry_t*)(arr + index) : NULL);

} // uuid_binary_retrieve

/*
* uuid_table_iterator - iterate through a binary table and call a callback routine on each item.
* If the callback routine returns NULL, the iteration continues.
* If the callback routine returns a non-NULL value, the iteration terminates.
*
*  /param  arr is an array of uuid entry items
*  /param  count is the number of items in the table
*  /param  callback is a pointer to the callback routine
*  /param  param is the data to pass to the callback routine
*
* Returns the value returned by the callback routine if non-NULL, else NULL.
*/
void* uuid_table_iterator (uuid_entry_t arr[], uint32_t count, uuid_item_callback callback, uint32_t param)
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
