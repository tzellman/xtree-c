%{
#include <stdio.h>
%}

%x element
%%

"<"                     BEGIN(element);     /* start of element */
"/>"                    BEGIN(INITIAL);     /* end of element */

%%

