#include <stdio.h>
#include "xtree/Element.h"

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
            printf(">");
        }
        else if (elem->elemType == XTREE_ROOT)
            depth--; /* update the depth if root, so we aren't starting off tabbed */
    
        if (elem->children)
        {
            xtree_ListNode *iter = elem->children->first;
            printf("\n");
            while(iter)
            {
                iterateTree((xtree_Element*)iter->data, depth + 1);
                iter = iter->next;
            }
        }
        if (elem->tag && elem->elemType != XTREE_ROOT)
        {
            for(i = 0; i < depth && elem->children; ++i)
                printf("\t");
            printf("</%s>\n", elem->tag);
        }
    }
}


int main(int argc, char **argv)
{
    xtree_Element *root, *elem1 = NULL, *elem2 = NULL, *elem3 = NULL,
        *elem4 = NULL, *elem5 = NULL;
    
    /* make the root element */
    root = xtree_Element_construct(NULL, NULL, XTREE_ROOT);
    
    /* add some elements to the tree */
    elem1 = xtree_Element_addChild(root, "html", XTREE_ELEMENT);
    elem2 = xtree_Element_addChild(elem1, "body", XTREE_ELEMENT);
    elem3 = xtree_Element_addChild(elem2, "div", XTREE_ELEMENT);
    xtree_Element_addTextChild(elem2, "tail text");
    xtree_Element_addCommentChild(elem2, "<!-- a comment -->");
    elem4 = xtree_Element_addChild(elem2, "span", XTREE_ELEMENT);
    xtree_Element_addTextChild(elem4, "some text");
    xtree_Element_addAttribute(elem4, "class", "\"spanClass\"");
    
    /* now, add a head, later on */
    elem5 = xtree_Element_construct(elem1, "head", XTREE_ELEMENT);
    xtree_List_pushFront(elem1->children, elem5);
    
    iterateTree(root, 0);
    
    xtree_Element_destruct(&root); /* this will delete them all, recursively */
    return 0;
}
