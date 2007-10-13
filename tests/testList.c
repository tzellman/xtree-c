#include <stdio.h>
#include "xtree/Element.h"

void iterateTree(xtree_List* tree, int depth)
{
    xtree_ListNode *node = tree->first;
    while(node)
    {
        int i;
        xtree_ListNode *attrNode = NULL;
        xtree_Element *data = (xtree_Element*)node->data;
        
        for(i = 0; i < depth; ++i)
            printf("\t");
        
        printf("<%s", data->tag);
        attrNode = data->attributes ? data->attributes->first : NULL;
        while(attrNode)
        {
            xtree_Attribute *attr = (xtree_Attribute*)attrNode->data;
            printf(" %s=%s", attr->name, attr->value);
            attrNode = attrNode->next;
        }
        printf(">");
        
        if (data->text)
            printf("%s", data->text);
        
        if (data->children)
        {
            printf("\n");
            iterateTree(data->children, depth + 1);
            for(i = 0; i < depth; ++i)
                printf("\t");
        }
        printf("</%s>", data->tag);
        
        if (data->tail)
            printf("%s", data->tail);
        
        printf("\n");
        node = node->next;
    }
}


int main(int argc, char **argv)
{
    xtree_List *tree = NULL;
    xtree_Element *elem1 = NULL, *elem2 = NULL, *elem3 = NULL, *elem4 = NULL, *elem5 = NULL;
    
    tree = xtree_List_construct();
    if (!tree)
    {
        fprintf(stderr, "NULL list\n");
        exit(EXIT_FAILURE);
    }
    
    elem1 = xtree_Element_construct(NULL, "html");
    xtree_List_pushBack(tree, elem1);
    
    elem2 = xtree_Element_addChild(elem1, "body");
    elem3 = xtree_Element_addChild(elem2, "div");
    xtree_Element_setTail(elem3, "tail text");
    elem4 = xtree_Element_addChild(elem2, "span");
    
    xtree_Element_setText(elem4, "some text");
    
    xtree_Element_addAttribute(elem4, "class", "\"spanClass\"");
    
    /* now, add a head, later on */
    elem5 = xtree_Element_construct(elem1, "head");
    xtree_List_pushFront(elem1->children, elem5);
    
    iterateTree(tree, 0);
    
    xtree_Element_destruct(&elem1); /* this will delete them all, recursively */
    
    xtree_List_destruct(&tree);
    return 0;
}
