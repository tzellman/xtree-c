#ifndef __XTREE_PAIR_H__
#define __XTREE_PAIR_H__

#include "xtree/System.h"

typedef struct _xtree_Pair
{
    char *first;
    void *second;
}xtree_Pair;


PUBFUNC(xtree_Pair*) xtree_Pair_construct(const char *first, void *second);

PUBFUNC(void) xtree_Pair_destruct(xtree_Pair **pair);

#endif
