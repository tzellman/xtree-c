%{

#include <stdio.h>
void yyerror(char *);
int yylex(void);

%}

%token START_TAG END_EMPTY_TAG END_TAG
%token ATTRIBUTE COMMENT CDATA REFERENCE

%%

document:
        prolog element misc
        ;

prolog:
        misc
        ;

misc:
        COMMENT
        |
        ;

element:
        START_TAG attList elementRemainder
        ;

attList:
        attList ATTRIBUTE
        | ATTRIBUTE
        | /* empty */
        ;

elementRemainder:
        END_EMPTY_TAG
        | '>' contentList END_TAG
        ;

content:
        element
        | COMMENT
        | CDATA
        | REFERENCE
        ;

contentList:
        contentList content
        | content
        |
        ;
    

%%

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}


