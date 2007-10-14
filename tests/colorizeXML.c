#include <stdio.h>
#include "Yacc.h"
#include "xtree/Element.h"

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


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: %s: [html template file]");
        exit(EXIT_SUCCESS);
    }

    gElement = xtree_Element_construct(NULL, NULL, XTREE_ROOT);
    yyparse();
    iterateTree(gElement, 0);
    
    return 0;
}
