%{
#include "values.hpp"

typedef void * yyscan_t;
%}

%pure-parser

%lex-param      { yyscan_t scanner }
%parse-param    { yyscan_t scanner }

%union {
    char * C;
    long I;
    void * V;
    ScmValue * scm;
    ScmValue::Cons * cons;
}

%destructor { if ($$) { free ($$); $$ = NULL; } } <C>

 //         "'"   "#'"       "`"       "\'"
%token <V> QUOTE SHARPQUOTE BACKQUOTE ESCQUOTE
 //         ","   ",@"
%token <V> COMMA COMMAAT
%token <V> L_BRACKET R_BRACKET
%token <C> STRING_LITERAL IDENTIFIER
%token <I> BOOLE I_LITERAL
%type <scm> atom s_expr s_expr_list list

%start translunit

%%

translunit
    : s_expr_list { $1->print(); }
    | /* ε */
    ;

atom
    : STRING_LITERAL { $$ = new ScmString($1); }
    | I_LITERAL      { $$ = new ScmInteger($1); }
    | IDENTIFIER     { $$ = new ScmString($1); } /* handle identifier differently? */
    ;

s_expr
    : atom
    | list
    ;

s_expr_list
    : s_expr             { $$ = new ScmPair($1, new ScmNull( )); }
    | s_expr '.' s_expr  { $$ = new ScmPair($1, $3); }
    | s_expr s_expr_list { $$ = new ScmPair($1, $2); }
    ;

list
    : '(' ')'             { $$ = new ScmNull( ); }
    | '(' s_expr_list ')' { $$ = $2; }
    | QUOTE s_expr
    | ESCQUOTE s_expr
    | SHARPQUOTE s_expr
    ;

%%

#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s);
