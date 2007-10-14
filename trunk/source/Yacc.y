%{

#include <stdio.h>
void yyerror(char *);
int yylex(void);

%}

%token START_TAG END_EMPTY_TAG END_TAG
%token ATTRIBUTE

%%

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
        | '>' content END_TAG
        ;

content:
        content element
        | element
        |
        ;

%%

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}


