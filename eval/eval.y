%{
#include "eval.yy.h"
%}

%start translunit

%%

translunit
    : /* ε */
    ;

%%

#include <stdio.h>

void evalerror(YYLTYPE loc, const char *s)
{
    fflush(stdout);
    fprintf(stderr, "%d:%d (%s): %s\n", evallineno, loc.last_column, yylval, s);
}