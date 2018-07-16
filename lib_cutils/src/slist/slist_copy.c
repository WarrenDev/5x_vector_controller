/**
* slist_copy.c
*
* \copyright
* Copyright 2016 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Create a shallow copy of a list.
*/
#include <aef/cutils/slist.h>

/**
 * Return a copy of the given list.  This copy is a shallow copy of all nodes.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The list to copy.
 * \param output            The pointer to be updated with a copy of the given
 *                          cloned list.
 *
 * \returns zero on success, or non-zero if allocation fails at any point during
 *          this operation.
 */
int slist_copy(const slist_options_t* options, slist_cons_t* list,
               slist_cons_t** output)
{
    if (slist_reverse_copy(options, list, output))
    {
        return 1;
    }
    else
    {
        //perform an inplace reverse of the reverse copied list.
        //This will put it in the same relative order as the original list.
        return slist_reverse_inplace(options, *output, output);
    }
}
