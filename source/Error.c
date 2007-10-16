#include "xtree/Error.h"

void xtree_Error_init(xtree_Error* err)
{
    memset((char*)err, 0, sizeof(xtree_Error));
}

void xtree_Error_populate(xtree_Error* err, const char* msg, const char* file, int lineno)
{
    snprintf(err->errorMessage, MAX_ERR_MSG, "%s:%d: %s", file, lineno, msg);
}

