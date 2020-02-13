#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "quad.h"
#include "expr.h"
#include "symbolTable.h"
#include "dataStructs.h"
#include "manage.h"

#define EXPAND_SIZE  1024
#define CURR_SIZE    (total * sizeof(quad_s))
#define NEW_SIZE     (EXPAND_SIZE * sizeof(quad_s) + CURR_SIZE)

extern int line;
quad_p quads = NULL;
unsigned int total = 0; // ta sunolika quad gia ta opoia exoume parei mnhmh
unsigned int currQuad = 0; // einai to index katw apo to teleutaio quad

struct compileError* comperror_list = NULL;

void expand(void) {
    assert(total == currQuad);
    quad_p p = (quad_p) malloc(NEW_SIZE);
    assert(p != NULL);
    if (quads != NULL) {
        memmove(p, quads, CURR_SIZE);
        free(quads);
    }
    quads = p;
    total += EXPAND_SIZE;
}

void emit(enum iopcode op, expr_p arg1, expr_p arg2, expr_p result,
        unsigned int label, unsigned int line) {

    if (currQuad == total) {
        expand();
    }

    quad_p p = quads + currQuad; // deixnei katw apo to teleutaio quad
    ++currQuad;

    p->op = op;
    p->arg1 = arg1;
    p->arg2 = arg2;
    p->result = result;
    p->label = label;
    p->line = line;
}

expr_p emit_iftableitem(expr_p e) {
    if (e == NULL) {
        return NULL;
    }
    if (e->type != tableitem_e) {
        return e;
    }
    expr_p result = newexpr(var_e);
    result->sym = (SymbolTableEntry*) newTemp();
    emit(tablegetelem, e, e->index, result, 0, line);
    return result;
}

expr_p make_call(expr_p lval, expr_p elist) {
    if (lval == NULL) {
        return NULL;
    }
    expr_p func = emit_iftableitem(lval);
    emit_params(elist);
    emit(call, func, NULL, NULL, 0, line);
    expr_p result = newexpr(var_e);
    result->sym = (SymbolTableEntry*) newTemp();
    emit(getretval, NULL, NULL, result, 0, line);

    return result;
}

void emit_params(expr_p elist) {
    elist = (expr_p) reverse_elist(elist);
    while (elist != NULL) {
        if (elist->type == boolexpr_e && elist->sym == NULL) {
            elist->sym = newTemp();
        }
        emit(param, elist, NULL, NULL, 0, line);
        elist = elist->next;
    }
}

expr_p reverse_elist(expr_p elist) {
    expr_p prev = NULL;
    expr_p head = elist;
    expr_p tmp;
    while (head != NULL) {
        tmp = head->next;
        head->next = prev;
        prev = head;
        head = tmp;
    }
    return prev;
}

unsigned int nextquad(void) {
    return currQuad;
}

void patchlabel(unsigned int quadNo, unsigned int label) {
    assert(quadNo < currQuad);
    quads[quadNo].label = label;
}

void backpatch(list_p head, int label) {
    while (head != NULL) {
        patchlabel(head->label_incJump, label);
        head = head->next;
    }
}

void backpatch_expr(expr_p e) {

    if (e->truelist == NULL && e->falselist == NULL) {
        return;
    }

    if (e->type == constbool_e) {
        backpatch(e->truelist, nextquad());
        backpatch(e->falselist, nextquad());
        return;
    }

    e->sym = newTemp();
    backpatch(e->truelist, nextquad());
    backpatch(e->falselist, nextquad() + 2);
    emit(assign, newexpr_constbool(1), NULL, e, 0, line);
    emit(jump, NULL, NULL, NULL, nextquad() + 2, line);
    emit(assign, newexpr_constbool(0), NULL, e, 0, line);
}

void printQuads() {
    int i;
    if (currQuad != 0) {
        printf("\n- - - - - - - - - - - - - Q U A D S - - - - - - - - - - - - -\n");
    }
    for (i = 0; i < currQuad; ++i) {
        printf("%3d:%5s%-15s", i, "", quadName(quads[i].op));
        if (quads[i].op == jump) {
            printf("%45d", quads[i].label);
        } else {
            if (quads[i].arg1 != NULL) {
                printExpr(quads[i].arg1, i);
            }
            if (quads[i].arg2 != NULL) {
                printExpr(quads[i].arg2, i);
            }
            if (quads[i].result != NULL) {
                printExpr(quads[i].result, i);
            }
            if (isBranch(quads[i].op)) {
                printf("%15u", quads[i].label);
            }
        }
        printf("\n");
    }
}

