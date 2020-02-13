#include <assert.h>
#include <stdlib.h>
#include "avmFuncs.h"
#include "execute.h"
#include "avm.h"
#include "dataStructs.h"
#include "avmOps.h"
#include "avmLibfuncs.h"
#include "avmHashtable.h"

extern avm_memcell_s avm_stack[];
extern unsigned int pc;
extern unsigned int top, topsp;
extern avm_memcell_s ax, retval;
extern instr_p instructions;
extern unsigned char executeFinished;
extern unsigned int currLine;


unsigned int totalActuals = 0;

userfunc_p avm_getfuncinfo(unsigned int addr) {
    instr_p instr = &instructions[addr];
    assert(instr->opcode = funcenter_v);

    return avm_getuserfuncs(instr->result->val);
}

void execute_call(instr_p instr) {
    //fprintf(stderr,"\n%d------\n",instr->arg1->type);
    avm_memcell_p func = avm_translate_operand(instr->arg1, &ax);

    assert(func != NULL);
    avm_callsaveenviroment();

    if (executeFinished) return;

    avm_memcellclear(&retval);
    assert(retval.type == undef_m);

    switch (func->type) {
        case userfunc_m:
        {
            userfunc_p f = avm_getuserfuncs(func->data.funcVal);
            pc = f->address;
            assert(pc < AVM_ENDING_PC);
            assert(instructions[pc].opcode == funcenter_v);
            break;
        }

        case string_m:
        {
            avm_calllibfunc(func->data.strVal);
            break;
        }

        case libfunc_m:
        {
            avm_calllibfunc(func->data.libfuncVal);
            break;
        }

        case table_m:
        {
            functorCall(func);
            break;
        }

        default:
        {
            char * s = avm_tostring(func); // auth kanei malloc ektos an einai undef
            if (s == NULL) {
                avm_error("%u: Error: call: illegal or undefined function!\n", currLine);
                return;
            }
            avm_error("%u: Error: call: cannot bind '%s' to function!\n", currLine, s);
            executeFinished = 1;
            free(s);
        }
    }
}

void execute_pusharg(instr_p instr) {
    avm_memcell_p arg = avm_translate_operand(instr->arg1, &ax);
    assert(arg != NULL);

    avm_assign(&avm_stack[top], arg);
    ++totalActuals;
    avm_dec_top();
}

void execute_funcenter(instr_p instr) {
    avm_memcell_p func = avm_translate_operand(instr->result, &ax);
    assert(func != NULL);
    userfunc_p f = avm_getuserfuncs(func->data.funcVal);
    assert(pc == f->address);

    /* Callee Actions Here */
    totalActuals = 0;
    userfunc_p funcInfo = avm_getfuncinfo(pc);

    topsp = top;
    top = top - funcInfo->localSize;
}

void execute_funcexit(instr_p unused) {
    unsigned int oldTop = top;

    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    while (++oldTop <= top) { // intentionally ignoring first
        avm_memcellclear(&avm_stack[oldTop]);
    }
}

void avm_callsaveenviroment() {
    avm_push_envvalue(totalActuals);

    if (executeFinished) return;
    avm_push_envvalue(pc + 1);

    if (executeFinished) return;
    avm_push_envvalue(top + totalActuals + 2);

    if (executeFinished) return;
    avm_push_envvalue(topsp);
}

void avm_push_envvalue(unsigned int val) {
    avm_stack[top].type = number_m;
    avm_stack[top].data.numVal = val;
    avm_dec_top();
}

void avm_dec_top() {
    if (top == 0) {
        avm_error("Error: stack overflow\n");
        executeFinished = 1;
    } else {
        --top;
    }
}

unsigned int avm_get_envvalue(unsigned int i) {
    assert(avm_stack[i].type == number_m);
    unsigned int val = (unsigned int) avm_stack[i].data.numVal;

    assert(avm_stack[i].data.numVal == ((double) val));
    return val;
}

void avm_calllibfunc(char * id) {
    library_func_t f = avm_getlibraryfunc(id);
    if (f == NULL) {
        avm_error("%u: Error: unsupported library function '%s' called!\n", currLine, id);
        executeFinished = 1;
    } else {
        topsp = top;
        totalActuals = 0;
        (*f)(); // call library function
        if (executeFinished == 0) {
            execute_funcexit(NULL); // return sequence
        }
    }
}

void functorCall(avm_memcell_p table) {
    assert(table->data.tableVal != NULL);

    avm_memcell_s key;
    key.type = string_m;
    key.data.strVal = "()";

    avm_memcell_p x = avm_tablegetelem(table->data.tableVal, &key);

    if (x != NULL) {
        switch (x->type) {
            case userfunc_m:
            {
                pusharg_table(table);

                userfunc_p f = avm_getuserfuncs(x->data.funcVal);

                pc = f->address;
                assert(pc < AVM_ENDING_PC);
                assert(instructions[pc].opcode == funcenter_v);
                break;
            }

            case string_m:
            {
                pusharg_table(table);

                avm_calllibfunc(x->data.strVal);
                break;
            }

            case libfunc_m:
            {
                pusharg_table(table);

                avm_calllibfunc(x->data.libfuncVal);
                break;
            }

            case table_m:
            {
                functorCall(x);
                return;
            }

            default:
            {
                avm_error("%u: Error: call: Not a function or table at index[\"()\"]\n", currLine);
                executeFinished = 1;
            }
        }
    } else {
        avm_error("%u: Error: call: Illegal call of table as functor\n", currLine);
        executeFinished = 1;
    }
}

void pusharg_table(avm_memcell_p table) {
    assert(table->type == table_m);
    if (top == 0) {
        avm_error("Error: stack overflow\n");
        executeFinished = 1;
    }
    unsigned int after_top = top - 1;
    double actuals_num = avm_stack[top + AVM_NUMACTUALS_OFFSET].data.numVal;
    unsigned int table_index = top + AVM_STACKENV_SIZE;

    stack_slide();
    assert(table_index == top);

    avm_assign(&avm_stack[top], table);
    top = top - AVM_STACKENV_SIZE - 1;

    assert(avm_stack[top + AVM_NUMACTUALS_OFFSET].data.numVal == actuals_num + 1);
    assert(top != 0);
    assert(top == after_top);

    assert(avm_stack[table_index].type == table->type &&
            avm_stack[table_index].data.tableVal == table->data.tableVal); // o stoxos
}

void stack_slide() {
    unsigned int prev = top + 1;
    int i;
    for (i = 0; i < 4; ++i) {
        avm_push_envvalue(avm_get_envvalue(prev));
        top = prev++;
        assert(avm_stack[top - 1].data.numVal == avm_stack[top].data.numVal);
    }
    avm_stack[top - 1].data.numVal++;
    /*
        unsigned int n = avm_stack[top - 1].data.numVal++;
        for (i = 0; i < n; ++i) {
            avm_assign(&avm_stack[top], &avm_stack[prev]);
            top = prev++;
        }
     */
}