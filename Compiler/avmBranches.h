#ifndef AVMBRANCHES_H
#define	AVMBRANCHES_H

#include "avm.h"

#define execute_jle execute_cmp
#define execute_jge execute_cmp
#define execute_jlt execute_cmp
#define execute_jgt execute_cmp

typedef unsigned char (*cmp_func_t)(double x, double y);

void execute_jump(instr_p instr);

void execute_jeq(instr_p instr);
void execute_jne(instr_p instr);

void execute_cmp(instr_p instr);

// ------------------------- TOBOOL FUNCS -------------------------

typedef unsigned char (* toboolfunc_t)(avm_memcell_p cell);

unsigned char number_tobool(avm_memcell_p cell);
unsigned char string_tobool(avm_memcell_p cell);
unsigned char bool_tobool(avm_memcell_p cell);
unsigned char table_tobool(avm_memcell_p cell);
unsigned char userfunc_tobool(avm_memcell_p cell);
unsigned char libfunc_tobool(avm_memcell_p cell);
unsigned char nil_tobool(avm_memcell_p cell);
unsigned char undef_tobool(avm_memcell_p cell);

unsigned char avm_tobool(avm_memcell_p cell);

// ------------------------- ISEQUAL FUNCS -------------------------

typedef unsigned char (* isEqualfunc_t)(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char avm_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);


unsigned char number_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char string_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char table_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char bool_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char userfunc_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char libfunc_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char nil_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

unsigned char undef_isEqual(avm_memcell_p rv1, avm_memcell_p rv2);

#endif	/* AVMBRANCHES_H */

