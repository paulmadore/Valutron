#ifndef _INTERPRETER_VM_H
#define _INTERPRETER_VM_H 1
#include <stdint.h>
#include <vector>

enum OpcodeVals {
    OP_NONE, // nop
    OP_LOAD,
    OP_CALL,
    OP_JUMP,
    OP_RETURN,
};

class ScmValue;

typedef struct {
    unsigned tag;

    union {
        ScmValue *scmval;
        unsigned number;
    };
} RegVal;

struct VMOpcode {
    uint16_t op;
    uint16_t flags;

    RegVal args[2];
};

class ScmVM {
    public:
        std::vector<RegVal> regs[32];


};

#include <values.hpp>

#endif
