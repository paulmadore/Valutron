%{
#include <cstring>
#include "eval.tab.i"
#include "eval.tab.h"
%}

NUM             ([0-9])
ALPHA           ([a-zA-Z])
ALPHNUM         ({LETTER}|{DIGIT})
WS              ([ \t\v\n\f])
NONBLANK        ([^ {WS}]*)

%option noyywrap
%option reentrant bison-bridge bison-locations
%option yylineno

%{

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

(\"([^"\\\n]|{NONBLANK})*\"{WS}*)+	{ YY_STORE_STR(); return STRING_LITERAL; }


%%

#include <stdio.h>
#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    fflush(stdout);
    fprintf(stderr, "%d:%d (%s): %s\n", loc.last_line, loc.last_column, yylval->C, s);
}