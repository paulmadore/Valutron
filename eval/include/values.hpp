#ifndef _INTERPRETER_VALUES_H
#define _INTERPRETER_VALUES_H 1

#include <cassert>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

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
    virtual ~ScmValue () {}

    virtual ScmValue * evaluate (class ScmPair * ast) { return this; }
    virtual void print () {}

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
    void print ();
    void print (bool print_first_paren);

    ScmValue * car () { return val.first; }
    ScmValue * cdr () { return val.second; }

    class iterator : public std::iterator<std::forward_iterator_tag, ScmPair *>
    {
        ScmPair * iter;

      public:
        iterator (int loc)
        {
            if (loc < 0)
                while (iter->val.second->tag == PAIR)
                    iter = dynamic_cast<ScmPair *> (iter->val.second);
            else
                for (int i = 0; i < loc; i++)
                {
                    assert (iter->val.second->tag == PAIR);
                    iter = dynamic_cast<ScmPair *> (iter->val.second);
                }
        }
        iterator (ScmPair * loc) : iter (loc) {}
        iterator (const iterator & anIter) : iter (anIter.iter) {}

        iterator & operator++()
        {
            /* If this fails, then you are probably trying to go beyond
             * List boundaries. It fails when cdr of a pair is no longer
             * another pair. */
            iter = dynamic_cast<ScmPair *> (iter->val.second);
            return *this;
        }

        bool operator==(const iterator & rhs) { return iter == rhs.iter; }
        bool operator!=(const iterator & rhs) { return iter != rhs.iter; }
        Cons & operator*() { return iter->val; }
        Cons & operator->() { return iter->val; }
    };

    iterator begin () { return iterator (0); }
    iterator end () { return iterator (-1); }
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

class ScmSymbol : public ScmValue
{
  public:
    std::string val;
    ScmSymbol (std::string str) : ScmValue (SYMBOL), val (str) {}
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
    ScmNull () : ScmPair (NULL, NULL){};
};

extern ScmNull * scmNil;

#endif
