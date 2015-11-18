#ifndef _INTERPRETER_VALUES_H
#define _INTERPRETER_VALUES_H 1

#include <string>
#include <vector>
#include <utility>

class ScmValue {
	public:
		typedef std::pair<ScmValue, ScmValue> Cons;

		enum Tagged {
			UNDEF,   PAIR,    SYMBOL,   STRING,
			VECTOR,  INTEGER, RATIONAL, REAL,
			BOOLEAN,
		} tag;

		union Data {
			Cons *valpair;
			long integer;
			double real;
			bool boolean;
			std::string *str;
			// and more...
		} data;

		 ScmValue( )         { tag = UNDEF; references = 1; };
		 ScmValue( auto val ){ references = 1; set( val ); }
		~ScmValue( );

		void set( Cons *vals );
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
