
%{
    #include <stdio.h>
    void yyerror(char *);
    int yylex(void);

    int sym[26];
%}

%token NAME         /* names */
%token S            /* space */
%token ATTVALUE     /* attribute value */
%token content      /* attribute value */


%%

    /*
        [39] element ::= EmptyElemTag | STag content ETag 
    */
element:
        EmptyElemTag
        | STag content ETag
        ;

    /*

        [40] STag ::= '<' Name (S Attribute)* S? '>'
    */
STag:
        '<' NAME _attlist OpWs '>'
        ;
        /* subsidiary attribute matcher */
_attlist:
        S Attribute _attlist
        | /* NULL */
        ;

    /* See regex */
/*
Name:
        NAME
        ;
*/

    /*
        [41] Attribute ::= Name Eq AttValue 
    */
Attribute:
            NAME Eq ATTVALUE

/*
AttValue:
         ATTVALUE;
         ;
*/

    /*
        [25] Eq ::= S? '=' S?
    */
Eq:
        OpWs '=' OpWs
        ;

OpWs:
    S
    |
    ;


    /*
        [42] ETag ::= '</' Name S? '>'
    */

ETag:
        '<' '/' NAME  OpWs '>'
        ;

    /*
        [44] EmptyElemTag ::= '<' Name (S Attribute)* S? '/>'
    */
EmptyElemTag:
        '<' NAME _attlist OpWs '/' '>'


%%

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}


