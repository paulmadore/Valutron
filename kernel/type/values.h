/* Valutron
 * Types declarations
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#ifndef INTERPRETER_VALUES_H_
#define INTERPRETER_VALUES_H_

#include <cassert>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define AsPtr(type, x) std::dynamic_pointer_cast<Scm##type> (x)

typedef std::shared_ptr<class VmOperation> VmOpPtr;
typedef std::vector<VmOpPtr> VmOpVec;

enum ScmType
{
    UNDEF,
    CELL,
    SYMBOL,
    STRING,
    VECTOR,
    INTEGER,
    RATIONAL,
    REAL,
    BOOLEAN,
    FUNCTION,
    SCM_NULL,
};

class ScmValue : public std::enable_shared_from_this<ScmValue>
{
  public:
    typedef std::shared_ptr<ScmValue> Ptr, ValPtr;
    typedef std::pair<Ptr, Ptr> Cons;
    typedef std::map<std::string, Ptr> Map;

    ScmType tag;

    ScmValue () { ScmValue (UNDEF); }
    ScmValue (ScmType tagged) : tag (tagged) {}
    virtual ~ScmValue (){};

    virtual bool operator==(const ScmValue & rhs) { return false; }
    virtual bool operator!=(const ScmValue & rhs) { return true; }

    /* `compile` will emplace VmOperations on the specified vector.
     * This will typically be the code vector of the current lambda
     * being compiled. */
    virtual void compile (VmOpVec & vec);

    virtual void print (){};
    virtual void deleteAll () { delete this; }
};

class ScmCell : public ScmValue
{
  public:
    typedef std::shared_ptr<ScmCell> CellPtr;
    Cons val;

    ScmCell (Cons pair) : ScmValue (CELL), val (pair) {}
    ScmCell (ScmValue * one, ScmValue * two)
        : ScmValue (CELL), val (Ptr (one), Ptr (two))
    {
    }
    ScmCell (Ptr one, Ptr two) : ScmValue (CELL), val (Ptr (one), Ptr (two)) {}

    bool operator==(const ScmCell & rhs)
    {
        return val.first == rhs.val.first && val.second == rhs.val.second;
    }
    bool operator!=(const ScmCell & rhs) { return !(*this == rhs); }

    void compile (VmOpVec & vec);

    void print ();
    void print (bool print_first_paren);
    void deleteAll ()
    {
        if (val.first->tag == CELL)
            val.first->deleteAll ();
        if (val.second->tag == CELL)
            val.second->deleteAll ();
        delete this;
    }

    Ptr car () { return val.first; }
    Ptr cdr () { return val.second; }

    class iterator : public std::iterator<std::forward_iterator_tag, ScmCell *>
    {
        CellPtr iter;

      public:
        iterator (CellPtr root, int loc) : iter (root)
        {
            if (loc < 0)
                while (iter->val.second->tag == CELL)
                    iter =
                        std::dynamic_pointer_cast<ScmCell> (iter->val.second);
            else
                for (int i = 0; i < loc; i++)
                {
                    assert (iter->val.second->tag == CELL);
                    iter =
                        std::dynamic_pointer_cast<ScmCell> (iter->val.second);
                }
        }
        iterator (CellPtr loc) : iter (loc) {}
        iterator (const iterator & anIter) : iter (anIter.iter) {}

        iterator operator++()
        {
            /* If this fails, then you are probably trying to go beyond
             * List boundaries. It fails when cdr of a pair is no longer
             * another pair. */
            iter = std::dynamic_pointer_cast<ScmCell> (iter->val.second);
            return *this;
        }
        iterator & operator++(int)
        {
            this->operator++();
            return *this;
        }

        bool operator==(const iterator & rhs) { return iter == rhs.iter; }
        bool operator!=(const iterator & rhs) { return iter != rhs.iter; }
        Cons * operator*() { return &iter->val; }
        Cons * operator->() { return &iter->val; }
    };

    iterator begin ()
    {
        return iterator (AsPtr (Cell, shared_from_this ()), 0);
    }
    iterator end () { return iterator (AsPtr (Cell, shared_from_this ()), -1); }
};

class ScmInteger : public ScmValue
{
  public:
    long val;
    ScmInteger (long integer) : ScmValue (INTEGER), val (integer) {}

    bool operator==(const ScmInteger & rhs) { return val == rhs.val; }
    bool operator!=(const ScmInteger & rhs) { return val != rhs.val; }

    void print ();
};

class ScmStr : public ScmValue
{
  public:
    std::string val;

    ScmStr (ScmType tag, std::string str) : ScmValue (tag), val (str) {}

    bool operator==(const ScmStr & rhs) { return val == rhs.val; }
    bool operator!=(const ScmStr & rhs) { return val != rhs.val; }
};

class ScmString : public ScmStr
{
  public:
    ScmString (std::string str) : ScmStr (STRING, str) {}
    void print ();
};

class ScmSymbol : public ScmStr
{
  public:
    typedef std::shared_ptr<ScmSymbol> Ptr;

    ScmSymbol (std::string str) : ScmStr (SYMBOL, str) {}
    void print ();
};

class ScmBoole : public ScmValue
{
  public:
    bool val;
    ScmBoole (bool boole) : ScmValue (BOOLEAN), val (boole) {}

    bool operator==(const ScmBoole & rhs) { return val == rhs.val; }
    bool operator!=(const ScmBoole & rhs) { return val != rhs.val; }

    void print ();
};

class ScmNull : public ScmCell
{
  public:
    ScmNull () : ScmCell (NULL, NULL){};

    bool operator==(const ScmNull & rhs) { return true; }
    bool operator!=(const ScmNull & rhs) { return false; }
};

class ScmFunction : public ScmValue
{
  public:
    typedef std::shared_ptr<ScmFunction> FuncPtr;

    ScmFunction () : ScmValue (FUNCTION) {}
};

class ScmPrimitive : public ScmFunction
{
  public:
    typedef std::shared_ptr<ScmPrimitive> PrimPtr;
    enum Type
    {
        UNKNOWN = 0,
        PLUS,
        MINUS,
        MUL,
        DIV,
        EQ,
        LIST,
    } primType;

  private:
    static std::map<Type, PrimPtr> prims;
    VmOpPtr vmOp1, vmOp2;

  public:
    static bool initialised;

    static void initialise ();
    static Type symToPrimType (ScmSymbol::Ptr sym);
    static PrimPtr primTypeToPrim (Type prim);

    ScmPrimitive (Type prim);
    void compile (VmOpVec & vec);
};

class ScmLambda : public ScmFunction
{
  public:
    typedef std::shared_ptr<ScmLambda> FuncPtr;

    std::string name;
    VmOpVec code;

    ScmLambda (std::string funcname) : name (funcname) {}
};

ScmValue::Ptr makeScmNil ();

#endif
