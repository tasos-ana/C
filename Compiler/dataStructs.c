#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "quad.h"
#include "dataStructs.h"
#include "symbolTable.h"
#include "generator.h"
#include "instruction.h"

extern quad_p quads;

extern int enterFunc;
extern instr_p instructions;
extern unsigned int currInstr;

int isEmptyStack(stack_p stackTop) {
    if (stackTop == NULL) {
        return 1;
    }
    return 0;
}

void push(stack_p * stackTop, int newVal) {
    stack_p newNode = malloc(sizeof (struct stack));

    newNode->val = newVal;
    newNode->next = *stackTop;
    *stackTop = newNode;
}

int pop(stack_p * stackTop) {
    assert(!isEmptyStack(*stackTop));

    stack_p tmp = *stackTop;
    int retVal = tmp->val;

    *stackTop = tmp->next;
    tmp->next = NULL;
    free(tmp);

    return retVal;
}

void freeStack(stack_p * stackTop) {
    while (!isEmptyStack(*stackTop)) {
        pop(stackTop);
    }
}

int f_isEmptyStack(f_stack_p stackTop) {
    if (stackTop == NULL) {
        return 1;
    }
    return 0;
}

SymbolTableEntry * f_top(f_stack_p stackTop) {
    return stackTop->f;
}

void f_push(f_stack_p * stackTop, SymbolTableEntry* newf) {
    f_stack_p newNode = malloc(sizeof (struct f_stack));

    newNode->f = newf;
    newNode->next = *stackTop;
    *stackTop = newNode;
}

SymbolTableEntry* f_pop(f_stack_p * stackTop) {
    assert(!f_isEmptyStack(*stackTop));

    f_stack_p tmp = *stackTop;
    SymbolTableEntry * retVal = tmp->f;

    *stackTop = tmp->next;
    tmp->next = NULL;
    free(tmp);
    return retVal;
}

void f_freeStack(f_stack_p * stackTop) {
    while (!f_isEmptyStack(*stackTop)) {
        f_pop(stackTop);
    }
}

list_p create_list_node(int label) {
    list_p tmp = (list_p) malloc(sizeof (struct list));
    tmp->next = NULL;
    tmp->label_incJump = label;
    return tmp;
}

list_p merge_list(list_p list1, list_p list2) {
    list_p curr = list1;
    if (list1 == NULL) {
        return list2;
    }
    if (list2 == NULL) {
        return list1;
    }
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = list2;

    return list1;
}

