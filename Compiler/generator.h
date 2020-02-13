#ifndef GENERATOR_H
#define	GENERATOR_H

#include "dataStructs.h"
#include "quad.h"
#include "instruction.h"


typedef void (*generator_func_t)(quad_p);

f_stack_p funcstack;

void generate(enum vmopcode op, quad_p quad);
void generate_relational(enum vmopcode op, quad_p quad);

void reset_operand(vmarg_p* arg);

void generate_ADD(quad_p quad);
void generate_SUB(quad_p quad);
void generate_MUL(quad_p quad);
void generate_DIV(quad_p quad);
void generate_MOD(quad_p quad);

void generate_NEWTABLE(quad_p quad);
void generate_TABLEGETELEM(quad_p quad);
void generate_TABLESETELEM(quad_p quad);
void generate_ASSIGN(quad_p quad);
void generate_NOP(void);
void generate_JUMP(quad_p quad);

void generate_IF_EQ(quad_p quad);
void generate_IF_NOTEQ(quad_p quad);
void generate_IF_GREATER(quad_p quad);
void generate_IF_GREATEREQ(quad_p quad);
void generate_IF_LESS(quad_p quad);
void generate_IF_LESSEQ(quad_p quad);

void generate_NOT(quad_p quad);

void generate_PARAM(quad_p quad);
void generate_CALL(quad_p quad);
void generate_GETRETVAL(quad_p quad);

void generate_FUNCSTART(quad_p quad);
void generate_RETURN(quad_p quad);
void generate_FUNCEND(quad_p quad);



void generate_tc(void);

#endif	/* GENERATOR_H */
