#include <cstdio>
#include <fstream>
#include <values.hpp>

#include "eval.tab.i"
#include "eval.tab.h"
#include "eval.yy.h"

int read (const char * text)
{
    /* Lex requires this scanner structure in order to operate. */
    yyscan_t scanner;
    /* It also requires a suitable buffer-state structure. */
    YY_BUFFER_STATE yybuffer;

    /* The scanner structure needs to be initialised. */
    evallex_init (&scanner);
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
    return 0;
}

int main (int argc, char * argv[])
{
    int returnValue = 0;
    char buffer[256];

    // This is just an example of the value class, delete this when it's not needed
    ScmValue *foo = new ScmValue( new ScmValue::Cons( true, new std::string( "asdf" )));
    printf( "(#%c . \"%s\")\n",
        (char[2]){'f', 't'}[foo->data.valpair->first.data.boolean],
        foo->data.valpair->second.data.str->c_str( ) );
    delete foo;

    while (returnValue != 99)
    {
        std::string prompt ("{ 0 } ok ");
        fputs (prompt.c_str (), stdout);
        fflush (stdout);
        if (fgets (buffer, sizeof (buffer), stdin) == NULL)
            break;

        read (buffer);
    }
}
