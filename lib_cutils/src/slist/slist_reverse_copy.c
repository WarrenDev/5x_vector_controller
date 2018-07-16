/**
* slist_reverse_copy.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Create a shallow reverse copy of a list.
*/
#include <aef/cutils/slist.h>

/**
 * Return the reverse clone of the given list.  This reverse is a shallow copy
 * of all nodes.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The list to reverse.
 * \param output            The pointer to be updated with a copy of the given
 *                          list in reverse.
 *
 * \returns zero on success, and non-zero if allocation failed at any point
 *          during this operation.
 */
int slist_reverse_copy(const slist_options_t* options, slist_cons_t* list,
                       slist_cons_t** output)
{
    slist_cons_t* tmp;
    slist_cons_t* ret = 0;

    //set the output to null initially.
    *output = 0;

    while (list)
    {
        //attempt to clone the node
        tmp = options->slist_clone(options->slist_context, list);
        if (!tmp)
        {
            slist_deallocate(options, ret);

            return 1;
        }

        //if successful, place the cloned node on the front of the list
        ret = slist_insert(options, tmp, ret);
        list = list->next;
    }

    //update the output value
    *output = ret;

    return 0;
}
