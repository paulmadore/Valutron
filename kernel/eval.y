%{
#include "values.h"

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
%destructor {
    if ($$ && !dynamic_cast<ScmNull *>($$))
        $$->deleteAll();
    $$ = NULL;
    } <scm>

 //         "'"   "#'"       "`"       "\'"
%token <V> QUOTE SHARPQUOTE BACKQUOTE ESCQUOTE
 //         ","   ",@"
%token <V> COMMA COMMAAT
%token <V> L_BRACKET R_BRACKET
%token <C> STRING_LITERAL IDENTIFIER
%token <I> BOOLE I_LITERAL
%type <scm> atom s_expr s_expr_list list

%start translunit

%{
#ifndef FLEX_SCANNER
typedef struct YYLTYPE YYLTYPE;
#include "scanner.yy.h"
#endif
%}

%%

translunit
    : s_expr  { *evalget_extra(scanner) = $1; }
    | /* ε */ { *evalget_extra(scanner) = NULL; }
    ;

atom
    : STRING_LITERAL
    {
        std::string lit($1);
        lit.erase(lit.find_last_not_of(" \t\v\n\r\f") + 1);
        $$ = new ScmString(lit);
        free ($1);
    }
    | I_LITERAL      { $$ = new ScmInteger($1); }
    | IDENTIFIER     { $$ = new ScmSymbol($1); free ($1); }
    ;

s_expr
    : atom
    | list
    ;

s_expr_list
    : s_expr             { $$ = new ScmCell(ScmValue::Ptr($1), makeScmNil()); }
    | s_expr '.' s_expr  { $$ = new ScmCell($1, $3); }
    | s_expr s_expr_list { $$ = new ScmCell($1, $2); }
    ;

list
    : '(' ')'             { $$ = new ScmNull( ); }
    | '(' s_expr_list ')' { $$ = $2; }
    | QUOTE s_expr
    | ESCQUOTE s_expr
    | SHARPQUOTE s_expr
    ;

%%

#include "scanner.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s);
