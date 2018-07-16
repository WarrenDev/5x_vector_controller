
/**
 * binarySearch.c
 *
 * \copyright
 * Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
 *
 * \brief Binary search routine.
 */
 
#include <aef/cutils/binary_search.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
 * binary_search - Searches for a value in a sorted array
 *   arr is an array to search in
 *   value is searched value
 *   left is an index of left boundary
 *   right is an index of right boundary
 * Returns the position of the value if present in the array
 *   or -1, if not found when mode is BS_MODE_SEARCH.
 * Returns the insert position of the value in the array. Duplicates are not be allowed
 */
int32_t binary_search (uint32_t arr[], uint32_t value, int32_t left, int32_t right, binary_search_mode_t mode) 
{   

    int middle = 0;
    int dir;

    while (left <= right) 
    {
        dir = 0;
        middle = (left + right) / 2;
        if ( arr[middle] == value )
            return ( ((mode == BS_MODE_SEARCH) ? middle : BS_DUPLICATE_ENTRY) );
        else 
        {
            if (arr[middle] > value)
                right = middle - 1;
            else
            {
                left = middle + 1;
                dir = 1;
            }
        }
    } // while
    
    return ( ((mode == BS_MODE_SEARCH) ? BS_NOT_FOUND : (middle+dir)) );
    
} // binary_search

/*
 * binary_insert - Insert a new value into a sorted array
 *   arr is an array to insert in
 *   new_value is value to insert
 *   left is an index of left boundary
 *   right is an index of right boundary
 * Returns the position of the value if present in the array
 *   or -1, if not found when mode is BS_MODE_SEARCH.
 * Returns the insert position of the value in the array. Duplicates are not be allowed
 */
 int32_t binary_insert (uint32_t arr[], uint32_t new_value, int32_t left, int32_t right )
 {
 
    int32_t index = binary_search ( arr, new_value, left, right, BS_MODE_INSERT );
    
    if ( index >= 0 )
    {
        memmove ( arr+index+1, arr+index, sizeof(uint32_t) * ((right+1)-index) );
        arr[index] = new_value;        
    }   
    
    return index;
         
 } // binary_insert
 
