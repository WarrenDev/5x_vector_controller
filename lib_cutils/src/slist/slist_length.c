/**
* slist_length.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Compute the length of the given single-linked list.
*/
#include <aef/cutils/slist.h>

/**
 * Return the length of the given list.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The head of the current list.
 *
 * \returns the length of this list.
 */
int slist_length(const slist_options_t* options, slist_cons_t* list)
{
    int length = 0;

    while (list)
    {
        ++length;
        list = list->next;
    }

    return length;
}
