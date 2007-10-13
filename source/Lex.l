%{
#include <stdio.h>
%}

%option stack yylineno
%x comment

%%

"<!--"          {
                    yy_push_state(comment);
                }

<comment>{
"-->"           {
                    yy_pop_state();
                }
.|\n            ECHO;
}

.
%%

