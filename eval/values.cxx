#include <values.hpp>
#include <cstdio>

void ScmString::print () { printf ("\"%s\"", val.c_str ()); }

void ScmInteger::print () { printf ("%ld", val); }

void ScmBoole::print () { printf ("#%c", val ? 't' : 'f'); }

void ScmPair::print( bool print_first_paren )
{
    if ( print_first_paren ){
        putchar( '(' );
    }

    if ( val.first ){
        val.first->print( );

    } else {
        // probably want to throw an error or something here,
        // this shouldn't happen
        ;
    }

    if ( val.second || val.second->tag == SCM_NULL ){
        if ( val.second->tag == PAIR ){
            putchar( ' ' );
            dynamic_cast<ScmPair *>(val.second)->print( false );

        } else {
            printf( " . " );
            val.second->print( );
            putchar( ')' );
        }

    } else {
        putchar( ')' );
    }
}

void ScmPair::print( ){
    print( true );
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
