/* Valutron
 * Compiler implementation
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#include "compile.h"

/* ScmCell constitutes a special case for compilation;
 * the resultant bytecode varies totally depending on its form. */
void ScmCell::compile (VmOperation::Vec & vec)
{
    short argcnt = 0;

    for (ScmCell::iterator it = this->begin ()++; it != this->end (); it++)
    {
        argcnt++;
        it->first->compile (vec);
    }

    vec.push_back (VmOperation::Ptr (
        new VmCall (AsPtr (Symbol, this->val.first), argcnt)));
}

ScmFunction::Ptr scmCompile (ScmCell::CellPtr list)
{
    ScmFunction::Ptr newProc = ScmFunction::Ptr (new ScmFunction);
}