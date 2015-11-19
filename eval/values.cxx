#include <values.hpp>
#include <cstdio>

void ScmString::print () { printf ("%s", val.c_str ()); }

void ScmSymbol::print () { printf ("%s", val.c_str ()); }

void ScmInteger::print () { printf ("%ld", val); }

void ScmBoole::print () { printf ("#%c", val ? 't' : 'f'); }

// void ScmNull::print () { printf( "()" ); }

void ScmPair::print (bool print_first_paren)
{
    if (print_first_paren)
    {
        putchar ('(');
    }

    if (val.first)
    {
        val.first->print ();
    }

    if (val.second)
    {
        if (val.second->tag == PAIR)
        {
            ScmPair * dat = dynamic_cast<ScmPair *> (val.second);

            // Check to see if it's a null pair
            if (dat->val.first && dat->val.second)
            {
                // if it's not, print the next pair
                putchar (' ');
                dynamic_cast<ScmPair *> (val.second)->print (false);
            }
            else
            {
                // And stop printing if it is
                putchar (')');
            }
        }
        else
        {
            printf (" . ");
            val.second->print ();
            putchar (')');
        }
    }
    else
    {
        putchar (')');
    }
}

void ScmPair::print () { print (true); }

// reference counting functions

// incRefs returns a pointer to the current class to make reasoning about
// reference incrementing on assignment a bit easier.
// e.g:
//   ScmValue *some_reference = another_value.incRefs( );
ScmValue * ScmValue::incRefs ()
{
    references++;
    return this;
}

void ScmValue::decRefs () { references--; }

unsigned ScmValue::getRefs () { return references; }
