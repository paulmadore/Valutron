/* Valutron
 * VM (Structure) declarations
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#pragma once

#include "values.h"

struct Frame
{
    /* Shared pointer to the current subroutine being executed. */
    ScmFunction::Ptr subroutine;
    /* The current offset into the subroutine. */
    unsigned location;

    /* The local variables of this frame. A mapping of string names to
     * shared pointers to ScmValues. */
    ScmValue::Map locals;
};

class VM
{
  public:
    /* The Fibre constitutes a context of execution.
     * Using `call/cc`, it is possible to deliver a duplicate of the current
     * Fibre to a subroutine, which may store it for later resumption. This
     * enables particularly complex flow control.
     * As well as this, a Fibre is the underlying mechanism enabling
     * Valutron's Eco-Friendly Threads. */
    struct Fibre
    {
        std::vector<ScmValue::Ptr> argstack;
        std::vector<Frame> frames;
        ScmValue::Ptr eval (ScmFunction::Ptr func);
    };

    std::vector<Fibre> fibres;

    Fibre & mainFibre () { return fibres[0]; }
};