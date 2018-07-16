/**
* slist_new_cell.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Create a new slist cell and return it to the user.
*/
#include <aef/cutils/slist.h>

/**
 * Create a cons cell.
 *
 * \param options           The options to use for this operation.
 * \param size              The total size of this cell.
 *
 * \returns the newly created cell, or 0 if the cell could not be created.
 */
slist_cons_t* slist_new_cell(const slist_options_t* options, size_t size)
{
    return options->slist_allocate(options->slist_context, size);
}
