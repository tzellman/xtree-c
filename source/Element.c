#include "xtree/Element.h"

PUBFUNC(xtree_Element*) xtree_Element_construct(xtree_Element* parent,
                                                char* tag)
{
    xtree_Element *elem = (xtree_Element*)malloc(sizeof(xtree_Element));
    if (!elem) return NULL;
    elem->parent = parent;
    if (tag)
    {
        elem->tag = malloc(strlen(tag) + 1);
        if (!elem->tag)
        {
            xtree_Element_destruct(&elem);
            return NULL;
        }
        strcpy(elem->tag, tag);
    }
    elem->attributes = elem->children = NULL;
    elem->text = elem->tail = NULL;
    return elem;
}

PUBFUNC(void) xtree_Element_destruct(xtree_Element **elem)
{
    if (*elem)
    {
        if ((*elem)->children)
        {
            xtree_Element *data = NULL;
            xtree_ListNode *cur = (*elem)->children->last;
            while(cur)
            {
                if (cur->data)
                {
                    data = (xtree_Element*)cur->data;
                    xtree_Element_destruct(&data);
                }
                cur = cur->prev;
            }
            xtree_List_destruct(&(*elem)->children);
        }
        if ((*elem)->attributes)
        {
            xtree_Attribute *data = NULL;
            xtree_ListNode *cur = (*elem)->attributes->last;
            while(cur)
            {
                if (cur->data)
                {
                    data = (xtree_Attribute*)cur->data;
                    xtree_Attribute_destruct(&data);
                }
                cur = cur->prev;
            }
            xtree_List_destruct(&(*elem)->attributes);
        }
        if ((*elem)->text)
            free((*elem)->text);
        if ((*elem)->tail)
            free((*elem)->tail);
        if ((*elem)->tag)
            free((*elem)->tag);
        free(*elem);
        *elem = NULL;
    }
}


PUBFUNC(xtree_Element*) xtree_Element_addChild(xtree_Element *elem, char *tag)
{
    xtree_Element *child = NULL;
    
    if (!elem->children)
    {
        elem->children = xtree_List_construct();
        if (!elem->children)
            return NULL;
    }
    
    child = xtree_Element_construct(elem, tag);
    if (child)
        xtree_List_pushBack(elem->children, child);
    return child;
}

PUBFUNC(xtree_Attribute*) xtree_Element_addAttribute(xtree_Element *elem,
                                                     char *name,
                                                     char *value)
{
    xtree_Attribute *attr = NULL;
    
    if (!elem->attributes)
    {
        elem->attributes = xtree_List_construct();
        if (!elem->attributes)
            return NULL;
    }
    
    attr = xtree_Attribute_construct(name, value);
    if (attr)
        xtree_List_pushBack(elem->attributes, attr);
    return attr;
}

PUBFUNC(void) xtree_Element_setText(xtree_Element *elem, char *text)
{
    if (elem->text)
        free(elem->text);
    elem->text = malloc(strlen(text) + 1);
    if (!elem->text)
    {
        /* TODO - notify error */
    }
    strcpy(elem->text, text);
}


PUBFUNC(void) xtree_Element_setTail(xtree_Element *elem, char *tail)
{
    if (elem->tail)
        free(elem->tail);
    elem->tail = malloc(strlen(tail) + 1);
    if (!elem->tail)
    {
        /* TODO - notify error */
    }
    strcpy(elem->tail, tail);
}

