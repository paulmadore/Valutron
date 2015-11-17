%{
typedef void * yyscan_t;
%}

%pure-parser

%lex-param      { yyscan_t scanner }
%parse-param    { yyscan_t scanner }

%union {
    char * C;
    long I;
    void * V;
}

%destructor { if ($$) { free ($$); $$ = NULL; } } <C>

 //         "'"   "#'"       "`"       "\'"
%token <V> QUOTE SHARPQUOTE BACKQUOTE ESCQUOTE
 //         ","   ",@"
%token <V> COMMA COMMAAT
%token <V> L_BRACKET R_BRACKET
%token <C> STRING_LITERAL IDENTIFIER
%token <I> BOOLE I_LITERAL

%start translunit

%%

translunit
    : s_expr
    | /* ε */
    ;

atom
    : STRING_LITERAL
    | I_LITERAL
    | IDENTIFIER
    ;

s_expr
    : atom
    | list
    ;

s_expr_list
    : s_expr
    | s_expr_list s_expr
    ;

list
    : '(' ')'
    | '(' s_expr_list ')'
    | '(' s_expr_list '.' s_expr ')'
    | QUOTE s_expr
    | ESCQUOTE s_expr
    | SHARPQUOTE s_expr
    ;

%%

#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s);