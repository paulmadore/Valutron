#include <stdio.h>

#include "eval.yy.h"
#include "eval.tab.i"
#include "eval.tab.h"

int main (int argc, char * argv[])
{
    evalin = fopen (argv[1], "r");
    evalparse ();
}