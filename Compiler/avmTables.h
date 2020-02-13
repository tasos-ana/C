#ifndef AVMTABLES_H
#define	AVMTABLES_H

#include "instruction.h"
#include "avmHashtable.h"
#include "avm.h"

//unsigned int get_tmp_stack_index(vmarg_p arg);

void execute_newtable(instr_p instr);
void execute_tablegetelem(instr_p instr);
void execute_tablesetelem(instr_p instr);
void execute_nop(instr_p instr);

void avm_tableincrefcounter(avm_table_p t);

void avm_tabledecrefcounter(avm_table_p t);

#endif	/* AVMTABLES_H */

