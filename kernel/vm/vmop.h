/* Valutron
 * VM (Operations) declarations
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#pragma once

#include "values.h"

class VmOperation
{
  public:
    typedef std::shared_ptr<VmOperation> Ptr;
    typedef std::vector<Ptr> Vec;

    enum Type
    {
        /* Push a new frame; bind contents of the top of data stack, down to
         * the first separator found, to their names; initialise frame with
         * location 0 in appropriate subroutine. */
        VMOP_CALL,
        /* Pop the current frame. */
        VMOP_RETURN,
        /* Execute a builtin procedure. */
        VMOP_BUILTIN,
        /* Push an object to the data stack. */
        VMOP_PUSH,
    } type;

    virtual void evaluate (){};
    VmOperation (Type opType) : type (opType) {}
};

class VmCall : public VmOperation
{
  public:
    unsigned short argcnt;
    ScmSymbol::Ptr func;

    VmCall (ScmSymbol::Ptr fun, unsigned short argcunt)
        : VmOperation (VMOP_CALL), argcnt (argcunt)
    {
    }

    void evaluate (){};
};

class VmReturn : public VmOperation
{
  public:
    VmReturn () : VmOperation (VMOP_RETURN) {}
};

class VmBuiltin : public VmOperation
{
    ScmPrimitive::Type prim;

  public:
    VmBuiltin (ScmPrimitive::Type primType)
        : VmOperation (VMOP_BUILTIN), prim (primType)
    {
    }
};

class VmPush : public VmOperation
{
    ScmValue::Ptr val;

  public:
    VmPush (ScmValue::Ptr value) : VmOperation (VMOP_PUSH), val (value) {}
};