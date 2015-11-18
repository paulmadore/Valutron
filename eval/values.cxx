#include <values.hpp>

ScmValue::~ScmValue( ){
	switch( tag ){
		case PAIR:
			delete data.valpair;
			break;

		case SYMBOL:
		case STRING:
			delete data.str;
			break;

		default:
			break;
	}
}

// A variety of setters for your convenience, using overloading
// to determine the proper tag value.
//void ScmValue::set( std::pair<ScmValue, ScmValue> *vals ){
void ScmValue::set( Cons *vals ){
	tag = PAIR;
	data.valpair = vals;
}

void ScmValue::set( std::string *str ){
	tag = STRING;
	data.str = str;
}

void ScmValue::set( enum ScmType tagged, std::string *str ){
	tag = tagged;
	data.str = str;
}

void ScmValue::set( long n ){
	tag = INTEGER;
	data.integer = n;
}

void ScmValue::set( double n ){
	tag = REAL;
	data.real = n;
}

void ScmValue::set( bool boolean ){
	tag = BOOLEAN;
	data.boolean = boolean;
}

void ScmValue::set( ScmValue val ){
	tag  = val.tag;
	data = val.data;
}

void ScmValue::set( enum  ScmType tagged,
                    union ScmValue::Data   new_data ){
    tag  = tagged;
    data = new_data;
}

void ScmValue::print()
{
    switch (tag)
    {
    case STRING:
        printf("\"%s\"", data.str->c_str());
        break;
    case INTEGER:
        printf("%ld", data.integer);
        break;
    case BOOLEAN:
        printf("#%c", data.boolean ? 't' : 'f');
        break;
    case PAIR:
    {
        ScmValue * dat = this;

        printf ("(");
        while (true)
        {
            dat->data.valpair->first.print();
            if (dat->data.valpair->second.tag == UNDEF)
            {
                printf(")");
                break;
            }
            dat = &dat->data.valpair->second;
            if (dat->tag != PAIR)
            {
                printf(" . ");
                dat->print();
                printf(")");
                break;
            }
            printf(" ");
        }

        break;
    }
    default:;
    }
}


// reference counting functions

// incRefs returns a pointer to the current class to make reasoning about
// reference incrementing on assignment a bit easier.
// e.g:
//   ScmValue *some_reference = another_value.incRefs( );
ScmValue *ScmValue::incRefs( ){
    references++;
    return this;
}

void ScmValue::decRefs( ){
    references--;
}

unsigned ScmValue::getRefs( ){
    return references;
}
