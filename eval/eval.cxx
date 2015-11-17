#include <stdio.h>

#include "eval.tab.i"
#include "eval.tab.h"
#include "eval.yy.h"

int main (int argc, char * argv[])
{
    yyscan_t scanner;
    FILE * in = fopen (argv[1], "r");

    if (evallex_init (&scanner))
        exit (1);

    yyparse (scanner);
}