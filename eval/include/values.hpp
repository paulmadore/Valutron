#ifndef _INTERPRETER_VALUES_H
#define _INTERPRETER_VALUES_H 1

#include <string>
#include <vector>
#include <utility>

enum ScmType
{
    UNDEF,
    PAIR,
    SYMBOL,
    STRING,
    VECTOR,
    INTEGER,
    RATIONAL,
    REAL,
    BOOLEAN,
    SCM_NULL,
};

class ScmValue
{
  public:
    typedef std::pair<ScmValue *, ScmValue *> Cons;
    ScmType tag;

    ScmValue ()
    {
        tag        = UNDEF;
        references = 1;
    };
    ScmValue (ScmType tagged) : tag (tagged) {}
    virtual ~ScmValue (){};

    virtual void print (){};

    ScmValue * incRefs ();
    void decRefs ();
    unsigned getRefs ();

  protected:
    unsigned references;
};

class ScmPair : public ScmValue
{
  public:
    Cons val;
    ScmPair (Cons pair) : ScmValue (PAIR), val (pair) {}
    ScmPair (ScmValue * one, ScmValue * two) : ScmValue (PAIR), val (one, two)
    {
    }
    void print( );
    void print( bool print_first_paren );
};

class ScmInteger : public ScmValue
{
  public:
    long val;
    ScmInteger (long integer) : ScmValue (INTEGER), val (integer) {}
    void print ();
};

class ScmString : public ScmValue
{
  public:
    std::string val;
    ScmString (std::string str) : ScmValue (STRING), val (str) {}
    void print ();
};

class ScmBoole : public ScmValue
{
  public:
    bool val;
    ScmBoole (bool boole) : ScmValue (BOOLEAN), val (boole) {}
    void print ();
};

class ScmNull : public ScmPair
{
  public:
    ScmNull() : ScmPair(NULL, NULL){ };
};

#endif
