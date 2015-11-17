%{
typedef void * yyscan_t;
%}

%pure-parser

%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }

%union {
    long I;
    char *C;
}

%token <C> LBRACKET RBRACKET STRINGLITERAL
%token <I> ILITERAL

%start translunit

%%

translunit
    : LBRACKET/* ε */
    ;

%%

#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s);