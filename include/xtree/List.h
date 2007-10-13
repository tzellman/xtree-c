#ifndef __XTREE_LIST_H__
#define __XTREE_LIST_H__

#include <stdlib.h>
#include <memory.h>

#include "xtree/System.h"

typedef struct _xtree_ListNode
{
    struct _xtree_ListNode *prev;
    struct _xtree_ListNode *next;
    void *data;
}xtree_ListNode;


typedef struct _xtree_List
{
    xtree_ListNode *first;
    xtree_ListNode *last;
    size_t size;
}xtree_List;

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
PUBFUNC(void) xtree_List_pushBack(xtree_List *list, void *data);

/**
 * Pushes a new ListNode onto the front of the List
 */
PUBFUNC(void) xtree_List_pushFront(xtree_List *list, void *data);

/**
 * Pops the last ListNode off the List and returns the data in the node.
 */
PUBFUNC(void*) xtree_List_popBack(xtree_List *list);

/**
 * Pops the first ListNode off the List and returns the data in the node.
 */
PUBFUNC(void*) xtree_List_popFront(xtree_List *list);

#endif
