#include <assert.h>
#include <stdlib.h>

#include "fileReader.h"
#include "instruction.h"
#include "avmOps.h"
#include "avmBranches.h"
#include "avmFuncs.h"
#include "avmTables.h"
#include "quad.h"
#include "execute.h"
#include "avm.h"
#include "avmLibfuncs.h"
#include "avmHashtable.h"
#include "dataStructs.h"

extern instr_p instructions;
extern unsigned int currInstr, globalCounter, top, topsp;

unsigned char executeFinished = 0;
unsigned int pc = 0;
unsigned int currLine = 0;
unsigned int codeSize = 0;

execute_func_t executeFuncs[] = {
    execute_add,
    execute_sub,
    execute_mul,
    execute_div,
    execute_mod,
    execute_assign,

    execute_newtable,
    execute_tablegetelem,
    execute_tablesetelem,

    execute_call,
    execute_pusharg,
    execute_funcenter,
    execute_funcexit,

    execute_jump,

    execute_jeq,
    execute_jne,
    execute_jgt,
    execute_jge,
    execute_jlt,
    execute_jle,

    execute_nop
};

void execute_cycle() {
    //printf("PC->%d\n", pc);
    assert(executeFinished == 0);

    if (pc == AVM_ENDING_PC) {
        executeFinished = 1;
        return;
    }
    assert(pc < AVM_ENDING_PC);

    instr_p instr = &instructions[pc];
    assert(instr->opcode >= 0 &&
            instr->opcode <= AVM_MAX_INSTRUCTIONS);

    if (instr->line) {
        currLine = instr->line;
    }
    unsigned int oldPC = pc;

    (* executeFuncs[instr->opcode])(instr);

    if (pc == oldPC) {
        ++pc;
    }
}

void avm_initialize(void) {
    codeSize = currInstr;

    top = AVM_STACKSIZE - 1 - globalCounter;
    topsp = 0;

    avm_initstack();

    avm_init_libfuncs_hashtable();

    avm_registerlibfunc("print", libfunc_print);
    avm_registerlibfunc("input", libfunc_input);
    avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
    avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
    avm_registerlibfunc("objectcopy", libfunc_objectcopy);
    avm_registerlibfunc("totalarguments", libfunc_totalarguments);
    avm_registerlibfunc("argument", libfunc_argument);
    avm_registerlibfunc("typeof", libfunc_typeof);
    avm_registerlibfunc("strtonum", libfunc_strtonum);
    avm_registerlibfunc("sqrt", libfunc_sqrt);
    avm_registerlibfunc("cos", libfunc_cos);
    avm_registerlibfunc("sin", libfunc_sin);

    /*------WARNING HANDLING*/
    warning_line = 0;
    warning_msg = NULL;
}

int main(int argc, char ** argv) {

    globalCounter = 0;

    if (argc > 1) {
        if (argc == 2) {
            if (strcmp(argv[1], "-t") == 0) {
                txtread();
            } else {
                fprintf(stderr, "Invalid option '%s'\n", argv[1]);
                fprintf(stderr, "Option::  -t: Read from tcode.txt\n");
                exit(1);
            }
        } else {// argc > 2
            fprintf(stderr, "Too many command arguments\n", "quads.txt");
            return 1;
        }
    } else {
        abcread();
    }

    avm_initialize();

    while (executeFinished == 0) {
        execute_cycle();
    }

    avm_stack_destroy();
    free_consts_arrays();
    free_instructions();

    return 0;
}