char * quadName(int op) {
    switch (op) {
        case assign: return "ASSIGN";
        case add: return "ADD";
        case sub: return "SUB";
        case mul: return "MUL";
        case _div: return "DIV";
        case mod: return "MOD";
        case uminus: return "UMINUS";
        case and: return "AND";
        case or: return "OR";
        case not: return "NOT";
        case if_eq: return "IF_EQ";
        case if_noteq: return "IF_NOTEQ";
        case if_lesseq: return "IF_LESSEQ";
        case if_greatereq: return "IF_GREATEREQ";
        case if_less: return "IF_LESS";
        case if_greater: return "IF_GREATER";
        case call: return "CALL";
        case param: return "PARAM";
        case ret: return "RETURN";
        case getretval: return "GETRETVAL";
        case funcstart: return "FUNCSTART";
        case funcend: return "FUNCEND";
        case tablecreate: return "TABLECREATE";
        case tablegetelem: return "TABLEGETELEM";
        case tablesetelem: return "TABLESETELEM";
        case jump: return "JUMP";
        default: assert(0);
    }
}

int isEntry(int type) {
    if (type < 8) {
        return 1;
    }
    return 0;
}

int isStringConst(int type) {
    if (type == 10) {
        return 1;
    }
    return 0;
}

int isNumConst(int type) {
    if (type == 8) {
        return 1;
    }
    return 0;
}

int isBoolConst(int type) {
    if (type == 9) {
        return 1;
    }
    return 0;
}

int isBranch(int type) {
    if (type >= 10 && type <= 15) {
        return 1;
    }
    return 0;
}

void printExpr(expr_p ex, int i) {

    if (isEntry(ex->type)) {
        assert(ex->sym != NULL);
        printf("%15s", ex->sym->value.varVal->name);
    } else if (isStringConst(ex->type)) {
        printf("%15s", ex->strConst);
    } else if (isNumConst(ex->type)) {
        printf("%15.3f", ex->numConst);
    } else if (isBoolConst(ex->type)) {
        if (ex->boolConst == 'T') {
            printf("%15s", "true");
        } else if (ex->boolConst == 'F') {
            printf("%15s", "false");
        } else {
            assert(0);
        }
    } else {
        if (ex->type != 11) {
            assert(0);
        }
        printf("%15s", "nil");
    }
}

void writeQuads(FILE * f) {
    int i;
    for (i = 0; i < currQuad; ++i) {
        fprintf(f, "%3d:%5s%-10s", i, "", quadName(quads[i].op));
        if (quads[i].op == jump) {
            fprintf(f, "%45d", quads[i].label);
        } else {
            if (quads[i].arg1 != NULL) {
                writeExpr(quads[i].arg1, i, f);
            }
            if (quads[i].arg2 != NULL) {
                writeExpr(quads[i].arg2, i, f);
            }
            if (quads[i].result != NULL) {
                writeExpr(quads[i].result, i, f);
            }
            if (isBranch(quads[i].op)) {
                fprintf(f, "%15u", quads[i].label);
            }
        }
        fprintf(f, "\n");
    }
}

void writeExpr(expr_p ex, int i, FILE * f) {
    if (isEntry(ex->type)) {
        assert(ex != NULL);
        assert(ex->sym != NULL);
        assert(ex->sym->value.varVal != NULL);
        fprintf(f, "%15s", ex->sym->value.varVal->name);
    } else if (isStringConst(ex->type)) {
        fprintf(f, "%15s", ex->strConst);
    } else if (isNumConst(ex->type)) {
        fprintf(f, "%15f", ex->numConst);
    } else if (isBoolConst(ex->type)) {
        if (ex->boolConst == 'T') {
            fprintf(f, "%15s", "true");
        } else {
            fprintf(f, "%15s", "false");
        }
    } else {
        if (ex->type != 11) {
            assert(0);
        }
        fprintf(f, "%15s", "nil");
    }
}

struct compileError *create_comperror(const char* msg, unsigned int lineNum) {
    struct compileError* tmp = malloc(sizeof (struct compileError));
    tmp->message = strdup(msg);
    tmp->line = line;
    tmp->next = NULL;
    return tmp;
}

void comperror(const char* format, int line) {
    struct compileError* newerr = create_comperror(format, line);
    struct compileError* curr = comperror_list;
    if (curr == NULL) {
        comperror_list = newerr;
        return;
    }

    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newerr;
}

void compileError_print(void) {
    struct compileError* curr = comperror_list;
    if (curr != NULL) {
        printf("\n\n");
        printf("- - - - - - - - - C O M P I L E  E R R O R S - - - - - - - - -\n\n");
    }

    while (curr != NULL) {
        printf("Line: %2d\tCompile Error:  %s \n", curr->line, curr->message);
        curr = curr->next;
    }
    printf("\n");
}



