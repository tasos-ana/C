#include <assert.h>
#include <string.h>

#include "instruction.h"
#include "avmOps.h"
#include "avm.h"
#include "execute.h"

extern avm_memcell_s avm_stack[];
extern avm_memcell_s ax, bx, retval;
extern unsigned char executeFinished;
extern unsigned int top;
extern unsigned int currLine;
extern unsigned int pc;

unsigned int from_assign = 0;

void execute_assign(instr_p instr) {
    avm_memcell_p lv = avm_translate_operand(instr->result, (avm_memcell_p) 0);
    avm_memcell_p rv = avm_translate_operand(instr->arg1, &ax);

    assert(lv != NULL && (((&(avm_stack[AVM_STACKSIZE - 1]) >= lv) && (lv > &(avm_stack[top]))) || lv == &retval));
    assert(rv != NULL);

    if (rv == &retval && retval.type == undef_m) {
        return;
    }

    from_assign = 1;
    avm_assign(lv, rv);
    from_assign = 0;
}

void avm_assign(avm_memcell_p lv, avm_memcell_p rv) {

    if (lv == rv) { /*same cells? destructive to assign*/
        return;
    }

    if (lv->type == table_m && /*same tables? no need to assign.*/
            rv->type == table_m &&
            lv->data.tableVal == rv->data.tableVal) {

        return;
    }

    if (rv->type == undef_m && from_assign == 1) { /*from undefined r-value> warn*/
        char * msg = "assigning from 'undef' content!";

        if (avm_warning_check(msg) == 1) {
            avm_warning("%u: Warning:  %s\n", currLine, msg);
        }
    }

    avm_memcellclear(lv);

    memmove(lv, rv, sizeof (avm_memcell_s));

    /*Now take care of copied value or reference counting
     */
    if (lv->type == string_m) {
        lv->data.strVal = strdup(rv->data.strVal);
    } else {
        if (lv->type == table_m) {
            avm_tableincrefcounter(lv->data.tableVal);
        }
    }
}

typedef double (*arithmetic_func_t) (double x, double y);

double add_impl(double x, double y) {
    return x + y;
}

double sub_impl(double x, double y) {
    return x - y;
}

double mul_impl(double x, double y) {
    return x*y;
}

double div_impl(double x, double y) {
    if (y == 0) {
        avm_error("%u: Error: can't make division with zero number\n", currLine);
        executeFinished = 1;
        return 0;
    }
    return x / y;
}

double mod_impl(double x, double y) {
    if (y == 0) {
        avm_error("%u: Error: can't make division with zero number\n", currLine);
        executeFinished = 1;
        return 0;
    }
    return ((unsigned int) x) % ((unsigned int) y);
}

/*Dispatcher just for arithmetic functions.*/
arithmetic_func_t arithmeticsFuncs[] = {
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

void execute_arithmetic(instr_p instr) {
    avm_memcell_p lv = avm_translate_operand(instr->result, (avm_memcell_p) 0);
    avm_memcell_p rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell_p rv2 = avm_translate_operand(instr->arg2, &bx);

    assert(lv != NULL && (((&(avm_stack[AVM_STACKSIZE - 1]) >= lv) && (lv > &(avm_stack[top]))) || lv == &retval));
    assert(rv1 != NULL && rv2 != NULL);

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("%u: Error: not a number in arithmetic!\n", currLine);
        executeFinished = 1;
    } else {
        arithmetic_func_t op = arithmeticsFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}
