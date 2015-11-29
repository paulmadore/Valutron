/* Valutron
 * Compiler declarations
 * Copyright (c) 2015 D. Mackay. All rights reserved. */

#pragma once

#include "values.h"
#include "vm.h"
#include "vmop.h"

ScmFunction::Ptr scmCompile (ScmCell::CellPtr list);