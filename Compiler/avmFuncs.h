#ifndef AVMFUNCS_H
#define	AVMFUNCS_H

#include <stdio.h>
#include "instruction.h"
#include "avmHashtable.h"

#define AVM_NUMACTUALS_OFFSET +4
#define AVM_SAVEDPC_OFFSET +3
#define AVM_SAVEDTOP_OFFSET +2
#define AVM_SAVEDTOPSP_OFFSET +1

void execute_call(instr_p instr);
void execute_pusharg(instr_p instr);
void execute_funcenter(instr_p instr);
void execute_funcexit(instr_p unused);

void avm_callsaveenviroment();
void avm_push_envvalue(unsigned int val);
void avm_dec_top();

unsigned int avm_get_envvalue(unsigned int i);

void avm_calllibfunc(char * id);

void functorCall(avm_memcell_p table);
void pusharg_table(avm_memcell_p table);
void stack_slide();

#endif	/* AVMFUNCS_H */

