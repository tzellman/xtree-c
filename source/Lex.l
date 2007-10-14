%{
#include <stdio.h>
#include "Yacc.h"
#include "xtree/Element.h"

xtree_Element *gElement;
%}

%option stack yylineno
%x comment
%s element

Letter          [A-Za-z]
Digit           [0-9]
NameChar        {Letter}|{Digit}|[-._:]
Name            ({Letter}|[_:]){NameChar}*

%%

"<"{Name}       {
                    yy_push_state(element);
                    
                    /* add a child element */
                    gElement = xtree_Element_addChild(gElement,
                        &yytext[1], XTREE_ELEMENT);
                    
                    printf("start tag: %s\n", &yytext[1]);
                    return START_TAG;
                }
                
<element>{

{Name}\s*[=]\"\s*{Name}\"   {
                                /* TODO - fix this regex -- use a state */
                                /* TODO - manipulate yylval */
                                return ATTRIBUTE;
                            }
"/>"            {
                    yy_pop_state();
                    gElement->isEmpty = 1;
                    /* pop to the parent element */
                    gElement = gElement->parent;
                    
                    printf("end empty tag\n");
                    return END_EMPTY_TAG;
                }

"</"{Name}">"   {
                    yy_pop_state();
                    /* pop to the parent element */
                    gElement = gElement->parent;
                    
                    printf("end tag: %s\n", &yytext[2]);
                    return END_TAG;
                }

[>=]            return *yytext;
[ \t\n]+        ; /* ignore whitespace */
}


%%



/********************************************
<a><b></b><c/></a>


<comment>{
"-->"           {
                    yy_pop_state();
                }
.|\n            ECHO;
}

[ \t\n]+        return WHITESPACE;

********************************************/