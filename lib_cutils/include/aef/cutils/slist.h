/**
* slist.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \brief Singly-linked list implementation in C.
*/
#ifndef SLIST_H_HEADER_GUARD
#define SLIST_H_HEADER_GUARD

#include <stdlib.h>

# ifdef   __cplusplus
extern "C" {
# endif

/**
 * \brief The cons cell is the backbone of a single-linked list.
 *
 * This implementation differs from the classical A/D implementation in that
 * we include the pointer to the next node as the first value in the cell,
 * and we do NOT define the second value.  Instead, this value becomes
 * implementation defined.  Using the C-style HAS-A mechanism for extending
 * a data structure, a user can implement his/her own cons cell as per this
 * example.
 *
 * typedef struct string_list
 * {
 *      slist_cons_t hdr;
 *      char string_val[40];
 * } string_list_t;
 */
typedef struct slist_cons
{
    struct slist_cons* next;
} slist_cons_t;

/**
 * \brief singly-linked list options.
 *
 * This structure contains the options to be used when allocating, cloning,
 * reversing, or iterating singly-linked lists.
 */
typedef struct slist_options
{
    /**
     * Allocate a cons cell.
     *
     * \param context       The user-defined context for this call.
     * \param size          The total size of the cons cell.
     *
     * \returns the newly allocated cons cell, or 0 if the cell could not
     *          be allocated.
     */
    slist_cons_t* (*slist_allocate)(void* context, size_t size);

    /**
     * Free a cons cell.
     *
     * \param context       The user-defined context for this call.
     * \param cell          A pointer to the cons cell to free.
     */
    void (*slist_free)(void* context, slist_cons_t* cell);

    /**
     * Clone a cons cell.
     *
     * Note that this method may require additional context only available
     * to the user in order to perform this cloning operation.
     *
     * \param context       The user-defined context for this call.
     * \param cell          The cell to clone.
     *
     * \returns a byte-for-byte shallow copy of the given cell, or 0 if the
     *          cell could not be allocated.
     */
    slist_cons_t* (*slist_clone)(void* context, slist_cons_t* cell);

    /**
     * Context for slist options.
     *
     * This context is passed into the allocate, clone, and
     * free methods.  This context can be used for implementation-specific
     * information.
     */
    void* slist_context;
} slist_options_t;

/**
 * Platform default options for slist.  This uses malloc() and free() for
 * allocation and deallocation respectively.  All cells are fixed to the size
 * of two pointers (one for the next pointer and one for the atom value).
 * This size should correspond to the native integer value on a given platform.
 */
extern slist_options_t* slist_default;

/**
 * A user-defined filter method.
 *
 * \param cell              The cell to test.
 *
 * \returns zero if this cell matches and non-zero if this cell does not match.
 */
typedef int (*slist_filter_method_t)(slist_cons_t* cell);

/**
 * Create a cons cell.
 *
 * \param options           The options to use for this operation.
 * \param size              The total size of this cell.
 *
 * \returns the newly created cell, or 0 if the cell could not be created.
 */
slist_cons_t* slist_new_cell(const slist_options_t* options, size_t size);

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
                           slist_cons_t* list);

/**
 * Return the next cell in the given list.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The head of the current list.
 *
 * \returns the next element in this list, or 0 if at the end of the list.
 */
slist_cons_t* slist_next(const slist_options_t* options, slist_cons_t* list);

/**
 * Return the length of the given list.
 *
 * \param options           The slist options structure to use for this
 *                          operation.
 * \param list              The head of the current list.
 *
 * \returns the length of this list.
 */
int slist_length(const slist_options_t* options, slist_cons_t* list);

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
               slist_cons_t** output);

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
                       slist_cons_t** output);

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
                          slist_cons_t** output);

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
int slist_deallocate(const slist_options_t* options, slist_cons_t* list);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // SLIST_H_HEADER_GUARD
