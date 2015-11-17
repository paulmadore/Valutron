%{
#include "eval.tab.i"
#include "eval.tab.h"
%}

NUM             ([0-9])
ALPHA           ([a-zA-Z])
ALPHNUM         ({LETTER}|{DIGIT})
WS              ([ \t\v\n\f])
NONBLANK        ([^ {WS}])

%option noyywrap
%option reentrant bison-bridge bison-locations
%option yylineno

%%

%%

#include <stdio.h>
#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    fflush(stdout);
    fprintf(stderr, "%d:%d (%s): %s\n", loc.last_line, loc.last_column, yylval, s);
}