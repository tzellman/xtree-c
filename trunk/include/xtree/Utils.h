#ifndef __XTREE_UTILS_H__
#define __XTREE_UTILS_H__

#include <string.h>
#include "xtree/System.h"

/**
 * This function does whitespace reduction on the input string.
 * If the inPlace flag is true, we modify the buffer directly. This means
 * that the result buffer would be the same as the input buffer.
 * 
 * For ease, we return the data buffer if it contains non-whitespace data.
 * Otherwise, we return NULL.
 * No matter what, we never free() the input buffer.
 * 
 * Example:
 * before: "   This    is a\n non-compact \t          string\n"
 * after:  "This is a non-compact string"
 * 
 * \param str       The string to compact.
 * \param inPlace   If true, we modify the input in-place, o/w we copy and return a new buffer.
 * \return          The input string if non-whitespace data exists, o/w NULL.
 */
PUBFUNC(char*) xtree_compactWhitespace(char *str, int inPlace);


#endif

