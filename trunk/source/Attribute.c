#include "xtree/Attribute.h"

PUBFUNC(xtree_Attribute*) xtree_Attribute_construct(char *name, char *value)
{
    xtree_Attribute *attr = (xtree_Attribute*)malloc(sizeof(xtree_Attribute));
    if (!attr) return NULL;
    if (name)
    {
        attr->name = malloc(strlen(name) + 1);
        if (!attr->name)
        {
            xtree_Attribute_destruct(&attr);
            return NULL;
        }
        strcpy(attr->name, name);
    }
    
    if (value)
    {
        attr->value = malloc(strlen(value) + 1);
        if (!attr->value)
        {
            xtree_Attribute_destruct(&attr);
            return NULL;
        }
        strcpy(attr->value, value);
    }
    
    return attr;
}


PUBFUNC(void) xtree_Attribute_destruct(xtree_Attribute **attr)
{
    if (*attr)
    {
        if ((*attr)->name)
            free((*attr)->name);
        if ((*attr)->value)
            free((*attr)->value);
        free(*attr);
        *attr = NULL;
    }
}
