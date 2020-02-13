#ifndef MANAGE_H
#define	MANAGE_H

#include "expr.h"
#include "dataStructs.h"

struct for_info {
    int test;
    int enter;
};

struct for_info * create_for_info(void);

struct stmt_list_info {
    list_p breaklist;
    list_p continuelist;
};

struct stmt_list_info* create_stmt_list_info(void);

int check_value_modification();

int valid_arithexpr(enum expr_t type);

int isUndefined(enum expr_t type);

struct stmt_list_info* manage_stmts(struct stmt_list_info* stmts, struct stmt_list_info* stmt);

struct stmt_list_info * manage_stmt_expr(expr_p e);

struct stmt_list_info * manage_stmt_ifstmt(struct stmt_list_info * stmt);

struct stmt_list_info* manage_break(void);
struct stmt_list_info* manage_continue(void);

struct stmt_list_info * manage_stmt_block(struct stmt_list_info* stmt);

expr_p manage_expr_assignexpr(expr_p e);

expr_p manage_exprPLUSexpr(expr_p e1, expr_p e2);
expr_p manage_exprMINUSexpr(expr_p e1, expr_p e2);
expr_p manage_exprMULexpr(expr_p e1, expr_p e2);
expr_p manage_exprDIVexpr(expr_p e1, expr_p e2);
expr_p manage_exprMODexpr(expr_p e1, expr_p e2);
expr_p manage_exprGREATERexpr(expr_p e1, expr_p e2);
expr_p manage_exprGREATER_EQUALexpr(expr_p e1, expr_p e2);
expr_p manage_exprLESSexpr(expr_p e1, expr_p e2);
expr_p manage_exprLESS_EQUALexpr(expr_p e1, expr_p e2);
expr_p manage_exprEQUALexpr(expr_p e1, expr_p e2);
expr_p manage_exprNOT_EQUALexpr(expr_p e1, expr_p e2);

expr_p manage_andprefix(expr_p e1);
expr_p manage_exprANDexpr(expr_p e1, int M, expr_p e2);

expr_p manage_orprefix(expr_p e1);
expr_p manage_exprORexpr(expr_p e1, int M, expr_p e2);

expr_p manage_orprefix(expr_p e1);
expr_p manage_andprefix(expr_p e1);

expr_p manage_expr_term(expr_p e);

int manage_M(void);

expr_p manage_term_expr(expr_p e);

expr_p manage_term_uminus(expr_p e);
expr_p manage_term_NOT(expr_p e);

expr_p manage_term_lvalue_PP(expr_p lval);
expr_p manage_term_lvalue_MM(expr_p lval);

expr_p manage_term_PP_lvalue(expr_p lval);
expr_p manage_term_MM_lvalue(expr_p lval);

expr_p manage_term_primary(expr_p e);

expr_p manage_assignexpr(expr_p lval, expr_p e);

expr_p manage_assignlvalue(expr_p lval);

expr_p manage_primary_call(expr_p e);

expr_p manage_primary_object(expr_p e);

expr_p manage_primary_funcdef(SymbolTableEntry* funcdef);

expr_p manage_primary_const(expr_p e);

expr_p manage_lvalue_ID(const char * id);
expr_p manage_lvalue_LOCAL_ID(const char * id);
expr_p manage_lvalue_CCOLON_ID(const char * id);
expr_p manage_lvalue_member(expr_p e);

expr_p manage_member_tableitem(expr_p e);
expr_p manage_tableitem_lvalue_dot_id(expr_p lval, const char * id);
expr_p manage_tableitem_lvalue_expr(expr_p lval, expr_p e);

expr_p manage_call_funcdef(SymbolTableEntry * funcdef, expr_p elist);

struct call_info * manage_callsufix_normcall(struct call_info * callinfo);

struct call_info * manage_callsufix_methodcall(struct call_info * methodcall);

expr_p manage_call_lvalue_callsuffix(expr_p lval, struct call_info * callsuffix);
struct call_info * manage_normcall(expr_p elist);
struct call_info * manage_methodcall(const char* id, expr_p elist);

expr_p manage_elist_expr(expr_p e, expr_p elists);
expr_p manage_elist_empty(void);
expr_p manage_exp(expr_p e);

expr_p manage_indexedelems_empty(void);

expr_p manage_indexedelem(expr_p e1, expr_p e2);

expr_p manage_object_elem(expr_p e);

expr_p manage_elem_elist(expr_p elist);
expr_p manage_elem_indexed(expr_p indexed);

struct stmt_list_info* manage_block(struct stmt_list_info* stmt);

void manage_openblock(void);

struct stmt_list_info* manage_endblock(struct stmt_list_info* stmt);

SymbolTableEntry * manage_funcdef(SymbolTableEntry *funcprefix, unsigned int counter);
SymbolTableEntry * manage_funcprefix(const char * funcname);
void manage_funcargs(void);
int manage_funcbody(int blockCounter);


int manage_openfuncblock();
const char * manage_funcname_ID(const char * id);
const char * manage_funcname_empty();

expr_p manage_const_INTEGER(int value);
expr_p manage_const_REAL(double value);
expr_p manage_const_STRING(const char *value);
expr_p manage_const_NIL(void);
expr_p manage_const_TRUE(void);
expr_p manage_const_FALSE(void);

void manage_formalid(const char * id);

int manage_ifprefix(expr_p e);

struct stmt_list_info* manage_ifstmt(int if_incJump, struct stmt_list_info* stmt);
struct stmt_list_info* manage_ifelsestmt(int if_incJump, struct stmt_list_info* stmt1, int else_incJump, struct stmt_list_info* stmt2);

int manage_elseprefix(void);

void manage_loopstart(void);
void manage_loopend(void);

struct stmt_list_info * manage_loopstmt(struct stmt_list_info* stmt);

int manage_whilestart(void);
int manage_whilecond(expr_p e);
void manage_whilestmt(int whilestart_incJump, int whilecond_incJump, struct stmt_list_info* stmt);

int manage_forN(void);
int manage_forM(void);

struct for_info * manage_forprefix(int forM, expr_p e);
void manage_forstmt(struct for_info* forprefix, int forN1, int forN2, struct stmt_list_info* stmt, int forN3);

void manage_retval_expr(expr_p e);

void manage_retval_empty(void);

SymbolTableEntry * tmpname_builder(char * _tmpName, int cnt);
SymbolTableEntry * newFuncTemp();

char * newTempName();

SymbolTableEntry * newTemp();
void resetTempCnt(void);


#endif	/* MANAGE_H */
