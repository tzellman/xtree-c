#ifndef __XTREE_ATTRIBUTE_H__
#define __XTREE_ATTRIBUTE_H__

#include "xtree/System.h"

CXX_GUARD

typedef struct _xtree_Attribute
{
    char *name;
    char *value;
}xtree_Attribute;


/**
 * Construct a new Attribute
 */
PUBFUNC(xtree_Attribute*) xtree_Attribute_construct(char *name, char *value);

/**
 * Destroy an Attribute
 */
PUBFUNC(void) xtree_Attribute_destruct(xtree_Attribute **attribute);

CXX_ENDGUARD

#endif

