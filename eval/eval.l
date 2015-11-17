%{
#include "eval.tab.h"
%}

NUM             ([0-9])
ALPHA           ([a-zA-Z])
ALPHNUM         ({LETTER}|{DIGIT})
WS              ([ \t\v\n\f])
NONBLANK        ([^ {WS}])

%option noyywrap
%option yylineno

%%