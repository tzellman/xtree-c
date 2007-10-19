#ifndef __XTREE_LIST_H__
#define __XTREE_LIST_H__

#include "xtree/System.h"

CXX_GUARD

/**
 * The internal Node contained in a List
 */
typedef struct _xtree_ListNode
{
    struct _xtree_ListNode *prev;   /** The previous node */
    struct _xtree_ListNode *next;   /** The next node */
    void *data;                     /** The internal user data */
}xtree_ListNode;


typedef struct _xtree_List
{
    xtree_ListNode *first;          /** The beginning of the list */
    xtree_ListNode *last;           /** Marks the end of the list */
    size_t size;                    /** Current size of the list */
}xtree_List;

/**
 * ListIterator essentially is a ListNode*. We use it to iterate the list 
 */
typedef struct _xtree_ListIterator
{
    xtree_ListNode *node;
}xtree_ListIterator;

/**
 * Constructs a new ListNode
 */
PUBFUNC(xtree_ListNode*) xtree_ListNode_construct(xtree_ListNode *prev,
                                                  xtree_ListNode *next,
                                                  void *data);
/**
 * Destroys the ListNode
 * NOTE: The contents of the data are NOT free'd
 */
PUBFUNC(void) xtree_ListNode_destruct(xtree_ListNode **node);

/**
 * Constructs a new List
 */
PUBFUNC(xtree_List*) xtree_List_construct();

/**
 * Destroys the list
 */
PUBFUNC(void) xtree_List_destruct(xtree_List **list);

/**
 * Returns the size of the List
 */
PUBFUNC(size_t) xtree_List_getSize(xtree_List *list);

/**
 * Pushes a new ListNode onto the end of the List
 */
PUBFUNC(XTREE_BOOL) xtree_List_pushBack(xtree_List *list, void *data);

/**
 * Pushes a new ListNode onto the front of the List
 */
PUBFUNC(XTREE_BOOL) xtree_List_pushFront(xtree_List *list, void *data);


PUBFUNC(xtree_ListIterator) xtree_List_begin(xtree_List *list);

PUBFUNC(xtree_ListIterator) xtree_List_end(xtree_List *list);

PUBFUNC(void) xtree_ListIterator_increment(xtree_ListIterator *iter);

PUBFUNC(XTREE_BOOL) xtree_ListIterator_equals(xtree_ListIterator *first,
                                              xtree_ListIterator *second);

PUBFUNC(void*) xtree_ListIterator_get(xtree_ListIterator *iter);

/**
 * Inserts the data before the given iterator position.
 * The iterator will remain unchanged, so that you can continue using it.
 * If iter is NULL, this acts as an append, and pushes the element onto the
 * back of the list.
 * 
 * This will also update the list->first and/or list->list if need be.
 * 
 * \param list  the list to modify
 * \param iter  pointer to the iterator where you would like to insert the
 *              data. If NULL, this appends the data to the end of the list.
 * \param data  the data to add to the list
 * \return XTREE_TRUE if the insert was a success, o/w XTREE_FALSE
 */
PUBFUNC(XTREE_BOOL) xtree_List_insert(xtree_List *list,
                                      xtree_ListIterator *iter,
                                      void *data);

/**
 * Removes (and returns) the data at the given index.
 * This will increment the iterator, unless the iterator is already at the 
 * end of the list.
 * 
 * This will also update the list->first and/or list->list if need be.
 * 
 * \return the data at the index, or NULL if an error occurred
 */
PUBFUNC(void*) xtree_List_remove(xtree_List *list, xtree_ListIterator *iter);


/**
 * Pops the last ListNode off the List and returns the data in the node.
 */
PUBFUNC(void*) xtree_List_popBack(xtree_List *list);

/**
 * Pops the first ListNode off the List and returns the data in the node.
 */
PUBFUNC(void*) xtree_List_popFront(xtree_List *list);

CXX_ENDGUARD

#endif
