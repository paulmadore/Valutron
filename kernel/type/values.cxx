#include <cstdio>

#include "values.h"

static ScmNull scmNil;

ScmValue::Ptr makeScmNil ()
{
    return ScmValue::Ptr (ScmValue::Ptr (), &scmNil);
}

void ScmString::print () { printf ("%s", val.c_str ()); }

void ScmSymbol::print () { printf ("%s", val.c_str ()); }

void ScmInteger::print () { printf ("%ld", val); }

void ScmBoole::print () { printf ("#%c", val ? 't' : 'f'); }

void ScmCell::print (bool print_first_paren)
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
        if (val.second->tag == CELL)
        {
            CellPtr dat = AsPtr (Cell, val.second);

            // Check to see if it's a null pair
            if (dat->val.first && dat->val.second)
            {
                // if it's not, print the next pair
                putchar (' ');
                AsPtr (Cell, val.second)->print (false);
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

void ScmCell::print () { print (true); }