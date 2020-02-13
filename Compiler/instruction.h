#ifndef INSTRUCTION_H
#define	INSTRUCTION_H

#include "symbolTable.h"
#include "expr.h"

enum vmarg_t {
    label_a = 0,
    global_a = 1,
    formal_a = 2,
    local_a = 3,
    number_a = 4,
    string_a = 5,
    bool_a = 6,
    nil_a = 7,
    userfunc_a = 8,
    libfunc_a = 9,
    retval_a = 10
};

typedef struct vmarg {
    enum vmarg_t type;
    unsigned int val;
} vmarg_s, *vmarg_p;

enum vmopcode {
    add_v = 0, sub_v, mul_v,
    div_v, mod_v, assign_v,
    newtable_v, tablegetelem_v, tablesetelem_v,
    callfunc_v, pusharg_v,
    funcenter_v, funcexit_v,
    jump_v, jeq_v, jne_v, jgt_v, jge_v, jlt_v, jle_v,

    nop_v = 20
};

typedef struct instruction {
    enum vmopcode opcode;

    vmarg_p arg1;
    vmarg_p arg2;
    vmarg_p result;

    unsigned int line;
} instr_s, *instr_p;

void instr_expand(void);
void instr_emit(instr_s t);
unsigned int nextinstr(void);
void instr_reset(instr_p t);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void make_operand(expr_p e, vmarg_p* arg);

void make_numberoperand(vmarg_p* arg, double val);
void make_booloperand(vmarg_p* arg, unsigned int val);
void make_retvaloperand(vmarg_p* arg);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void instr_patchlabel(unsigned int instrNo, unsigned int label);
void instr_backpatch(list_p head, int label);

typedef struct incomplete_jump {
    unsigned int instrNo;
    unsigned int iaddress;
    struct incomplete_jump * next;
} incJump_s, *incJump_p;

void add_incomplete_jump(unsigned int instrNo, unsigned int iaddress);

void patch_incomplete_jumps();

void instr_print(void);
char* instrName(int op);
void decode(vmarg_p arg, instr_list_p * currVar);

#endif	/* INSTRUCTION_H */
