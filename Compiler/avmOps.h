#ifndef AVMOPS_H
#define	AVMOPS_H

#include "instruction.h"
#include "avm.h"

#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic

void execute_assign(instr_p instr);

void avm_assign(avm_memcell_p lv, avm_memcell_p rv);

double add_impl(double x, double y);
double sub_impl(double x, double y);
double mul_impl(double x, double y);
double div_impl(double x, double y);
double mod_impl(double x, double y);

void execute_arithmetic(instr_p instr);


#endif	/* AVMOPS_H */

