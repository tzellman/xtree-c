#include <stdio.h>
#include "Yacc.h"

int tokens[] = {START_TAG, '>', END_TAG, EOF};
int index;

int yylex()
{
    return tokens[index++];
}

int main(int argc, char **argv)
{
    index = 0;
    yyparse();
    return 0;
}
