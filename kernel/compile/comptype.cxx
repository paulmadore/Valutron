/* Valutron
 * Compiler/Types (Type bytecode representation) implementation
 * Copyright (c) 2015 D. Mackay. All rights reserved.
 *
 * This file contains primarily methods of the type classes. They
 * are placed here because, while they actually breach the barrier
 * separating the Compiler from the Types subsystems, they fit most
 * clearly into the Compiler subsystem. */

#include "values.h"
#include "vmop.h"

/* The default is to simply push the object onto the data stack. */
void ScmValue::compile (VmOperation::Vec & vec)
{
    vec.push_back (VmOperation::Ptr (new VmPush (shared_from_this ())));
}

ScmPrimitive::ScmPrimitive (Type prim) : primType (prim)
{
    vmOp1 = VmOperation::Ptr (new VmBuiltin (primType));
}

void ScmPrimitive::compile (VmOperation::Vec & vec) { vec.push_back (vmOp1); }