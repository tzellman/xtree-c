#include "xtree/Pair.h"

PUBFUNC(xtree_Pair*) xtree_Pair_construct(const char *first, void *second)
{
    xtree_Pair *pair = (xtree_Pair*)malloc(sizeof(xtree_Pair));
    pair->first = malloc(strlen(first) + 1);
    /* TODO - mem checks */
    strcpy(pair->first, first);
    pair->second = second;
    return pair;
}

PUBFUNC(void) xtree_Pair_destruct(xtree_Pair **pair)
{
    if (*pair)
    {
        if ((*pair)->first)
            free((*pair)->first);
        free(*pair);
    }
    *pair = NULL;
}
