
/**
* vtable_binary_search.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Binary search routine.
*/

#include <aef/cutils/vtable_binary_search.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
* Searches for a value in a sorted array of vtable entry structures
*
*  /param arr is an array of vtable entry structures to search in
*  /param value is searched value in vtableId
*  /param left is an index of left boundary
*  /param right is an index of right boundary
*
* Returns the position of the value if present in the array
*   or -1, if not found when mode is VT_MODE_SEARCH.
* Returns the insert position of the value in the array. Duplicates are not be allowed
*/
int32_t vtable_binary_search (vtable_entry_t arr[], uint32_t value, int32_t left, int32_t right, vtable_binary_search_mode_t mode)
{

	int middle = 0;
    int dir = 0;

	while (left <= right)
	{
        dir = 0;
		middle = (left + right) / 2;
		if (arr[middle].vtableId == value)
            return ( ((mode == VT_MODE_SEARCH) ? middle : VT_DUPLICATE_ENTRY) );
		else
		{
			if (arr[middle].vtableId > value)
				right = middle - 1;
			else
            {
				left = middle + 1;
                dir = 1;
            }
		}
	} // while

    return ( ((mode == VT_MODE_SEARCH) ? VT_ENTRY_NOT_FOUND : (middle+dir)) );

} // vtable_binary_search

/*
* vtable_binary_insert - Insert a new vtable entry item into a sorted vtable entry item array
*
* /param  arr is an array of vtable entry items to insert in
* /param  new_value is vtable entry item to insert
* /param  left is an index of left boundary
* /param  right is an index of right boundary
*
* Returns the insert position of the vtable entry item in the array. Duplicates are not allowed
*/
int32_t vtable_binary_insert (vtable_entry_t arr[], vtable_entry_t *new_entry, int32_t left, int32_t right )
{

	int32_t index = vtable_binary_search(arr, new_entry->vtableId, left, right, VT_MODE_INSERT);
   
	if ( index >= 0 )
	{
		memmove ( arr+index+1, arr+index, sizeof(vtable_entry_t) * ((right+1)-index) );
		memcpy  ( arr+index, new_entry, sizeof(vtable_entry_t) );
	}
   
   return index;
        
} // vtable_binary_insert

/*
* vtable_binary_retrieve - retrieve a vtable entry item from the sorted vtable entry item array
*
*  /param  arr is an array of vtable entry items
*  /param  value is vtable entry item to retrieve
*  /param  left is an index of left boundary
*  /param  right is an index of right boundary
*
* Returns the pointer of a vtable entry item if present in the array or NULL if not found.
*/
vtable_entry_t* vtable_binary_retrieve(vtable_entry_t arr[], uint32_t value, int32_t left, int32_t right)
{

	 int32_t index = vtable_binary_search(arr, value, left, right, VT_MODE_SEARCH);

	 return ( (index >= 0) ? (vtable_entry_t*)(arr + index) : NULL );

} // vtable_binary_retrieve

/*
* vtable_table_iterator - iterate through a binary table and call a callback routine on each item.
* If the callback routine returns NULL, the iteration continues.
* If the callback routine returns a non-NULL value, the iteration terminates.
*
*  /param  arr is an array of vtable entry items
*  /param  count is the number of items in the table
*  /param  callback is a pointer to the callback routine
*  /param  param is the data to pass to the callback routine
*
* Returns the value returned by the callback routine if non-NULL, else NULL.
*/
void* vtable_table_iterator (vtable_entry_t arr[], uint32_t count, vtable_item_callback callback, uint32_t param)
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
