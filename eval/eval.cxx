#include <cstdio>
#include <fstream>
#include <values.hpp>

#include "eval.tab.i"
#include "eval.tab.h"
#include "eval.yy.h"

ScmValue * read (const char * text)
{
    /* The ScmValue that we parse from this text.*/
    ScmValue * result = 0;
    /* Lex requires this scanner structure in order to operate. */
    yyscan_t scanner;
    /* It also requires a suitable buffer-state structure. */
    YY_BUFFER_STATE yybuffer;

    /* The scanner structure needs to be initialised. */
    evallex_init_extra (&result, &scanner);
    /* Now we need to scan our string into the buffer. */
    yybuffer = eval_scan_string (text, scanner);

    try
    {
        yyparse (scanner);
    }
    catch (int)
    {
        printf ("Caught error during read().\n");
    }

    /* Now we can clean up the buffer. */
    eval_delete_buffer (yybuffer, scanner);
    /* And, finally, destroy this scanner. */
    evallex_destroy (scanner);
    return result;
}

int main (int argc, char * argv[])
{
    int returnValue = 0;
    char buffer[256];

    while (returnValue != 99)
    {
        ScmValue * input = 0;
        std::string prompt ("{ 0 } ok ");

        fputs (prompt.c_str (), stdout);
        fflush (stdout);
        if (fgets (buffer, sizeof (buffer), stdin) == NULL)
            break;

        input = read (buffer);
        if (input)
        {
            input->print ();
            putchar ('\n');
        }
    }
}
