/* Valutron
 * Types (Builtin functions) implementation
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#include "values.h"

std::map<ScmPrimitive::Type, ScmPrimitive::PrimPtr> ScmPrimitive::prims;
bool ScmPrimitive::initialised = false;

void ScmPrimitive::initialise ()
{
#define MkPrim(x) prims[x] = PrimPtr (new ScmPrimitive (x))
    MkPrim (PLUS);
    MkPrim (MINUS);
    MkPrim (MUL);
    MkPrim (DIV);
    MkPrim (EQ);
    MkPrim (LIST);
#undef MkPrim
    initialised = true;
}

ScmPrimitive::Type ScmPrimitive::symToPrimType (ScmSymbol::Ptr sym)
{
    std::string & val = sym->val;
    /* clang-format off */
    return val == "+"    ? PLUS 
         : val == "-"    ? MINUS
         : val == "*"    ? MUL
         : val == "/"    ? DIV
         : val == "="    ? EQ
         : val == "list" ? LIST
         : /* ε */         UNKNOWN;
    /* clang-format on */
}

ScmPrimitive::PrimPtr ScmPrimitive::primTypeToPrim (Type prim)
{
    return prims[prim];
}