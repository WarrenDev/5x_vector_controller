/**
* slist_default_options.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Default platform options for singly-linked lists.
*/
#include <aef/cutils/slist.h>
#include <string.h>
#include "bsp.h"

//forward decls
static slist_cons_t* default_slist_allocate(void* context, size_t);
static void default_slist_free(void* context, slist_cons_t*);
static slist_cons_t* default_slist_clone(void* context, slist_cons_t*);

/**
 * By default, we use malloc and free.  We clone the first two natural words
 * or pointers of a cons cell.
 */
static slist_options_t options =
{
    .slist_allocate = &default_slist_allocate,
    .slist_free = &default_slist_free,
    .slist_clone = &default_slist_clone,
    .slist_context = NULL
};

/**
 * Platform default options for slist.  This uses malloc() and free() for
 * allocation and deallocation respectively.  All cells are fixed to the size
 * of two pointers (one for the next pointer and one for the atom value).
 * This size should correspond to the native integer value on a given platform.
 */
slist_options_t* slist_default = &options;

/**
 * By default, use malloc to handle cons cell allocations.
 *
 * \param context       The user-defined context for this call.
 * \param size          The total size of the cons cell.
 *
 * \returns the newly allocated cons cell, or 0 if the cell could not
 *          be allocated.
 */
static slist_cons_t* default_slist_allocate(void* context, size_t size)
{
    slist_cons_t* ret = malloc(size);

    if (ret)
    {
        memset(ret, 0, size);
    }

    return ret;
};

/**
 * By default, use free to free an slist cons cell.
 *
 * \param context       The user-defined context for this call.
 * \param cell          A pointer to the cons cell to free.
 */
static void default_slist_free(void* context, slist_cons_t* cell)
{
    free(cell);
}

#define DEFAULT_CELL_SIZE (sizeof(slist_cons_t) + sizeof(void*))

/**
 * By default, assume that a cons cell is two pointers in size.  Clone this
 * size.
 *
 * \param context       The user-defined context for this call.
 * \param cell          The cell to clone.
 *
 * \returns a byte-for-byte shallow copy of the given cell, or 0 if the
 *          cell could not be allocated.
 */
static slist_cons_t* default_slist_clone(void* context, slist_cons_t* cell)
{
    slist_cons_t* ret = malloc(DEFAULT_CELL_SIZE);
    if (ret)
    {
        memcpy(ret, cell, DEFAULT_CELL_SIZE);
    }

    return ret;
}
