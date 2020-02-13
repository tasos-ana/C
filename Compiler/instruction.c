#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "instruction.h"
#include "expr.h"
#include "symbolTable.h"
#include "dataStructs.h"
#include "quad.h"

#define EXPAND_SIZE  1024
#define INSTR_CURR_SIZE    (instr_total * sizeof(instr_s))
#define INSTR_NEW_SIZE     (EXPAND_SIZE * sizeof(instr_s) + INSTR_CURR_SIZE)

extern unsigned int currQuad;
extern quad_p quads;
extern instr_list_p instr_head;
extern instr_list_p instr_tail;

extern double * numConsts;
extern char ** stringConsts;
extern char ** namedLibfuncs;
extern userfunc_p userFuncs;


instr_p instructions = NULL;
unsigned int instr_total = 0;
unsigned int currInstr = 0;

void instr_expand(void) {
    assert(instr_total == currInstr);
    instr_p p = (instr_p) malloc(INSTR_NEW_SIZE);
    assert(p != NULL);
    if (instructions != NULL) {
        memmove(p, instructions, INSTR_CURR_SIZE);
    }
    instructions = p;
    instr_total += EXPAND_SIZE;
}

void instr_emit(instr_s t) {

    if (currInstr == instr_total) {
        instr_expand();
    }

    instr_p instr = &instructions[currInstr]; // deixnei katw apo to teleutaio instr
    ++currInstr;

    instr->opcode = t.opcode;

    instr->arg1 = t.arg1;
    instr->arg2 = t.arg2;
    instr->result = t.result;

    instr->line = t.line;
}

unsigned int nextinstr(void) {
    return currInstr;
}

void instr_reset(instr_p t) {
    t->arg1 = NULL;
    t->arg2 = NULL;
    t->result = NULL;
}

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void make_operand(expr_p e, vmarg_p* arg) {
    (*arg) = (vmarg_p) malloc(sizeof (vmarg_s));
    if (e == NULL) {
        (*arg) = NULL;
        return;
    }

    switch (e->type) {
        case assignexpr_e:
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e:
        {
            assert(e->sym != NULL);
            instr_list_insert(e->sym->value.varVal->name);
            (*arg)->val = e->sym->value.varVal->offset;

            switch (e->sym->value.varVal->space) {
                case programvar: (*arg)->type = global_a;
                    break;
                case functionlocal: (*arg)->type = local_a;
                    break;
                case formalarg: (*arg)->type = formal_a;
                    break;
                default: assert(0);
            }
            break;
        }

        case constbool_e:
        {
            (*arg)->val = 1;
            if (e->boolConst == 'F') {
                (*arg)->val = 0;
            }

            //(*arg)->val = e->boolConst;
            (*arg)->type = bool_a;
            break;
        }

        case conststring_e:
        {
            (*arg)->val = consts_newstring(e->strConst);
            (*arg)->type = string_a;
            break;
        }

        case constnum_e:
        {
            (*arg)->val = consts_newnumber(e->numConst);
            (*arg)->type = number_a;
            break;
        }

        case nil_e:
        {
            (*arg)->type = nil_a;
            break;
        }

        case programfunc_e:
        {
            (*arg)->type = userfunc_a;
            (*arg)->val = userfuncs_newfunc(e->sym);
            break;
        }

        case libraryfunc_e:
        {
            (*arg)->type = libfunc_a;
            (*arg)->val = libfuncs_newused(e->sym->value.funcVal->name);
            break;
        }

        default: assert(0);
    }
}

void make_numberoperand(vmarg_p* arg, double val) {
    (*arg) = (vmarg_p) malloc(sizeof (vmarg_s));
    (*arg)->val = consts_newnumber(val);
    (*arg)->type = number_a;
}

void make_booloperand(vmarg_p* arg, unsigned int val) {
    (*arg) = (vmarg_p) malloc(sizeof (vmarg_s));
    (*arg)->val = val;
    (*arg)->type = bool_a;
}

void make_retvaloperand(vmarg_p* arg) {
    (*arg) = (vmarg_p) malloc(sizeof (vmarg_s));
    (*arg)->type = retval_a;
}

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void instr_patchlabel(unsigned int instrNo, unsigned int label) {
    assert(instrNo < currInstr);
    assert(instructions[instrNo].result->type == label_a);
    instructions[instrNo].result->val = label;
}

