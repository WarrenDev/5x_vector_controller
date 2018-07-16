/**
* slist_insert.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Insert an element at the beginning of a list and return the result.
*/
#include <aef/cutils/slist.h>

/**
 * Insert the given cell at the beginning of the list.
 *
 * Note that this operation modifies cell's next pointer to point to list.
 * The list parameter is not touched.  If the list parameter is NULL or 0,
 * then this inserted entry will be the only entry in the returned list.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param cell              The cell to insert.
 * \param list              The list to which this cell will point.
 *
 * \returns the beginning of the updated list.
 */
slist_cons_t* slist_insert(const slist_options_t* options, slist_cons_t* cell,
                           slist_cons_t* list)
{
    cell->next = list;

    return cell;
}
