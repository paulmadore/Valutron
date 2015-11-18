#include <values.hpp>

void ScmString::print () { printf ("\"%s\"", val.c_str ()); }

void ScmInteger::print () { printf ("%ld", val); }

void ScmBoole::print () { printf ("#%c", val ? 't' : 'f'); }

void ScmPair::print ()
{
    ScmValue * dat = this;

    printf ("(");
    while (true)
    {
        dynamic_cast<ScmPair *> (dat)->val.first->print ();
        if (dynamic_cast<ScmPair *> (dat)->val.second->tag == UNDEF)
        {
            printf (")");
            break;
        }
        dat = dynamic_cast<ScmPair *> (dat)->val.second;
        if (dat->tag != PAIR)
        {
            printf (" . ");
            dat->print ();
            printf (")");
            break;
        }
        printf (" ");
    }
}

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
