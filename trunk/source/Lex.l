%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Yacc.h"
#include "xtree/Element.h"
#include "xtree/Utils.h"

xtree_Element *gElement;

char* yyStrippedText();

%}

%option stack yylineno
%x startElement
%s endElement
%x attribute

Ws              [ \t\r\n]
Letter          [A-Za-z]
Digit           [0-9]
NameChar        {Letter}|{Digit}|[-._:]
Name            ({Letter}|[_:]){NameChar}*
Comment         "<!--"[^-]*[-]{2,}([^->][^-]*[-]{2,})*">"
CData           "<![CDATA["[^\]]*[\]]{2,}([^\]>][^\]]*[\]]{2,})*">"
CharData        [^<&]*
AttValue        (\"[^<&"]*\")|("'"[^<&']*"'")
CharRef         "&#"[0-9]+";"|"&#x"[0-9a-fA-F]+";"
EntityRef       "&"{Name}";"

%%

"<"{Name}       {
                    yy_push_state(startElement);
                    
                    /* add a child element */
                    gElement = xtree_Element_addChild(gElement,
                        &yytext[1], XTREE_ELEMENT);
                    
                    return START_TAG;
                }

<attribute>{

{Name}{Ws}*[=]    {
                    yyless(yyleng - 1); /* put back the equals sign */
                    /* get the attribute name */
                    /* TODO - strip the spaces from the name */
                    xtree_Element_addAttribute(gElement, yytext, NULL);
                }
[=]{Ws}*        ;   /* ignore */
{AttValue}      {
                    char* value = NULL;
                    xtree_Attribute *attr =
                        (xtree_Attribute*)gElement->attributes->last->data;
                    yy_pop_state();
                    
                    /* set the value of the attribute */
                    value = malloc(yyleng + 1);
                    memset(value, 0, yyleng + 1);
                    memcpy(value, yytext, yyleng);
                    attr->value = value;
                }
}


<startElement>{
{Name}{Ws}*[=]{Ws}*["']    {
                                /* this is an attribute, so let's use those rules */
                                yy_push_state(attribute);
                                yyless(0);  /* put back the entire lexeme */
                           }

"/>"            {
                    yy_pop_state();
                    gElement->isEmpty = 1;
                    /* pop to the parent element */
                    gElement = gElement->parent;
                    
                    return END_EMPTY_TAG;
                }

[>]             {
                    yy_pop_state();
                    yy_push_state(endElement);
                    return *yytext;
                }
                
[ \t\n]         ; /* ignore whitespace */
}



<endElement>{
"</"{Name}">"   {
                    yy_pop_state();
                    /* pop to the parent element */
                    gElement = gElement->parent;
                    
                    return END_TAG;
                }

{CharData}      {
                    /* TODO - do a check here to see if we want to preserve
                     * whitespace. If so, keep it all. Otherwise, strip it,
                     * then see if we even have any text left to add
                     */
                    char* text = xtree_compactWhitespace(yytext, 1);
                    if (text)
                        xtree_Element_addTextChild(gElement, XTREE_TEXT, text);
                }

[ \t\n]         ; /* ignore whitespace */

}

{Comment}       {
                    xtree_Element_addTextChild(gElement, XTREE_COMMENT, yytext);
                    return COMMENT;
                }

{CData}         {
                    xtree_Element_addTextChild(gElement, XTREE_CDATA, yytext);
                    return CDATA;
                }

{CharRef}       {
                    xtree_Element_addTextChild(gElement, XTREE_CHAR_REF, yytext);
                    return REFERENCE;
                }

{EntityRef}     {
                    xtree_Element_addTextChild(gElement, XTREE_ENTITY_REF, yytext);
                    return REFERENCE;
                }

[\n]            ; /* ignore */
.               ; /* ignore ? */
%%
