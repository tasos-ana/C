#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "expr.h"
#include "quad.h"
#include "dataStructs.h"

extern int line;

expr_p newexpr(enum expr_t type) {
    expr_p newExpr = (expr_p) malloc(sizeof (struct expr));
    newExpr->type = type;

    newExpr->boolConst = 'u';
    newExpr->index = NULL;
    newExpr->next = NULL;
    newExpr->numConst = 0;
    newExpr->strConst = NULL;
    newExpr->sym = NULL;

    newExpr->truelist = NULL;
    newExpr->falselist = NULL;
    return newExpr;
}

expr_p lvalue_expr(SymbolTableEntry * sym) {
    assert(sym != NULL);
    expr_p e = (expr_p) malloc(sizeof (struct expr));
    memset(e, 0, sizeof (struct expr));

    e->boolConst = 'u';

    e->truelist = NULL;
    e->falselist = NULL;

    e->next = NULL;
    e->sym = sym;

    switch (sym->type) {
        case _GLOBAL: e->type = var_e;
            break;
        case _LOCAL: e->type = var_e;
            break;
        case _TEMPVAR: e->type = var_e;
            break;
        case _FORMAL: e->type = var_e;
            break;
        case _USERFUNC: e->type = programfunc_e;
            break;
        case _LIBFUNC: e->type = libraryfunc_e;
            break;
        default: assert(0);
    }
    return e;
}

expr_p member_item(expr_p lval, const char * id) {
    lval = emit_iftableitem(lval);
    expr_p item = newexpr(tableitem_e);
    item->sym = lval->sym;
    item->index = newexpr_conststring(id);
    return item;
}

expr_p newexpr_conststring(const char *idname) {
    expr_p e = newexpr(conststring_e);

    int len = strlen(idname) + 3;

    char s[len];

    strcpy(s, "\"");
    strcat(s, idname);
    strcat(s, "\"");

    e->strConst = strdup(s);
    return e;
}

expr_p newexpr_constnum(double num) {
    expr_p e = newexpr(constnum_e);
    e->numConst = num;
    return e;
}

expr_p newexpr_constbool(int i) {
    expr_p e = newexpr(constbool_e);
    if (i == 1) {
        e->boolConst = 'T';
    } else {
        e->boolConst = 'F';
    }
    return e;
}

struct call_info * create_call_info(void) {
    struct call_info * newCall_info = malloc(sizeof (struct call_info));

    newCall_info->elist = NULL;
    newCall_info->isMethod = 'u';
    newCall_info->name = NULL;

    return newCall_info;
}

int checkuminus(expr_p e) {
    if (e->type == constbool_e ||
            e->type == conststring_e ||
            e->type == nil_e ||
            e->type == newtable_e ||
            e->type == programfunc_e ||
            e->type == libraryfunc_e ||
            e->type == boolexpr_e) {
        comperror("Illegal expr to unary minus", line);
        return 0;
    }
    return 1;
}
