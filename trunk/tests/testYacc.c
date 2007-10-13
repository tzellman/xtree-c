#include <stdio.h>
#include "Yacc.h"

int tokens[] = {'<', NAME, '>', content, '<', '/', NAME, '>', EOF};
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
