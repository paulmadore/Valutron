#ifndef _INTERPRETER_VALUES_H
#define _INTERPRETER_VALUES_H 1

#include <string>
#include <vector>
#include <utility>

class ScmValue {
	public:
		enum Tagged {
			PAIR,   SYMBOL,  STRING,
			VECTOR, INTEGER, RATIONAL,
			REAL,   BOOLEAN,
		} tag;

		union Data {
			std::pair<ScmValue, ScmValue> *valpair;
			long integer;
			double real;
			bool boolean;
			std::string *str;
			// and more...
		} data;

		 ScmValue( );
		~ScmValue( );

		void set( std::string *str );
		void set( enum Tagged tagged, std::string *str );
		void set( long n );
		void set( double r );
		void set( ScmValue val );
		void set( bool boolean );
		void set( enum Tagged tagged, union Data new_data );
		// and so on...

		ScmValue *incRefs( );
		void     decRefs( );
		unsigned getRefs( );

	private:
		unsigned references;
};

#endif
