#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Yacc.h"
#include "xtree/Element.h"
#include "xtree/Error.h"


/* global gElement used in Yacc.y */
extern xtree_Element *gElement;

void escapePrint(char c)
{
    switch(c)
    {
    case '<':
        printf("&lt;");
        break;
    case '>':
        printf("&gt;");
        break;
    case '\n':
        printf("<br />\n");
        break;
    default:
        printf("%c", c);
    }
}

printEscapedStr(const char* str)
{
    int i;
    for (i = 0 ; i < strlen(str); ++i)
        escapePrint(str[i]);
}

int hasNonTextChild(xtree_Element *elem)
{
    /* TODO: implement this */
    return 0;
}

void iterateTree(xtree_Element *elem, int depth)
{
    int i;
    
    if (elem->elemType != XTREE_ROOT)
    {
        for(i = 0; i < depth; ++i)
            printf("\t");
    }
    
    /* if a comment or text, just output the data */
    if (elem->elemType == XTREE_TEXT
         && elem->data)
    {
        /*printf("%s\n", elem->data);*/
        printf("<div class=\"text\">");
        printEscapedStr(elem->data);
        printf("</div>");
    }
    else if (elem->elemType == XTREE_COMMENT && elem->data)
    {
        printf("<div class=\"comment\">\n");
        printEscapedStr(elem->data);
        printf("</div>\n");
    }
    else
    {
        if (elem->tag && elem->elemType != XTREE_ROOT)
        {
            xtree_ListNode *attrNode = NULL;
            /*printf("<%s", elem->tag);*/
            printf("<div>\n");
            printEscapedStr("<");
            printf("<span class=\"tagname\">");
            printEscapedStr(elem->tag);
            printf("</span>");
            attrNode = elem->attributes ? elem->attributes->first : NULL;
            while(attrNode)
            {
                xtree_Attribute *attr = (xtree_Attribute*)attrNode->data;
                printf(" <span class=\"attrKey\">%s</span>=<span class=\"attrVal\">%s</span> ", attr->name, attr->value);
                attrNode = attrNode->next;
            }
            if (!elem->isEmpty)
            {
                printEscapedStr(">");
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
                printEscapedStr("/>\n");
            else
            {
                for(i = 0; i < depth && elem->children; ++i)
                    printf("\t");
                printEscapedStr("</");
                printf("<span class=\"tagname\">");
                printEscapedStr(elem->tag);
                printf("</span>");
                printEscapedStr(">\n");
            }
            printf("</div>\n");
        }
    }
}

/*
     Convention: program will look for the first line
         that contains only whitespace to use as the starting point
         for inserting the generated content
 */

#define BUF_SIZE 4096

void readTemplateFileIntoBuffers(char** buffers, char* path, xtree_Error* err)
{
    char buf[BUF_SIZE];                 /* buffer for the file input*/
    ssize_t nBytesRead;                 /* num bytes read */
    int fd = -1;                        /* file descriptor */
    int i = 0;                          /* iterator */

    memset(buf, 0, BUF_SIZE);
 
    fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        /* TODO: use the errno in this */
        XTREE_ERROR(err, "Couldn't open template file for reading");
        goto FUNC_EXIT;
    }

    /* TODO: make this more robust (allow larger templates */
    nBytesRead = read(fd, (void*)buf, BUF_SIZE);
    
    
    for (i = 0; i < nBytesRead; ++i)
        if (buf[i] == '\n' && buf[i+1] == '\n')
            break;

    buffers[0] = calloc(i+1, sizeof(char));
    buffers[1] = calloc(nBytesRead-i+1, sizeof(char));
    if (buffers[0] == NULL || buffers[1] == NULL)
    {
        XTREE_ERROR(err, "couldn't allocate space for templates");
        goto FUNC_EXIT;
    }
    strncpy(buffers[0], buf, i);
    strncpy(buffers[1], &buf[i], nBytesRead-i);

FUNC_EXIT:
    if (fd != -1) close(fd);

} 


int main(int argc, char **argv)
{
    /* buffers for holding the begin and end of
     * generated document
     */
    char* buffers[2];
    xtree_Error err;
    if (argc != 2)
    {
        printf("usage: %s: [html template file]\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    xtree_Error_init(&err);
    readTemplateFileIntoBuffers(buffers, argv[1], &err);

    gElement = xtree_Element_construct(NULL, NULL, XTREE_ROOT);
    yyparse();

    fprintf(stdout, "%s\n", buffers[0]);
    iterateTree(gElement, 0);
    fprintf(stdout, "%s\n", buffers[1]);
    
    return 0;
}

