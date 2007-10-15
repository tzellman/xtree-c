#ifndef __XTREE_ERROR_H__
#define __XTREE_ERROR_H__

#include <memory.h>
#include <stdio.h>

#define MAX_ERR_MSG 512

typedef struct _xtree_Error
{
    char errorMessage[MAX_ERR_MSG];
} xtree_Error;

void xtree_Error_init(xtree_Error *err);

void xtree_Error_populate(xtree_Error *err,
                          const char *msg,
                          const char *file,
                          int lineno);

#define XTREE_ERROR(err, msg) xtree_Error_populate(err, msg, __FILE__, __LINE__);

#endif
