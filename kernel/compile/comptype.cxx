/* Valutron
 * Compiler/Types (Type bytecode representation) implementation
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#include "values.h"
#include "vmop.h"

/* The default is to simply push the object onto the data stack. */
void ScmValue::compile (VmOperation::Vec & vec)
{
    vec.push_back (VmOperation::Ptr (new VmPush (shared_from_this ())));
}

void ScmPrimitive::compile (VmOperation::Vec & vec) { vec.push_back (vmOp1); }