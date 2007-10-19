#include "xtree/List.h"


PUBFUNC(xtree_ListNode*) xtree_ListNode_construct(xtree_ListNode *prev,
                                                  xtree_ListNode *next,
                                                  void *data)
{
    xtree_ListNode *node = (xtree_ListNode*)malloc(sizeof(xtree_ListNode));
    if (!node) return NULL;
    node->prev = prev;
    node->next = next;
    node->data = data;
    return node;
}

PUBFUNC(void) xtree_ListNode_destruct(xtree_ListNode **node)
{
    if (*node)
        free(*node);
    *node = NULL;
}


PUBFUNC(xtree_List*) xtree_List_construct()
{
    xtree_List *list = (xtree_List*)malloc(sizeof(xtree_List));
    if (!list) return NULL;
    list->size = 0;
    list->first = list->last = NULL;
    return list;
}

PUBFUNC(void) xtree_List_destruct(xtree_List **list)
{
    if (*list)
    {
        xtree_ListIterator iter, end;
        iter = xtree_List_begin(*list);
        end = xtree_List_end(*list);
        /* the remove method automatically updates the iterator */
        while(!xtree_ListIterator_equals(&iter, &end))
            xtree_List_remove(*list, &iter);
        free(*list);
    }
    *list = NULL;
}

PUBFUNC(size_t) xtree_List_getSize(xtree_List *list)
{
    return list ? list->size : 0;
}


PUBFUNC(xtree_ListIterator) xtree_List_begin(xtree_List *list)
{
    assert(list);
    xtree_ListIterator iter;
    iter.node = list ? list->first : NULL;
    return iter;
}

PUBFUNC(xtree_ListIterator) xtree_List_end(xtree_List *list)
{
    assert(list);
    xtree_ListIterator iter;
    iter.node = NULL;
    return iter;
}

PUBFUNC(void) xtree_ListIterator_increment(xtree_ListIterator *iter)
{
    if (iter)
        iter->node = iter->node ? iter->node->next : NULL;
}

PUBFUNC(XTREE_BOOL) xtree_ListIterator_equals(xtree_ListIterator *first,
                                              xtree_ListIterator *second)
{
    return first == second || (first && second && first->node == second->node);
}

PUBFUNC(void*) xtree_ListIterator_get(xtree_ListIterator *iter)
{
    return iter && iter->node ? iter->node->data : NULL;
}

PUBFUNC(XTREE_BOOL) xtree_List_insert(xtree_List *list,
                                      xtree_ListIterator *iter,
                                      void *data)
{
    xtree_ListNode *node = NULL;
    if (!list) goto EXCEPTION;
    
    /* if we have an iterator, insert it before the current node */
    if (iter && iter->node)
    {
        node = xtree_ListNode_construct(iter->node->prev, iter->node, data);
        if (!node) goto EXCEPTION;
        
        /* update the list delims in case we are at the corners */
        if (list->first == iter->node)
            list->first = node;
        if (!list->last)
            list->last = node;
        iter->node->prev = node;
    }
    /* otherwise, if iterator is NULL, we append it to the end of the list */
    else
    {
        node = xtree_ListNode_construct(list->last, NULL, data);
        if (!node) goto EXCEPTION;
        if (list->last)
            list->last->next = node;
        list->last = node;
        if (!list->first)
            list->first = node;
    }
    
    list->size++; /* update the size */
    return XTREE_TRUE;

  EXCEPTION:
    return XTREE_FALSE;
}

PUBFUNC(void*) xtree_List_remove(xtree_List *list,
                                 xtree_ListIterator *iter)
{
    xtree_ListNode *node = NULL;
    void *data = NULL;
    if (!list || !iter || !iter->node) goto EXCEPTION;
    
    node = iter->node;
    if (node->next)
    {
        iter->node = node->next;
        iter->node->prev = node->prev;
        if (node->prev)
            node->prev->next = iter->node;
    }
    else
    {
        /* if we are already at the end, just "pop" it off */
        iter->node = node->prev;
    }
    
    /* update the list delims, in case they were removed */
    if (list->last == node)
        list->last = iter->node;
    if (list->first == node)
        list->first = iter->node;
        
    data = node->data;
    xtree_ListNode_destruct(&node);
    list->size--; /* update the size */
    return data;

  EXCEPTION:
    return NULL;
}


PUBFUNC(void*) xtree_List_popBack(xtree_List *list)
{
    xtree_ListIterator iter = xtree_List_end(list);
    return xtree_List_remove(list, &iter);
}

PUBFUNC(void*) xtree_List_popFront(xtree_List *list)
{
    xtree_ListIterator iter = xtree_List_begin(list);
    return xtree_List_remove(list, &iter);
}

PUBFUNC(XTREE_BOOL) xtree_List_pushBack(xtree_List *list, void *data)
{
    xtree_ListIterator iter = xtree_List_end(list);
    return xtree_List_insert(list, &iter, data);
}

PUBFUNC(XTREE_BOOL) xtree_List_pushFront(xtree_List *list, void *data)
{
    xtree_ListIterator iter = xtree_List_begin(list);
    return xtree_List_insert(list, &iter, data);
}
