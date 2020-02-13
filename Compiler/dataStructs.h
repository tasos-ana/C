#ifndef DATASTRUCTS_H
#define	DATASTRUCTS_H

#include "symbolTable.h"

typedef struct stack {
    int val;
    struct stack * next;
} *stack_p;

int isEmptyStack(stack_p stackTop);
//int top(stack_p stackTop);
void push(stack_p * stackTop, int newVal);
int pop(stack_p * stackTop);
void freeStack(stack_p * stackTop);

typedef struct f_stack {
    struct SymbolTableEntry * f;
    struct f_stack * next;
} *f_stack_p;

int f_isEmptyStack(f_stack_p stackTop);
struct SymbolTableEntry * f_top(f_stack_p stackTop);
void f_push(f_stack_p * stackTop, struct SymbolTableEntry * newf);
struct SymbolTableEntry * f_pop(f_stack_p * stackTop);
void f_freeStack(f_stack_p * stackTop);

typedef struct list {
    int label_incJump;
    struct list * next;
} *list_p;

list_p create_list_node(int label);
list_p merge_list(list_p list1, list_p list2);

list_p append_list(list_p l, int label);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


typedef struct instr_list {
    const char* name;
    struct instr_list* next;
} instr_list_s, *instr_list_p;

void instr_list_insert(const char* name);
instr_list_p instr_list_create(const char* name);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct userfunc {
    unsigned int address;
    unsigned int localSize;
    const char * id;
} userfunc_s, *userfunc_p;

void num_expand(void);
void str_expand(void);
void lib_expand(void);
void user_expand(void);

unsigned int consts_newnumber(double n);
unsigned int consts_newstring(const char* s);
unsigned int libfuncs_newused(const char* s);
unsigned int userfuncs_newfunc(struct SymbolTableEntry * sym);
unsigned int userfuncs_add(unsigned int addr, const char* name, unsigned int totallocals);

void consts_numbers_free(void);
void consts_strings_free(void);
void libfuncs_free(void);
void userfuncs_free(void);

void free_consts_arrays(void);
void free_instructions(void);
#endif	/* DATASTRUCTS_H */
