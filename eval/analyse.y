%start translunit

%%

translunit
    : /* ε */
    ;

%%

#include <stdio.h>

int main(int argc, char * argv[])
{
    evalin = fopen(argv[1], "r");
    yyparse();
}
