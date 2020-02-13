#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "quad.h"
#include "instruction.h"
#include "generator.h"
#include "dataStructs.h"

extern quad_p quads;

extern unsigned int currQuad;

int currProcessQuad = 0;

int enterFunc = 0;

void generate(enum vmopcode op, quad_p quad) {
    instr_s t;
    instr_reset(&t);

    t.line = quad->line;
    t.opcode = op;

    make_operand(quad->arg1, &t.arg1);
    make_operand(quad->arg2, &t.arg2);
    make_operand(quad->result, &t.result);

    quad->taddress = nextinstr();

    instr_emit(t);
}

void generate_relational(enum vmopcode op, quad_p quad) {
    instr_s t;

    instr_reset(&t);

    t.line = quad->line;
    t.opcode = op;

    make_operand(quad->arg1, &t.arg1);
    make_operand(quad->arg2, &t.arg2);
    t.result = (vmarg_p) malloc(sizeof (vmarg_s));

    t.result->type = label_a;

    if (quad->label < currProcessQuad) {
        t.result->val = quads[quad->label].taddress;
    } else {
        add_incomplete_jump(nextinstr(), quad->label);
    }
    quad->taddress = nextinstr();
    instr_emit(t);
}

void reset_operand(vmarg_p* arg) {
    *arg = NULL;
}

void generate_ADD(quad_p quad) {
    generate(add_v, quad);
}

void generate_SUB(quad_p quad) {
    generate(sub_v, quad);
}

void generate_MUL(quad_p quad) {
    generate(mul_v, quad);
}

void generate_DIV(quad_p quad) {
    generate(div_v, quad);
}

void generate_MOD(quad_p quad) {
    generate(mod_v, quad);
}

void generate_NEWTABLE(quad_p quad) {
    generate(newtable_v, quad);
}

void generate_TABLEGETELEM(quad_p quad) {
    generate(tablegetelem_v, quad);
}

void generate_TABLESETELEM(quad_p quad) {
    generate(tablesetelem_v, quad);
}

void generate_ASSIGN(quad_p quad) {
    generate(assign_v, quad);
}

/*
void generate_NOP(void){
    instr_s t;
    instr_reset(&t);

    t.opcode = nop_v;
}
 */

void generate_JUMP(quad_p quad) {
    generate_relational(jump_v, quad);
}

void generate_IF_EQ(quad_p quad) {
    generate_relational(jeq_v, quad);
}

void generate_IF_NOTEQ(quad_p quad) {
    generate_relational(jne_v, quad);
}

void generate_IF_GREATER(quad_p quad) {
    generate_relational(jgt_v, quad);
}

void generate_IF_GREATEREQ(quad_p quad) {
    generate_relational(jge_v, quad);
}

void generate_IF_LESS(quad_p quad) {
    generate_relational(jlt_v, quad);
}

void generate_IF_LESSEQ(quad_p quad) {
    generate_relational(jle_v, quad);
}

void generate_NOT(quad_p quad) {
    quad->taddress = nextinstr();

    instr_s t;
    instr_reset(&t);

    t.line = quad->line;
    t.opcode = jeq_v;

    make_operand(quad->arg1, &t.arg1);
    make_booloperand(&t.arg2, 0);
    t.result = (vmarg_p) malloc(sizeof (vmarg_s));
    t.result->type = label_a;
    t.result->val = nextinstr() + 3;
    instr_emit(t);

    t.opcode = assign_v;
    make_booloperand(&t.arg1, 0);
    reset_operand(&t.arg2);
    make_operand(quad->result, &t.result);
    instr_emit(t);

    t.opcode = jump_v;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    t.result = (vmarg_p) malloc(sizeof (vmarg_s));
    t.result->type = label_a;
    t.result->val = nextinstr() + 2;
    instr_emit(t);

    t.opcode = assign_v;
    make_booloperand(&t.arg1, 1);
    reset_operand(&t.arg2);
    make_operand(quad->result, &t.result);
    instr_emit(t);
}

void generate_PARAM(quad_p quad) {
    quad->taddress = nextinstr();
    instr_s t;
    instr_reset(&t);

    t.line = quad->line;
    t.opcode = pusharg_v;

    make_operand(quad->arg1, &t.arg1);
    instr_emit(t);
}

void generate_CALL(quad_p quad) {
    quad->taddress = nextinstr();
    instr_s t;
    instr_reset(&t);

    t.line = quad->line;
    t.opcode = callfunc_v;

    make_operand(quad->arg1, &t.arg1);
    instr_emit(t);
}

void generate_GETRETVAL(quad_p quad) {
    quad->taddress = nextinstr();
    instr_s t;
    instr_reset(&t);

    t.line = quad->line;
    t.opcode = assign_v;

    make_operand(quad->result, &t.result);
    make_retvaloperand(&t.arg1);
    instr_emit(t);
}

void generate_FUNCSTART(quad_p quad) {
    instr_s t;
    instr_reset(&t);

    t.line = quad->line;

    t.opcode = jump_v;

    t.result = (vmarg_p) malloc(sizeof (vmarg_s));
    t.result->type = label_a;

    SymbolTableEntry *f = quad->result->sym;
    f->value.funcVal->returnList = append_list(f->value.funcVal->returnList, nextinstr());

    instr_emit(t);

    // --------------------------------
    instr_reset(&t);

    enterFunc = 1;

    f->value.funcVal->taddress = nextinstr();
    quad->taddress = nextinstr();

    f_push(&funcstack, f);

    t.line = quad->line;
    t.opcode = funcenter_v;

    make_operand(quad->result, &t.result);
    enterFunc = 0;
    instr_emit(t);
}

void generate_RETURN(quad_p quad) {
    quad->taddress = nextinstr();
    instr_s t;
    instr_reset(&t);

    if (quad->arg1 != NULL) {
        t.line = quad->line;
        t.opcode = assign_v;

        make_retvaloperand(&t.result);
        make_operand(quad->arg1, &t.arg1);

        instr_emit(t);
    }

    SymbolTableEntry* f = f_top(funcstack);

    f->value.funcVal->returnList = append_list(f->value.funcVal->returnList, nextinstr());

    t.opcode = jump_v;

    reset_operand(&t.arg1);
    reset_operand(&t.arg2);

    t.result = (vmarg_p) malloc(sizeof (vmarg_s));
    t.result->type = label_a;

    instr_emit(t);
}

void generate_FUNCEND(quad_p quad) {
    SymbolTableEntry* f = f_pop(&funcstack);
    assert(f->value.funcVal->returnList != NULL);

    list_p inc_jump_funcstart = f->value.funcVal->returnList;

    list_p inc_jump_funcreturn = (f->value.funcVal->returnList)->next;

    inc_jump_funcstart->next = NULL;

    instr_backpatch(inc_jump_funcstart, nextinstr() + 1);
    instr_backpatch(inc_jump_funcreturn, nextinstr());

    inc_jump_funcstart->next = inc_jump_funcreturn;

    quad->taddress = nextinstr();
    instr_s t;
    instr_reset(&t);
    t.line = quad->line;

    t.opcode = funcexit_v;

    make_operand(quad->result, &t.result);
    instr_emit(t);
}

generator_func_t generators[] = {
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_ASSIGN,
    generate_JUMP,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_GREATER,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_LESSEQ,
    generate_NOT,
    generate_PARAM,
    generate_CALL,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_RETURN,
    generate_FUNCEND
};

void generate_tc(void) {
    unsigned int i;
    for (i = 0; i < currQuad; ++i) {
        (*generators[quads[i].op])(quads + i);
        currProcessQuad++;
    }
}




