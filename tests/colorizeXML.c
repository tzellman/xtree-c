#include <stdio.h>
#include "Yacc.h"
#include "xtree/Element.h"

#include "xtree/System.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* global gElement used in Yacc.y */
extern xtree_Element *gElement;


void iterateTree(xtree_Element *elem, int depth)
{
    int i;
    
    if (elem->elemType != XTREE_ROOT)
    {
        for(i = 0; i < depth; ++i)
            printf("\t");
    }
    
    /* if a comment or text, just output the data */
    if ((elem->elemType == XTREE_TEXT ||
        elem->elemType == XTREE_COMMENT) && elem->data)
    {
        printf("%s\n", elem->data);
    }
    else
    {
        if (elem->tag && elem->elemType != XTREE_ROOT)
        {
            xtree_ListNode *attrNode = NULL;
            printf("<%s", elem->tag);
            attrNode = elem->attributes ? elem->attributes->first : NULL;
            while(attrNode)
            {
                xtree_Attribute *attr = (xtree_Attribute*)attrNode->data;
                printf(" %s=%s", attr->name, attr->value);
                attrNode = attrNode->next;
            }
            if (!elem->isEmpty)
            {
                printf(">");
                if (elem->children)
                    printf("\n");
            }
        }
        else if (elem->elemType == XTREE_ROOT)
            depth--; /* update the depth if root, so we aren't starting off tabbed */
    
        if (elem->children && !elem->isEmpty)
        {
            xtree_ListNode *iter = elem->children->first;
            while(iter)
            {
                iterateTree((xtree_Element*)iter->data, depth + 1);
                iter = iter->next;
            }
        }
        if (elem->tag && elem->elemType != XTREE_ROOT)
        {
            if (elem->isEmpty)
                printf("/>\n");
            else
            {
                for(i = 0; i < depth && elem->children; ++i)
                    printf("\t");
                printf("</%s>\n", elem->tag);
            }
        }
    }
}

/*
     Convention: program will look for the first line
         starting with an empty comment to use as the starting point
         for inserting the generated comment
 */

#define BUF_SIZE 4096

char** readTemplateFileIntoBuffers(char* path, Sys_Error* err)
{
    char* buf[BUF_SIZE];                /* buffer for the file input*/
    FILE* tfile;                        /* template file ptr */
    char* buffers[2];                   /* return buffers */
    ssize_t nBytesRead;                 /* num bytes read */
    int fd = -1;                        /* file descriptor */

    memset(buf, 0, BUF_SIZE);
 
    fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        /* TODO: use the errno in this */
        SYS_ERROR(err, "Couldn't open template file for reading");
        goto ERR_EXIT;
    }

    while ( (nBytesRead = read(fd, (void*)buf, BUF_SIZE)) > 0  )
    {
        /* do something w/ this */
        printf("%s\n", buf);
    }
    

ERR_EXIT:
    if (fd != -1) close(fd);
    return buffers;

} 


int main(int argc, char **argv)
{
    /* buffers for holding the begin and end of
     * generated document
     */
    char* buffers[2];
    Sys_Error err;
    if (argc != 2)
    {
        printf("usage: %s: [html template file]");
        exit(EXIT_SUCCESS);
    }

    /*Sys_Error_init(&err);*/
    readTemplateFileIntoBuffers(argv[1], &err);

    /*
    gElement = xtree_Element_construct(NULL, NULL, XTREE_ROOT);
    yyparse();
    iterateTree(gElement, 0);
    */
    
    return 0;
}
