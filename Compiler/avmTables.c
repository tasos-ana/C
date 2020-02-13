#include <assert.h>
#include <stdlib.h>

#include "avm.h"
#include "avmTables.h"
#include "avmHashtable.h"
#include "avmOps.h"
#include "instruction.h"

extern avm_memcell_s avm_stack[];
extern avm_memcell_s ax, bx, retval;

extern unsigned char executeFinished;
extern unsigned int currLine;

extern char* typeStrings[];

extern unsigned int top;

extern unsigned int pc;

unsigned int from_exe_set = 0;

void execute_newtable(instr_p instr) {
    avm_memcell_p lv = avm_translate_operand(instr->arg1, (avm_memcell_p) 0);
    assert(lv != NULL && (((&(avm_stack[AVM_STACKSIZE - 1]) >= lv) && (lv > &(avm_stack[top]))) || lv == &retval));

    avm_memcellclear(lv);

    lv->type = table_m;
    lv->data.tableVal = avm_tablenew();

    avm_tableincrefcounter(lv->data.tableVal);
}

void execute_tablegetelem(instr_p instr) {
    avm_memcell_p lv = avm_translate_operand(instr->result, (avm_memcell_p) 0);
    avm_memcell_p t = avm_translate_operand(instr->arg1, (avm_memcell_p) 0);
    avm_memcell_p i = avm_translate_operand(instr->arg2, &ax);

    assert(lv != NULL && (((&(avm_stack[AVM_STACKSIZE - 1]) >= lv) && (lv > &(avm_stack[top]))) || lv == &retval));
    assert(t != NULL && ((&(avm_stack[AVM_STACKSIZE - 1]) >= t) && (t > &(avm_stack[top]))));
    assert(i != NULL);

    avm_memcellclear(lv);
    lv->type = nil_m; /*default value*/

    if (t->type != table_m) {
        avm_error("%u: Error: illegal use of type %s as table!\n", currLine, typeStrings[t->type]);
        executeFinished = 1;
    } else {
        avm_memcell_p content = (avm_memcell_p) avm_tablegetelem(t->data.tableVal, i);
        if (content != NULL) {
            avm_assign(lv, content);
        } else {
            char* is = (char*) avm_tostring(i);

            if (executeFinished == 1) {
                is = strdup("'undef'");
                executeFinished = 0;
            }

            char* msg = "not found!";
            if (avm_warning_check(msg) == 1) {
                avm_warning("%u: Warning: index[%s] on specified table %s\n", currLine, is, msg);
            }
            free(is);
        }
    }
}

void execute_tablesetelem(instr_p instr) {
    avm_memcell_p t = avm_translate_operand(instr->arg1, (avm_memcell_p) 0);
    avm_memcell_p i = avm_translate_operand(instr->arg2, &ax);
    avm_memcell_p c = avm_translate_operand(instr->result, &bx);

    assert(t != NULL && ((&(avm_stack[AVM_STACKSIZE - 1]) >= t) && (t > &(avm_stack[top]))));
    assert(i != NULL && c != NULL);

    if (t->type != table_m) {
        avm_error("%u: Error: illegal use of type %s as table!\n", currLine, typeStrings[t->type]);
    } else {
        if (c->type == undef_m) {
            char * msg = "assigning 'undef' content to table!";

            if (avm_warning_check(msg) == 1) {
                avm_warning("%u: Warning:  %s\n", currLine, msg);
            }
        }
        from_exe_set = 1;
        avm_tablesetelem(t->data.tableVal, i, c);
        from_exe_set = 0;
    }
}

void execute_nop(instr_p instr) {

}

void avm_tableincrefcounter(avm_table_p t) {
    (++(t->refCounter));
}

void avm_tabledecrefcounter(avm_table_p t) {
    if (t->refCounter <= 0) {
        printf("%u--------------------------------\n", t->refCounter);
    }
    assert(t->refCounter > 0);
    if ((--(t->refCounter)) == 0) {
        avm_tabledestroy(t);

    }

}
