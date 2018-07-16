
/**
* binary_search.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief binary search module header.
*/
#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

typedef enum binary_search_error_code
{
    BS_NOT_FOUND            = -1,       // Entry not found
    BS_DUPLICATE_ENTRY      = -2,       // Duplicate entry
    BS_NO_MEMORY            = -3        // The array is full
} binary_search_error_code_t;

typedef enum binary_search_mode
{
    BS_MODE_SEARCH          = 0x0000,   // Search mode
    BS_MODE_INSERT          = 0x0001    // Insert mode
    
} binary_search_mode_t;

/**
 *	Function prototypes
 */
int32_t binary_search   (uint32_t arr[], uint32_t value, int32_t left, int32_t right, binary_search_mode_t mode);
int32_t binary_insert   (uint32_t arr[], uint32_t new_value, int32_t left, int32_t right );

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif //BINARY_SEARCH_H
