%{
typedef void * yyscan_t;
%}

%pure-parser

%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }

%union {
    char * C;
    long I;
    void * V;
}

%token <V> L_BRACKET R_BRACKET
%token <C> STRING_LITERAL IDENTIFIER
%token <I> BOOLE I_LITERAL

%destructor { if ($$) { free ($$); $$ = NULL; } } <C>

%start translunit

%%

translunit
    : STRING_LITERAL  'f'/* ε */
    ;

%%

#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s);