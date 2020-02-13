#include <assert.h>
#include "scopeSpaces.h"

unsigned int programVarOffset = 0;
unsigned int functionLocalOffset = 0;
unsigned int formalArgOffset = 0;
unsigned int scopeSpaceCounter = 1;

enum scopespace_t currscopespace(void) {
    if (scopeSpaceCounter == 1) {
        return programvar;
    } else {
        if (scopeSpaceCounter % 2 == 0) {
            return formalarg;
        } else {
            return functionlocal;
        }
    }
}

unsigned int currscopeoffset(void) {
    switch (currscopespace()) {
        case programvar: return programVarOffset;
        case functionlocal: return functionLocalOffset;
        case formalarg: return formalArgOffset;
        default: assert(0);
    }
}

void inccurrscopeoffset(void) {
    switch (currscopespace()) {
        case programvar: ++programVarOffset;
            break;
        case functionlocal: ++functionLocalOffset;
            break;
        case formalarg: ++formalArgOffset;
            break;
        default: assert(0);
    }
}

void restorecurrscopeoffset(unsigned int val) {
    switch (currscopespace()) {
        case programvar: programVarOffset = val;
            break;
        case functionlocal: functionLocalOffset = val;
            break;
        case formalarg: formalArgOffset = val;
            break;
        default: assert(0);
    }
}

void resetcurrscopeoffset(void) {
    switch (currscopespace()) {
        case programvar: programVarOffset = 0;
            break;
        case functionlocal: functionLocalOffset = 0;
            break;
        case formalarg: formalArgOffset = 0;
            break;
        default: assert(0);
    }
}

void enterscopespace(void) {
    ++scopeSpaceCounter;
}

void exitscopespace(void) {
    assert(scopeSpaceCounter > 1);
    --scopeSpaceCounter;
    --scopeSpaceCounter;
}
