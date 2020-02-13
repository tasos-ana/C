#ifndef QUAD_H
#define	QUAD_H

#include "expr.h"
#include "dataStructs.h"

enum iopcode {
    add, sub, mul,
    _div, mod,
    tablecreate, tablegetelem, tablesetelem,
    assign, jump,
    if_eq, if_noteq,
    if_greater, if_greatereq,
    if_less, if_lesseq,
    not,
    param, call, getretval,
    funcstart, ret, funcend, uminus, and, or
};

typedef struct quad_s {
    enum iopcode op;
    expr_p result;
    expr_p arg1;
    expr_p arg2;
    unsigned int label;
    unsigned int line;

    unsigned int taddress;
} quad_s, *quad_p;

void expand(void);

void emit(enum iopcode op, expr_p arg1, expr_p arg2, expr_p result,
        unsigned int label, unsigned int line);

expr_p emit_iftableitem(expr_p e);
expr_p make_call(expr_p lval, expr_p elist);
void emit_params(expr_p elist);
expr_p reverse_elist(expr_p elist);
unsigned int nextquad(void);

void patchlabel(unsigned int quadNo, unsigned int label);
void backpatch(list_p head, int label);

void backpatch_expr(expr_p e);


char * quadName(int op);
int isEntry(int type);
int isStringConst(int type);
int isNumConst(int type);
int isBoolConst(int type);
int isBranch(int type);

void printExpr(expr_p ex, int i);

void printQuads();

void writeQuads(FILE * f);
void writeExpr(expr_p ex, int i, FILE * f);

struct compileError {
    char * message;
    unsigned int line;
    struct compileError * next;
};

struct compileError *create_comperror(const char* msg, unsigned int lineNum);

void comperror(const char* format, int line);

void compileError_print(void);



#endif	/* QUAD_H */