list_p append_list(list_p l, int label) {
    return merge_list(l, create_list_node(label));
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
instr_list_p instr_head = NULL;
instr_list_p instr_tail = NULL;

void instr_list_insert(const char* name) {
    if (instr_tail == NULL) {
        instr_head = instr_list_create(name);
        instr_tail = instr_head;
    } else {
        instr_tail->next = instr_list_create(name);
        instr_tail = instr_tail->next;
    }
}

instr_list_p instr_list_create(const char* name) {
    instr_list_p tmp = (instr_list_p) malloc(sizeof (instr_list_s));
    tmp->name = strdup(name);
    tmp->next = NULL;
    return tmp;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define EXPAND_SIZE  1024

#define NUM_CURR_SIZE    (num_totalMem * sizeof(double))
#define NUM_NEW_SIZE     (EXPAND_SIZE * sizeof(double) + NUM_CURR_SIZE)

#define STR_CURR_SIZE    (str_totalMem * sizeof(char *))
#define STR_NEW_SIZE     (EXPAND_SIZE * sizeof(char *) + STR_CURR_SIZE)

#define LIB_CURR_SIZE    (lib_totalMem * sizeof(char *))
#define LIB_NEW_SIZE     (EXPAND_SIZE * sizeof(char *) + LIB_CURR_SIZE)

#define USER_CURR_SIZE    (user_totalMem * sizeof(userfunc_s))
#define USER_NEW_SIZE     (EXPAND_SIZE * sizeof(userfunc_s) + USER_CURR_SIZE)

double * numConsts = NULL;
char ** stringConsts = NULL;
char ** namedLibfuncs = NULL;
userfunc_p userFuncs = NULL;

unsigned int totalNumConsts = 0; // einai to index katw apo to teleutaio numConst
unsigned int totalStringConsts = 0; // einai to index katw apo to teleutaio stringConst
unsigned int totalNamedLibfuncs = 0; // einai to index katw apo to teleutaio namedLibfunc
unsigned int totalUserFuncs = 0; // einai to index katw apo to teleutaio userFunc

unsigned int num_totalMem = 0; // ta sunolika numConsts gia ta opoia exoume parei mnhmh
unsigned int str_totalMem = 0; // ta sunolika stringConsts gia ta opoia exoume parei mnhmh
unsigned int lib_totalMem = 0; // ta sunolika namedLibfuncs gia ta opoia exoume parei mnhmh
unsigned int user_totalMem = 0; // ta sunolika userFuncs gia ta opoia exoume parei mnhmh

void num_expand(void) {
    assert(num_totalMem == totalNumConsts);
    double * p = (double *) malloc(NUM_NEW_SIZE);
    assert(p != NULL);
    if (numConsts != NULL) {
        memmove(p, numConsts, NUM_CURR_SIZE);
        free(numConsts);
    }
    numConsts = p;
    num_totalMem += EXPAND_SIZE;
}

void str_expand(void) {
    assert(str_totalMem == totalStringConsts);
    char ** p = (char **) malloc(STR_NEW_SIZE);
    assert(p != NULL);
    if (stringConsts != NULL) {
        memmove(p, stringConsts, STR_CURR_SIZE);
        free(stringConsts);
    }
    stringConsts = p;
    str_totalMem += EXPAND_SIZE;
}

void lib_expand(void) {
    assert(lib_totalMem == totalNamedLibfuncs);
    char ** p = (char **) malloc(LIB_NEW_SIZE);
    assert(p != NULL);
    if (namedLibfuncs != NULL) {
        memmove(p, namedLibfuncs, LIB_CURR_SIZE);
        free(namedLibfuncs);
    }
    namedLibfuncs = p;
    lib_totalMem += EXPAND_SIZE;
}

void user_expand(void) {
    assert(user_totalMem == totalUserFuncs);
    userfunc_p p = (userfunc_p) malloc(USER_NEW_SIZE);
    assert(p != NULL);
    if (userFuncs != NULL) {
        memmove(p, userFuncs, USER_CURR_SIZE);
        free(userFuncs);
    }
    userFuncs = p;
    user_totalMem += EXPAND_SIZE;
}

unsigned int consts_newnumber(double n) {
    int i;

    for (i = 0; i < totalNumConsts; ++i) {
        if (numConsts[i] == n) {
            return i;
        }
    }

    if (totalNumConsts == num_totalMem) {
        num_expand();
    }

    numConsts[totalNumConsts] = n;

    return totalNumConsts++;
}

unsigned int consts_newstring(const char* s) {
    int i;
    for (i = 0; i < totalStringConsts; ++i) {
        if (strcmp(s, stringConsts[i]) == 0) {
            return i;
        }
    }

    if (totalStringConsts == str_totalMem) {
        str_expand();
    }

    stringConsts[totalStringConsts] = strdup(s);

    return totalStringConsts++;
}

unsigned int libfuncs_newused(const char* s) {
    int i;
    for (i = 0; i < totalNamedLibfuncs; ++i) {
        if (strcmp(s, namedLibfuncs[i]) == 0) {
            return i;
        }
    }
    if (totalNamedLibfuncs == lib_totalMem) {
        lib_expand();
    }

    *(namedLibfuncs + totalNamedLibfuncs) = strdup(s);

    return totalNamedLibfuncs++;
}

unsigned int userfuncs_newfunc(SymbolTableEntry * sym) {
    if (enterFunc == 0) {
        return instructions[sym->value.funcVal->taddress].result->val;
    }

    if (totalUserFuncs == user_totalMem) {
        user_expand();
    }
    userfunc_p p = userFuncs + totalUserFuncs; // deixnei katw apo thn teleutaia userfunc

    p->address = sym->value.funcVal->taddress;
    p->id = sym->value.funcVal->name;
    p->localSize = sym->value.funcVal->totallocals;

    return totalUserFuncs++;
}

unsigned int userfuncs_add(unsigned int addr, const char* name, unsigned int totallocals) {
    if (totalUserFuncs == user_totalMem) {
        user_expand();
    }
    userfunc_p p = userFuncs + totalUserFuncs; // deixnei katw apo thn teleutaia userfunc

    p->address = addr;
    p->id = strdup(name);
    p->localSize = totallocals;

    return totalUserFuncs++;
}

void consts_numbers_free(void) {
    free(numConsts);
    numConsts = NULL;
}

void consts_strings_free(void) {
    int i;
    for (i = 0; i < totalStringConsts; ++i) {
        free(stringConsts[i]);
        stringConsts[i] = NULL;
    }
    free(stringConsts);
    stringConsts = NULL;
}

void libfuncs_free(void) {
    int i;
    for (i = 0; i < totalNamedLibfuncs; ++i) {
        free(namedLibfuncs[i]);
        namedLibfuncs[i] = NULL;
    }
    free(namedLibfuncs);
    namedLibfuncs = NULL;
}

void userfuncs_free(void) {
    int i;
    for (i = 0; i < totalUserFuncs; ++i) {
        free((char*) (userFuncs[i].id));
        userFuncs[i].id = NULL;
    }
    free(userFuncs);
    userFuncs = NULL;
}

void free_consts_arrays(void) {
    consts_numbers_free();
    consts_strings_free();
    libfuncs_free();
    userfuncs_free();
}

void free_instructions() {
    int i;
    for (i = 0; i < currInstr; ++i) {
        free(instructions[i].arg1);
        free(instructions[i].arg2);
        free(instructions[i].result);

        instructions[i].arg1 = NULL;
        instructions[i].arg2 = NULL;
        instructions[i].result = NULL;
    }
    free(instructions);
    instructions = NULL;
}