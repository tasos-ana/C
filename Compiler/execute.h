#ifndef EXECUTE_H
#define	EXECUTE_H

#include "instruction.h"

unsigned int codeSize;

#define AVM_MAX_INSTRUCTIONS (unsigned int) nop_v
#define AVM_ENDING_PC codeSize

typedef void (* execute_func_t)(instr_p);

void execute_cycle();

void avm_initialize(void);

#endif	/* EXECUTE_H */

