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
        xtree_ListNode *rmNode = NULL;
        while((*list)->last)
        {
            rmNode = (*list)->last;
            (*list)->last = (*list)->last->prev;
            xtree_ListNode_destruct(&rmNode);
        }
        free(*list);
    }
    *list = NULL;
}

PUBFUNC(size_t) xtree_List_getSize(xtree_List *list)
{
    return list->size;
}

PUBFUNC(void) xtree_List_pushBack(xtree_List *list, void *data)
{
    xtree_ListNode *node = xtree_ListNode_construct(list->last, NULL, data);
    if (list->last)
        list->last->next = node;
    list->last = node;
    if (!list->first)
        list->first = list->last;
    list->size++;
}

PUBFUNC(void) xtree_List_pushFront(xtree_List *list, void *data)
{
    xtree_ListNode *node = xtree_ListNode_construct(NULL, list->first, data);
    if (list->first)
        list->first->prev = node;
    list->first = node;
    if (!list->last)
        list->last = list->first;
    list->size++;
}

PUBFUNC(void*) xtree_List_popBack(xtree_List *list)
{
    void *data = NULL;
    xtree_ListNode *rmNode = NULL;
    if (!list->size) return NULL;
    data = list->last->data;
    
    rmNode = list->last;
    list->last = list->last->prev;
    list->last->next = NULL;
    
    xtree_ListNode_destruct(&rmNode);
    list->size--;
    return data;
}

PUBFUNC(void*) xtree_List_popFront(xtree_List *list)
{
    void *data = NULL;
    xtree_ListNode *rmNode = NULL;
    if (!list->size) return NULL;
    data = list->first->data;
    
    rmNode = list->first;
    list->first = list->first->next;
    list->first->prev = NULL;
    
    xtree_ListNode_destruct(&rmNode);
    list->size--;
    return data;
}

