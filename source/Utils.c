#include "xtree/Utils.h"

PUBFUNC(char*) xtree_compactWhitespace(char *str, int inPlace)
{
    char *tok = NULL, *text = NULL;
    int len = 0, tokLen;
    
    if (!inPlace)
    {
        text = malloc(strlen(str) + 1);
        strcpy(text, str);
    }
    else
        text = str;
    
    tok = strtok(text, " \n\t");
    while(tok != NULL)
    {
        if (len > 0)
            text[len++] = ' ';
        tokLen = strlen(tok);
        memcpy(text + len, tok, tokLen);
        len += tokLen;
        tok = strtok(NULL, " \n\t");
    }
    
    if (len <= 0)
    {
        if (!inPlace)
            free(text);
        return NULL;
    }
    
    /* set the terminating char */
    text[len] = 0;
    
    return text;
}
