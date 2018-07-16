/**
* slist_next.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Return the next value in the given list.
*/
#include <aef/cutils/slist.h>

/**
 * Return the next cell in the given list.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The head of the current list.
 *
 * \returns the next element in this list, or 0 if at the end of the list.
 */
slist_cons_t* slist_next(const slist_options_t* options, slist_cons_t* list)
{
    if (list)
    {
        list = list->next;
    }

    return list;
}
