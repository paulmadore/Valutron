#ifndef _INTERPRETER_VALUES_H
#define _INTERPRETER_VALUES_H 1

#include <string>
#include <vector>
#include <utility>

class ScmValue {
	public:
		enum tagged_type {
			PAIR,   SYMBOL,  STRING,
			VECTOR, INTEGER, RATIONAL,
			REAL,   BOOLEAN,
		} tag;

		union data {
			std::pair<ScmValue *, ScmValue *> val_pair;
			signed long integer;
			bool boolean;
			std::string str;
			// and more...
		} data;

		 ScmValue( );
		~ScmValue( );

		void set( enum tagged_type tagged, union data new_data );
		void set( signed long n );
		void set( ScmValue val );
		// and so on...
};

#endif
