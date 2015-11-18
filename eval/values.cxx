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

void ScmValue::set( enum ScmValue::Tagged tagged, std::string *str ){
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

void ScmValue::set( enum  ScmValue::Tagged tagged,
                    union ScmValue::Data   new_data ){
    tag  = tagged;
    data = new_data;
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