void instr_backpatch(list_p head, int label) {
    while (head != NULL) {
        instr_patchlabel(head->label_incJump, label);
        head = head->next;
    }
}

incJump_p ij_head = NULL;
incJump_p ij_tail = NULL;

void add_incomplete_jump(unsigned int instrNo, unsigned int iaddress) {
    incJump_p new_incJump = (incJump_p) malloc(sizeof (incJump_s));

    new_incJump->instrNo = instrNo;
    new_incJump->iaddress = iaddress;

    if (ij_head == NULL) {
        ij_head = new_incJump;
        ij_tail = ij_head;
    } else {
        ij_tail->next = new_incJump;
        ij_tail = new_incJump;
    }
}

void patch_incomplete_jumps() {
    incJump_p curr = ij_head;
    while (curr != NULL) {
        assert(instructions[curr->instrNo].result->type == label_a);
        if (curr->iaddress == currQuad) {
            instructions[curr->instrNo].result->val = currInstr;
        } else {
            if (quads[curr->iaddress].op == funcstart) {
                instructions[curr->instrNo].result->val = quads[curr->iaddress].taddress - 1;
            } else {
                instructions[curr->instrNo].result->val = quads[curr->iaddress].taddress;
            }
        }
        curr = curr->next;
    }
}

void instr_print(void) {
    if (currInstr != 0) {
        printf("\n- - - - - - - - - - - I N S T R U C T I O N S - - - - - - - - - - -\n");
    }
    int i;
    instr_p data;
    instr_list_p currVar = instr_head;

    for (i = 0; i < currInstr; ++i) {
        assert(instructions != NULL);
        data = &instructions[i];
        assert(data != NULL);

        printf("%3d:%5s%-15s", i, "", instrName(data->opcode));

        if (data->arg1 != NULL) {
            printf("%11d,", data->arg1->type);
            decode(data->arg1, &currVar);
        }
        if (data->arg2 != NULL) {
            printf("%10d,", data->arg2->type);
            decode(data->arg2, &currVar);
        }
        if (data->result != NULL) {
            printf("%10d,", data->result->type);
            decode(data->result, &currVar);
        }

        printf("\n");
    }
}

char* instrName(int op) {
    switch (op) {
        case assign_v: return "ASSIGN";
        case add_v: return "ADD";
        case sub_v: return "SUB";
        case mul_v: return "MUL";
        case div_v: return "DIV";
        case mod_v: return "MOD";
        case jeq_v: return "JEQ";
        case jne_v: return "JNE";
        case jle_v: return "JLE";
        case jge_v: return "JGE";
        case jlt_v: return "JLT";
        case jgt_v: return "JGT";
        case callfunc_v: return "CALLFUNC";
        case pusharg_v: return "PUSHARG";
        case funcenter_v: return "FUNCENTER";
        case funcexit_v: return "FUNCEXIT";
        case newtable_v: return "NEWTABLE";
        case tablegetelem_v: return "TABLEGETELEM";
        case tablesetelem_v: return "TABLESETELEM";
        case jump_v: return "JUMP";
        default: assert(0);
    }
}

void decode(vmarg_p arg, instr_list_p * currVar) {
    if (arg->type == retval_a || arg->type == nil_a) {
        return;
    }



    if (arg->type == label_a || arg->type == bool_a) {
        printf("%-12d", arg->val);
        return;
    } else {
        printf("%d", arg->val);
    }

    if (arg->type == global_a || arg->type == formal_a || arg->type == local_a) {
        instr_list_p tmp = *currVar;
        *currVar = tmp->next;
        printf(":%-10s", tmp->name);
        return;
    }
    if (arg->type == number_a) {
        double d = numConsts[arg->val];
        printf(":%-10.3lf", d);
        return;
    }
    if (arg->type == string_a) {
        char * s = stringConsts[arg->val];
        printf(":%-10s", s);
        return;
    }
    if (arg->type == userfunc_a) {
        userfunc_p f = &userFuncs[arg->val];
        printf(":%-10s", f->id);
        return;
    }
    if (arg->type == libfunc_a) {
        char * lib = namedLibfuncs[arg->val];
        printf(":%-10s", lib);
        return;
    }
}
