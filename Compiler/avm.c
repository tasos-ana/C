#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "avm.h"
#include "avmHashtable.h"

avm_memcell_s avm_stack[AVM_STACKSIZE];

extern double * numConsts;
extern char ** stringConsts;
extern char ** namedLibfuncs;
extern userfunc_p userFuncs;

extern unsigned int totalNumConsts; // einai to index katw apo to teleutaio numConst
extern unsigned int totalStringConsts; // einai to index katw apo to teleutaio stringConst
extern unsigned int totalNamedLibfuncs; // einai to index katw apo to teleutaio namedLibfunc
extern unsigned int totalUserFuncs; // einai to index katw apo to teleutaio userFunc

extern unsigned int executeFinished;

extern unsigned int currLine;

avm_memcell_s ax, bx, cx;
avm_memcell_s retval;

unsigned int top;
unsigned int topsp;

unsigned int globalCounter;

void avm_initstack(void) {
    unsigned int i;
    for (i = 0; i < AVM_STACKSIZE; ++i) {
        AVM_WIPEOUT(avm_stack[i]);
        avm_stack[i].type = undef_m;
    }
}

void avm_stack_destroy(void) {
    unsigned int i;
    for (i = AVM_STACKSIZE - 1; i > top; --i) {
        avm_memcellclear(&(avm_stack[i]));
    }
}

char * typeStrings[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};
// ------------------------ TRANSLATION -----------------------------

avm_memcell_p avm_translate_operand(vmarg_p arg, avm_memcell_p reg) {
    switch (arg->type) {
        case global_a: return &avm_stack[AVM_STACKSIZE - 1 - arg->val];
        case local_a: return &avm_stack[topsp - arg->val];
        case formal_a: return &avm_stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];

        case retval_a: return &retval;

        case number_a:
        {
            reg->type = number_m;
            reg->data.numVal = avm_consts_getnumber(arg->val);
            return reg;
        }

        case string_a:
        {
            reg->type = string_m;
            reg->data.strVal = strdup(avm_consts_getstring(arg->val));
            return reg;
        }

        case bool_a:
        {
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        }

        case nil_a: reg->type = nil_m;
            return reg;

        case userfunc_a:
        {
            reg->type = userfunc_m;
            reg->data.funcVal = arg->val;
            return reg;
        }

        case libfunc_a:
        {
            reg->type = libfunc_m;
            reg->data.libfuncVal = avm_libfuncs_getused(arg->val);
            return reg;
        }

        default:
        {
            printf("%u\n", arg->type);
            printf("%u\n", arg->val);
            assert(0);
        }
    }
}

double avm_consts_getnumber(unsigned int index) {
    assert(index >= 0 || index < totalNumConsts);
    return numConsts[index];
}

char * avm_consts_getstring(unsigned int index) {
    assert(index >= 0 || index < totalStringConsts);
    return stringConsts[index];
}

char * avm_libfuncs_getused(unsigned int index) {
    assert(index >= 0 || index < totalNamedLibfuncs);
    return namedLibfuncs[index];
}

userfunc_p avm_getuserfuncs(unsigned int index) {
    assert(index >= 0 || index < totalUserFuncs);
    return userFuncs + index;
}

void memclear_string(avm_memcell_p m) {
    assert(m->data.strVal != NULL);
    free(m->data.strVal);
}

void memclear_table(avm_memcell_p m) {
    assert(m->data.tableVal != NULL);
    avm_tabledecrefcounter(m->data.tableVal);
}

memclear_func_t memclearFuncs[] = {
    0, //number
    memclear_string,
    0, //bool
    memclear_table,
    0, //userfunc
    0, //libfunc
    0, //nill
    0 //undef
};

void avm_memcellclear(avm_memcell_p m) {
    if (m->type != undef_m) {
        memclear_func_t f = memclearFuncs[m->type];
        if (f != NULL) {
            (*f)(m);
        }
        m->type = undef_m;
    }
}

char* avm_tostring(avm_memcell_p cell) {

    switch (cell->type) {
        case number_m:
        {
            char* numStr = malloc(100 * sizeof (char));
            sprintf(numStr, "%.3lf", cell->data.numVal);
            return numStr;
        }

        case string_m: return strdup(cell->data.strVal);
        case bool_m:
        {
            if ((unsigned int) cell->data.boolVal == 1) {
                return strdup("true");
            }
            return strdup("false");
        }

        case table_m:
        {
            return strdup("_table");
        }
        case userfunc_m:
        {

            userfunc_p f = avm_getuserfuncs(cell->data.funcVal);

            char* numStr = malloc(100 * sizeof (char));
            sprintf(numStr, "%d", f->address);

            int N = (14 + strlen(numStr) + 2 + strlen(f->id) + 1) + 1;

            char* ret_str = malloc(N * sizeof (char));

            strcpy(ret_str, "user function ");
            strcat(ret_str, numStr);
            strcat(ret_str, " (");
            strcat(ret_str, f->id);
            strcat(ret_str, ")");

            free(numStr);

            return ret_str;
        }
        case libfunc_m:
        {
            int N = 17 + strlen(cell->data.libfuncVal) + 1;

            char* ret_str = malloc(N * sizeof (char));

            strcpy(ret_str, "library function ");
            strcat(ret_str, cell->data.libfuncVal);

            return ret_str;
        }
        case nil_m:
        {
            return strdup("nil");
        }
        case undef_m:
        {
            executeFinished = 1;
            return NULL;
        }
        default:
        {
            assert(0);
        }
    }
}

int avm_warning_check(char* msg) {
    if (warning_line == currLine && warning_msg != NULL) {
        if (strcmp(warning_msg, msg) == 0) {
            return 0;
        }
    }
    free(warning_msg);
    warning_msg = NULL;
    warning_line = currLine;
    warning_msg = strdup(msg);
    return 1;
}

