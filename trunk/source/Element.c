#include "xtree/Element.h"

PUBFUNC(xtree_Element*) xtree_Element_construct(xtree_Element* parent,
                                                char* tag,
                                                int elemType)
{
    xtree_Element *elem = (xtree_Element*)malloc(sizeof(xtree_Element));
    if (!elem) return NULL;
    elem->parent = parent;
    elem->tag = NULL;
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
    elem->elemType = elemType;
    elem->attributes = elem->children = NULL;
    elem->data = NULL;
    elem->isEmpty = 0;
    return elem;
}

PUBFUNC(void) xtree_Element_destruct(xtree_Element **elem)
{
    if (*elem)
    {
        if ((*elem)->children)
        {
            xtree_Element *data = NULL;
            xtree_ListIterator iter, end;
            iter = xtree_List_begin((*elem)->children);
            end = xtree_List_end((*elem)->children);
            while(!xtree_ListIterator_equals(&iter, &end))
            {
                data = (xtree_Element*)xtree_ListIterator_get(&iter);
                if (data)
                    xtree_Element_destruct(&data);
                xtree_ListIterator_increment(&iter);
            }
            xtree_List_destruct(&(*elem)->children);
        }
        if ((*elem)->attributes)
        {
            xtree_Attribute *data = NULL;
            xtree_ListIterator iter, end;
            iter = xtree_List_begin((*elem)->attributes);
            end = xtree_List_end((*elem)->attributes);
            while(!xtree_ListIterator_equals(&iter, &end))
            {
                data = (xtree_Attribute*)xtree_ListIterator_get(&iter);
                if (data)
                    xtree_Attribute_destruct(&data);
                xtree_ListIterator_increment(&iter);
            }
            xtree_List_destruct(&(*elem)->attributes);
        }
        if ((*elem)->data)
            free((*elem)->data);
        if ((*elem)->tag)
            free((*elem)->tag);
        free(*elem);
        *elem = NULL;
    }
}


PUBFUNC(xtree_Element*) xtree_Element_addChild(xtree_Element *elem,
                                               char *tag,
                                               int elemType)
{
    xtree_Element *child = NULL;
    
    if (!elem->children)
    {
        elem->children = xtree_List_construct();
        if (!elem->children)
            return NULL;
    }
    
    child = xtree_Element_construct(elem, tag, elemType);
    if (child)
        xtree_List_pushBack(elem->children, child);
    return child;
}

PUBFUNC(xtree_Element*) xtree_Element_addTextChild(xtree_Element *elem,
                                                   int elemType,
                                                   char *data)
{
    xtree_Element *child = xtree_Element_addChild(elem, NULL, elemType);
    if (child)
        xtree_Element_setData(child, data);
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

PUBFUNC(void) xtree_Element_setData(xtree_Element *elem, char *data)
{
    if (elem->data)
        free(elem->data);
    elem->data = malloc(strlen(data) + 1);
    if (!elem->data)
    {
        /* TODO - notify error */
    }
    strcpy(elem->data, data);
}

