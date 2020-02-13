#include <assert.h>
#include <string.h>


#include "avmBranches.h"
#include "instruction.h"
#include "avm.h"
#include "execute.h"

extern unsigned char executeFinished;
extern avm_memcell_s avm_stack[];
extern avm_memcell_s ax, bx, retval;
extern unsigned int pc;
extern unsigned int currLine;

extern unsigned int top;
extern char* typeStrings[];

void execute_jump(instr_p instr) {
    assert(instr->result->type == label_a);
    pc = instr->result->val;
}

void execute_jeq(instr_p instr) {
    assert(instr->result->type == label_a);

    avm_memcell_p rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell_p rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m) {
        avm_error("%u: Error: 'undef' involved in equality\n", currLine);
        executeFinished = 1;
    } else {
        if (rv1->type == nil_m || rv2->type == nil_m) {
            result = (rv1->type == nil_m && rv2->type == nil_m);
        } else {
            if (rv1->type == bool_m || rv2->type == bool_m) {
                result = (avm_tobool(rv1) == avm_tobool(rv2));
            } else {
                if (rv1->type != rv2->type) {
                    avm_error("%u: Error: %s == %s is illegal!\n", currLine, typeStrings[rv1->type], typeStrings[rv2->type]);
                    executeFinished = 1;
                } else {
                    // equality check with dispatching
                    result = avm_isEqual(rv1, rv2);
                }
            }
        }
    }

    if (executeFinished == 0 && result) {
        pc = instr->result->val;
    }
}

void execute_jne(instr_p instr) {
    assert(instr->result->type == label_a);

    avm_memcell_p rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell_p rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m) {
        avm_error("%u: Error: 'undef' involved in equality\n", currLine);
        executeFinished = 1;
    } else {
        if (rv1->type == nil_m || rv2->type == nil_m) {
            result = ((rv1->type == nil_m && rv2->type != nil_m) ||
                    (rv1->type != nil_m && rv2->type == nil_m));
        } else {
            if (rv1->type == bool_m || rv2->type == bool_m) {
                result = (avm_tobool(rv1) != avm_tobool(rv2));
            } else {
                if (rv1->type != rv2->type) {
                    avm_error("%u: Error: %s != %s is illegal!\n", currLine, typeStrings[rv1->type], typeStrings[rv2->type]);
                    executeFinished = 1;
                } else {
                    // equality check with dispatching
                    result = !(avm_isEqual(rv1, rv2));
                }
            }
        }
    }

    if (executeFinished == 0 && result) {
        pc = instr->result->val;
    }
}

unsigned char jle_impl(double x, double y) {
    return x <= y;
}

unsigned char jge_impl(double x, double y) {
    return x >= y;
}

unsigned char jlt_impl(double x, double y) {
    return x<y;
}

unsigned char jgt_impl(double x, double y) {
    return x>y;
}

/*Dispatcher for greater/equal,less/equal branches*/

cmp_func_t comparisonFuncs[] = {
    jgt_impl,
    jge_impl,
    jlt_impl,
    jle_impl
};

void execute_cmp(instr_p instr) {
    assert(instr->result->type == label_a);

    avm_memcell_p rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell_p rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("%u: Error: not a number in branches!\n", currLine);
        executeFinished = 1;
    } else {
        cmp_func_t op = comparisonFuncs[instr->opcode - jgt_v];
        result = (*op)(rv1->data.numVal, rv2->data.numVal);
    }

    if (executeFinished == 0 && result == 1) {
        pc = instr->result->val;
    }
}

// ------------------------- TOBOOL FUNCS -------------------------

unsigned char number_tobool(avm_memcell_p cell) {
    return cell->data.numVal != 0;
}

unsigned char string_tobool(avm_memcell_p cell) {
    return cell->data.strVal[0] != 0;
}

unsigned char bool_tobool(avm_memcell_p cell) {
    return cell->data.boolVal;
}

unsigned char table_tobool(avm_memcell_p cell) {
    return 1;
}

unsigned char userfunc_tobool(avm_memcell_p cell) {
    return 1;
}

unsigned char libfunc_tobool(avm_memcell_p cell) {
    return 1;
}

unsigned char nil_tobool(avm_memcell_p cell) {
    return 0;
}

unsigned char undef_tobool(avm_memcell_p cell) {
    assert(0);
    return 0;
}

toboolfunc_t toboolFuncs[] = {
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool
};

unsigned char avm_tobool(avm_memcell_p cell) {
    assert(cell->type >= 0 && cell->type < undef_m);
    return (* toboolFuncs[cell->type])(cell);
}

// ------------------------- ISEQUAL FUNCS -------------------------

isEqualfunc_t isEqualFuncs[] = {
    number_isEqual,
    string_isEqual,
    bool_isEqual,
    table_isEqual,
    userfunc_isEqual,
    libfunc_isEqual,
    nil_isEqual,
    undef_isEqual
};

unsigned char avm_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {
    assert(rv1->type == rv2->type);

    return (* isEqualFuncs[rv1->type])(rv1, rv2);
}

unsigned char number_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {
    assert(rv1->type == number_m);

    return (rv1->data.numVal == rv2->data.numVal);
}

unsigned char string_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {
    assert(rv1->type == string_m);

    return (strcmp(rv1->data.strVal, rv2->data.strVal) == 0);
}

unsigned char table_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {
    assert(rv1->type == table_m);

    return (rv1->data.tableVal == rv2->data.tableVal);
}

unsigned char bool_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {

}

unsigned char userfunc_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {
    assert(rv1->type == userfunc_m);

    return (rv1->data.funcVal == rv2->data.funcVal);
}

unsigned char libfunc_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {
    assert(rv1->type == libfunc_m);

    return (strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal) == 0);
}

unsigned char nil_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {

}

unsigned char undef_isEqual(avm_memcell_p rv1, avm_memcell_p rv2) {

}
