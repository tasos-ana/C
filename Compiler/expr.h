#ifndef EXPR_H
#define	EXPR_H

#include "symbolTable.h"
#include "dataStructs.h"

enum expr_t {
    var_e, tableitem_e,
    programfunc_e, libraryfunc_e,
    arithexpr_e, boolexpr_e,
    assignexpr_e, newtable_e,

    constnum_e, constbool_e, conststring_e,

    nil_e
};

typedef struct expr {
    enum expr_t type;
    SymbolTableEntry* sym;
    struct expr * index;
    double numConst;
    char * strConst;
    unsigned char boolConst;
    list_p truelist;
    list_p falselist;
    struct expr * next;
} *expr_p;


expr_p newexpr(enum expr_t type);

expr_p lvalue_expr(SymbolTableEntry * sym);

expr_p member_item(expr_p lval, const char * id);

expr_p newexpr_conststring(const char *idname);

expr_p newexpr_constnum(double num);

expr_p newexpr_constbool(int i);

struct call_info {
    expr_p elist;
    unsigned char isMethod;
    const char * name;
};

struct call_info * create_call_info(void);

int checkuminus(expr_p e);


#endif	/* EXPR_H */
