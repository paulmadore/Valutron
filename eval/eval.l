%{
#include <cstring>
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

%{

#define YY_STORE_STR() yylval->C = strdup(yytext);

#define YY_USER_ACTION { \
    int curr_line = 1; \
    \
    yylloc->first_line = yylineno; \
    yylloc->first_column = yycolumn; \
    \
    for (char * s = yytext; *s != '\0'; s++) \
    { \
        if(*s == '\n') \
        { \
            curr_line++; yylineno++; \
            yycolumn = 1; \
        } \
        else yycolumn++; \
    } \
    yylloc->last_line = yylineno; \
    yylloc->last_column = yycolumn- 1; \
    }
%}

%%

%%

#include <stdio.h>
#include "eval.yy.h"

void evalerror(YYLTYPE loc, yyscan_t yyscanner, const char *s)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    fflush(stdout);
    fprintf(stderr, "%d:%d (%s): %s\n", loc.last_line, loc.last_column, yylval->C, s);
}