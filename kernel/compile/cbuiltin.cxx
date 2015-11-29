/* Valutron
 * Compiler/Types (Type bytecode representation) implementation
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#include "values.h"
#include "vmop.h"

void ScmPrimitive::compile (VmOperation::Vec & vec) { vec.push_back (vmOp1); }