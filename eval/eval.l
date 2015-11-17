%{
#include <cstring>
#include "eval.tab.i"
#include "eval.tab.h"
%}

ES          (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS          [ \t\v\n\r\f]+
NUM         [0-9]
INT10       ("-"?){NUM}+
LETTER      [a-zA-Z]|"!"|"$"|"%"|"&"|"*"|"+"|"-"|"."|"/"|":"|"<"|"="|">"|"?"|"@"|"^"|"_"|"~"
IDENTIFIER  {LETTER}({NUM}|{LETTER})*

%option noyywrap
%option reentrant bison-bridge bison-locations
%option yylineno

%{

#define YY_STORE_INT(x) yylval->I = x;
#define YY_STORE_STR() yylval->C = strdup(yytext);

#define YY_USER_INIT \
    yylloc->last_line = 0; \
    yylloc->last_column = 0;

#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    \
    for(int i = 0; yytext[i] != '\0'; i++) \
    { \
        if(yytext[i] == '\n') \
        { \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else \
            yylloc->last_column++; \
    }
%}

%%

(\"([^"\\\n]|{ES})*\"{WS}*)+        { YY_STORE_STR(); return STRING_LITERAL; }

"#t"                                { YY_STORE_INT(1); return BOOLE; }
"#f"                                { YY_STORE_INT(0); return BOOLE; }

{INT10}                             {
                                        YY_STORE_INT(strtol(yytext, 0, 10));
                                        return I_LITERAL;
                                    }

{IDENTIFIER}                        { YY_STORE_STR(); return IDENTIFIER; }

{WS}*                               { ; }
.                                   { printf("<%s>\n", yytext); YY_FATAL_ERROR("Unrecognised token"); }

%%

#include <stdio.h>
#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    fflush(stdout);
    fprintf(stderr, "%d:%d (%s): %s\n", loc.last_line, loc.last_column, yytext, s);
}