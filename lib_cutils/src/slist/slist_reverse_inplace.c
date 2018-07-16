/**
* slist_reverse_inplace.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Destructively reverse a list in place.
*/
#include <aef/cutils/slist.h>

/**
 * Perform an in-place reverse of the given list.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The list to reverse.
 * \param output            The head of the reversed list.
 *
 * \returns zero in all cases.
 */
int slist_reverse_inplace(const slist_options_t* options, slist_cons_t* list,
                          slist_cons_t** output)
{
    slist_cons_t* tmp;
    slist_cons_t* ret = 0;

    while (list)
    {
        tmp = list->next;
        list->next = ret;
        ret = list;
        list = tmp;
    }

    *output = ret;

    return 0;
}
