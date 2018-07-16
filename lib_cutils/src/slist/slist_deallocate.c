/**
* slist_deallocate.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Deallocate a list.
*/
#include <aef/cutils/slist.h>

/**
 * Deallocate all elements in the given list.  After this call, the list and all
 * elements it contains will be invalid and cannot be dereferenced.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The list to deallocate.
 *
 * \returns zero in all cases.
 */
int slist_deallocate(const slist_options_t* options, slist_cons_t* list)
{
    slist_cons_t* tmp;

    while (list)
    {
        tmp = list;
        list = list->next;

        options->slist_free(options->slist_context, tmp);
    }

    return 0;
}
