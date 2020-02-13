#include <string.h>
#include <stdlib.h>
#include "manage.h"
#include "symbolTable.h"
#include "dataStructs.h"
#include "expr.h"
#include "quad.h"
#include "scopeSpaces.h"
#include <assert.h>

extern int line;
extern unsigned int currScope;
extern unsigned int functionLocalOffset;

SymbolTableEntry * lookUpResult;
int blockCnt = 0;
int funcNameTmpCnt = 0;
int varNameTmpCnt = 0;
int funcNameFlag = 0;
int hasDot = 0;
int isFunction = 0;

int loopcounter = 0;
int constCnt = 0;
int notEmited = 0;

stack_p offsetStack = NULL;

stack_p loopcntStack = NULL;

stack_p constCntStack = NULL;

struct for_info* create_for_info(void) {
    struct for_info * tmp = (struct for_info*) malloc(sizeof (struct for_info));
    return tmp;
}

struct stmt_list_info* create_stmt_list_info(void) {
    struct stmt_list_info* tmp = (struct stmt_list_info*) malloc(sizeof (struct stmt_list_info));
    tmp->breaklist = NULL;
    tmp->continuelist = NULL;
    return tmp;
}

int check_value_modification() {
    if (hasDot) {
        hasDot = 0;
    } else if (isFunction) {
        newError("function can't modify it's value", line, NULL);
        return 0;
    }
    return 1;
}

int valid_arithexpr(enum expr_t type) {
    switch (type) {
        case programfunc_e: return 0;
        case libraryfunc_e: return 0;
        case boolexpr_e: return 0;
        case newtable_e: return 0;
        case constbool_e: return 0;
        case conststring_e: return 0;
        case nil_e: return 0;
        default: return 1;
    }
}

int trueTest(expr_p e) {
    assert(e != NULL);
    switch (e->type) {
        case constnum_e: return e->numConst != 0;
        case programfunc_e: return 1;
        case libraryfunc_e: return 1;
        case newtable_e: return 1;
        case nil_e: return 0;
        case conststring_e: return (strcmp(e->strConst, "\"\"") != 0);
        default: return -1;
    }

}

int isUndefined(enum expr_t type) {
    if (type == var_e ||
            type == tableitem_e ||
            type == arithexpr_e ||
            type == boolexpr_e ||
            type == assignexpr_e)
        return 1;

    return 0;
}

struct stmt_list_info* manage_stmts(struct stmt_list_info* stmts, struct stmt_list_info* stmt) {
    assert(stmts != NULL);
    assert(stmt != NULL);

    if (notEmited == 1) {
        notEmited = 0;
    }
    //resetTempCnt();
    stmts->breaklist = merge_list(stmts->breaklist, stmt->breaklist);
    stmts->continuelist = merge_list(stmts->continuelist, stmt->continuelist);
    return stmts;
}

struct stmt_list_info * manage_stmt_expr(expr_p e) {
    if (e != NULL) {
        backpatch_expr(e);
    }

    return create_stmt_list_info();
}

struct stmt_list_info * manage_stmt_ifstmt(struct stmt_list_info * stmt) {
    return stmt;
}

struct stmt_list_info* manage_break(void) {
    struct stmt_list_info* tmp = create_stmt_list_info();
    if (loopcounter > 0) {
        tmp->breaklist = create_list_node(nextquad());
        emit(jump, NULL, NULL, NULL, 0, line);
    } else {
        comperror("break not in a loop", line);
    }

    return tmp;
}

struct stmt_list_info* manage_continue(void) {
    struct stmt_list_info* tmp = create_stmt_list_info();
    if (loopcounter > 0) {
        tmp->continuelist = create_list_node(nextquad());
        emit(jump, NULL, NULL, NULL, 0, line);
    } else {
        comperror("continue not in a loop", line);
    }

    return tmp;
}

struct stmt_list_info * manage_stmt_block(struct stmt_list_info* stmt) {
    return stmt;
}

expr_p manage_expr_assignexpr(expr_p e) {
    return e;
}

expr_p manage_exprPLUSexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '+' ", line);
        return NULL;
    }

    expr_p result;
    if (e1->type == constnum_e && e2->type == constnum_e) {
        result = newexpr(constnum_e);
        result->numConst = e1->numConst + e2->numConst;
    } else {
        result = newexpr(arithexpr_e);
        result->sym = newTemp();
        emit(add, e1, e2, result, 0, line);
    }
    return result;
}

expr_p manage_exprMINUSexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '-' ", line);
        return NULL;
    }

    expr_p result;
    if (e1->type == constnum_e && e2->type == constnum_e) {
        result = newexpr(constnum_e);
        result->numConst = e1->numConst - e2->numConst;
    } else {
        result = newexpr(arithexpr_e);
        result->sym = newTemp();
        emit(sub, e1, e2, result, 0, line);
    }
    return result;
}

expr_p manage_exprMULexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '*' ", line);
        return NULL;
    }

    expr_p result;
    if (e1->type == constnum_e && e2->type == constnum_e) {
        result = newexpr(constnum_e);
        result->numConst = e1->numConst * e2->numConst;
    } else {
        result = newexpr(arithexpr_e);
        result->sym = newTemp();
        emit(mul, e1, e2, result, 0, line);
    }
    return result;
}

expr_p manage_exprDIVexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '/' ", line);
        return NULL;
    }

    expr_p result;
    if (e1->type == constnum_e && e2->type == constnum_e) {
        result = newexpr(constnum_e);
        result->numConst = e1->numConst / e2->numConst;
    } else {
        result = newexpr(arithexpr_e);
        result->sym = newTemp();
        emit(_div, e1, e2, result, 0, line);
    }
    return result;
}

expr_p manage_exprMODexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '%' ", line);
        return NULL;
    }

    expr_p result;
    if (e1->type == constnum_e && e2->type == constnum_e) {
        result = newexpr(constnum_e);
        int num = (int) (e1->numConst / e2->numConst);
        result->numConst = e1->numConst - (double) (num) * e2->numConst;
    } else {
        result = newexpr(arithexpr_e);
        result->sym = newTemp();
        emit(mod, e1, e2, result, 0, line);
    }
    return result;
}

expr_p manage_exprGREATERexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }

    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '>' ", line);
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);
    if (isUndefined(e1->type) || isUndefined(e2->type)) {
        result->truelist = create_list_node(nextquad());
        result->falselist = create_list_node(nextquad() + 1);

        //result->sym = newTemp();

        emit(if_greater, e1, e2, NULL, 0, line);
        emit(jump, NULL, NULL, NULL, 0, line);
        return result;
    }
    if ((e2->type == constnum_e) && (e1->type == constnum_e)) {
        if (e1->numConst > e2->numConst) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
        emit(jump, NULL, NULL, NULL, 0, line);
        result->type = constbool_e;
        return result;
    }
    comperror("Illegal operand for the operator '>' ", line);
    return NULL;
}

expr_p manage_exprGREATER_EQUALexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }

    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '>=' ", line);
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);

    if (isUndefined(e1->type) || isUndefined(e2->type)) {
        result->truelist = create_list_node(nextquad());
        result->falselist = create_list_node(nextquad() + 1);

        //result->sym = newTemp();

        emit(if_greatereq, e1, e2, NULL, 0, line);
        emit(jump, NULL, NULL, NULL, 0, line);
        return result;
    }
    if ((e2->type == constnum_e) && (e1->type == constnum_e)) {
        if (e1->numConst >= e2->numConst) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
        emit(jump, NULL, NULL, NULL, 0, line);
        result->type = constbool_e;
        return result;
    }
    comperror("Illegal operand for the operator '>=' ", line);
    return NULL;
}

expr_p manage_exprLESSexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }

    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '<' ", line);
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);

    if (isUndefined(e1->type) || isUndefined(e2->type)) {
        result->truelist = create_list_node(nextquad());
        result->falselist = create_list_node(nextquad() + 1);

        //result->sym = newTemp();

        emit(if_less, e1, e2, NULL, 0, line);
        emit(jump, NULL, NULL, NULL, 0, line);
        return result;
    }
    if ((e2->type == constnum_e) && (e1->type == constnum_e)) {
        if (e1->numConst < e2->numConst) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
        emit(jump, NULL, NULL, NULL, 0, line);
        result->type = constbool_e;
        return result;
    }
    comperror("Illegal operand for the operator '<' ", line);
    return NULL;
}

expr_p manage_exprLESS_EQUALexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }

    if (!valid_arithexpr(e1->type) || !valid_arithexpr(e2->type)) {
        comperror("Illegal operand for the operator '<=' ", line);
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);

    if (isUndefined(e1->type) || isUndefined(e2->type)) {
        result->truelist = create_list_node(nextquad());
        result->falselist = create_list_node(nextquad() + 1);

        //result->sym = newTemp();

        emit(if_lesseq, e1, e2, NULL, 0, line);
        emit(jump, NULL, NULL, NULL, 0, line);
        return result;
    }
    if ((e2->type == constnum_e) && (e1->type == constnum_e)) {
        if (e1->numConst <= e2->numConst) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
        emit(jump, NULL, NULL, NULL, 0, line);
        result->type = constbool_e;
        return result;
    }
    comperror("Illegal operand for the operator '<=' ", line);
    return NULL;
}

expr_p manage_exprEQUALexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);
    if (isUndefined(e1->type) || isUndefined(e2->type)) {
        result->truelist = create_list_node(nextquad());
        result->falselist = create_list_node(nextquad() + 1);

        //result->sym = newTemp();

        emit(if_eq, e1, e2, NULL, 0, line);
        emit(jump, NULL, NULL, NULL, 0, line);
        if (notEmited == 1) {
            notEmited = 0;
        }
        return result;
    }

    if (e1->type == e2->type) {
        if (e1->type == constbool_e) {
            if (e1->boolConst == e2->boolConst) {
                result->boolConst = 'T';
                result->truelist = create_list_node(nextquad());
            } else {
                if (e1->boolConst == 'T') {
                    e1->falselist = e1->truelist;
                    e1->truelist = NULL;
                } else {
                    e2->falselist = e2->truelist;
                    e2->truelist = NULL;
                }
                result->boolConst = 'F';
                result->falselist = create_list_node(nextquad());
            }
        } else if (e1->type == constnum_e) {
            if (e1->numConst == e2->numConst) {
                result->boolConst = 'T';
                result->truelist = create_list_node(nextquad());
            } else {
                result->boolConst = 'F';
                result->falselist = create_list_node(nextquad());
            }
        } else if (e1->type == conststring_e) {
            if (strcmp(e1->strConst, e2->strConst) == 0) {
                result->boolConst = 'T';
                result->truelist = create_list_node(nextquad());
            } else {
                result->boolConst = 'F';
                result->falselist = create_list_node(nextquad());
            }
        } else {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        }
    } else if (e1->type == constbool_e) {
        int x = trueTest(e2);
        assert(x != -1);
        if ((x == 1 && e1->boolConst == 'T') || (x == 0 && e1->boolConst == 'F')) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
    } else if (e2->type == constbool_e) {
        int x = trueTest(e1);
        assert(x != -1);
        if ((x == 1 && e2->boolConst == 'T') || (x == 0 && e2->boolConst == 'F')) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
    } else if (e1->type == newtable_e && e2->type == nil_e) {
        result->boolConst = 'F';
        result->falselist = create_list_node(nextquad());
    } else if (e2->type == newtable_e && e1->type == nil_e) {
        result->boolConst = 'F';
        result->falselist = create_list_node(nextquad());
    } else {
        comperror("Illegal operand for the operator '==', operands with different type", line);
    }
    result->truelist = merge_list(result->truelist, merge_list(e1->truelist, e2->truelist));
    result->falselist = merge_list(result->falselist, merge_list(e1->falselist, e2->falselist));
    emit(jump, NULL, NULL, NULL, 0, line);
    result->type = constbool_e;
    return result;
}

expr_p manage_exprNOT_EQUALexpr(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);
    if (isUndefined(e1->type) || isUndefined(e2->type)) {
        result->truelist = create_list_node(nextquad());
        result->falselist = create_list_node(nextquad() + 1);

        //result->sym = newTemp();

        emit(if_noteq, e1, e2, NULL, 0, line);
        emit(jump, NULL, NULL, NULL, 0, line);
        if (notEmited == 1) {
            notEmited = 0;
        }
        return result;
    }

    if (e1->type == e2->type) {
        if (e1->type == constbool_e) {
            if (e1->boolConst != e2->boolConst) {
                result->boolConst = 'T';
                result->truelist = create_list_node(nextquad());
            } else {
                result->boolConst = 'F';
                result->falselist = create_list_node(nextquad());
            }
        } else if (e1->type == constnum_e) {
            if (e1->numConst != e2->numConst) {
                result->boolConst = 'T';
                result->truelist = create_list_node(nextquad());
            } else {
                result->boolConst = 'F';
                result->falselist = create_list_node(nextquad());
            }
        } else if (e1->type == conststring_e) {
            if (strcmp(e1->strConst, e2->strConst) != 0) {
                result->boolConst = 'T';
                result->truelist = create_list_node(nextquad());
            } else {
                result->boolConst = 'F';
                result->falselist = create_list_node(nextquad());
            }
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
    } else if (e1->type == constbool_e) {
        int x = trueTest(e2);
        assert(x != -1);
        if ((x == 1 && e1->boolConst != 'T') || (x == 0 && e1->boolConst != 'F')) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
    } else if (e2->type == constbool_e) {
        int x = trueTest(e1);
        assert(x != -1);
        if ((x == 1 && e2->boolConst != 'T') || (x == 0 && e2->boolConst != 'F')) {
            result->boolConst = 'T';
            result->truelist = create_list_node(nextquad());
        } else {
            result->boolConst = 'F';
            result->falselist = create_list_node(nextquad());
        }
    } else if (e1->type == newtable_e && e2->type == nil_e) {
        result->boolConst = 'T';
        result->truelist = create_list_node(nextquad());
    } else if (e2->type == newtable_e && e1->type == nil_e) {
        result->boolConst = 'T';
        result->truelist = create_list_node(nextquad());
    } else {
        comperror("Illegal operand for the operator '!=', operands with different type", line);
        return NULL;
    }
    result->truelist = merge_list(merge_list(e1->truelist, e2->truelist), result->truelist);
    result->falselist = merge_list(merge_list(e1->falselist, e2->falselist), result->falselist);
    emit(jump, NULL, NULL, NULL, 0, line);
    result->type = constbool_e;
    return result;
}

expr_p manage_andprefix(expr_p e1) {
    if (e1 == NULL) {
        return NULL;
    }

    push(&constCntStack, constCnt);
    constCnt = 0;

    if (e1->type == boolexpr_e && notEmited == 0) {
        return e1;
    }
    expr_p r1 = manage_exprEQUALexpr(e1, newexpr_constbool(1));
    assert(r1 != NULL);

    if (isUndefined(e1->type)) {
        e1->truelist = merge_list(e1->truelist, r1->truelist);
        e1->falselist = merge_list(e1->falselist, r1->falselist);
    } else {
        assert(r1->type == constbool_e);

        e1->boolConst = r1->boolConst;

        e1->truelist = r1->truelist;
        e1->falselist = r1->falselist;
        constCnt++;
    }

    notEmited = 0;

    return e1;
}

expr_p manage_exprANDexpr(expr_p e1, int M, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);

    if (e2->type != boolexpr_e || notEmited == 1) {
        expr_p r2 = manage_exprEQUALexpr(e2, newexpr_constbool(1));
        assert(r2 != NULL);

        if (isUndefined(e2->type)) {
            e2->truelist = merge_list(e2->truelist, r2->truelist);
            e2->falselist = merge_list(e2->falselist, r2->falselist);
        } else {
            assert(r2->type == constbool_e);

            e2->boolConst = r2->boolConst;

            e2->truelist = r2->truelist;
            e2->falselist = r2->falselist;
            constCnt++;
        }
    }

    if (constCnt == 2) {
        result->type = constbool_e;

        // - - - - - Debug Session - - - - - - - - -
        if (e1->type != constbool_e && isUndefined(e1->type)) {
            printQuads();
        }
        assert(e1->type == constbool_e || !isUndefined(e1->type));
        assert(e1->boolConst == 'T' || e1->boolConst == 'F');
        assert(e2->type == constbool_e || !isUndefined(e2->type));
        assert(e2->boolConst == 'T' || e2->boolConst == 'F');
        // - - - - - - - END - - - - - - - - - - - -

        if (e1->boolConst == 'F' || e2->boolConst == 'F') {
            result->boolConst = 'F';
        } else {
            result->boolConst = 'T';
        }
    }
    constCnt = pop(&constCntStack);
    notEmited = 0;

    backpatch(e1->truelist, M);
    result->truelist = e2->truelist;
    result->falselist = merge_list(e1->falselist, e2->falselist);

    return result;
}

expr_p manage_orprefix(expr_p e1) {
    if (e1 == NULL) {
        return NULL;
    }

    push(&constCntStack, constCnt);
    constCnt = 0;

    if (e1->type == boolexpr_e && notEmited == 0) {
        return e1;
    }
    expr_p r1 = manage_exprEQUALexpr(e1, newexpr_constbool(1));
    assert(r1 != NULL);

    if (isUndefined(e1->type)) {
        e1->truelist = merge_list(e1->truelist, r1->truelist);
        e1->falselist = merge_list(e1->falselist, r1->falselist);
    } else {
        assert(r1->type == constbool_e);

        e1->boolConst = r1->boolConst;

        e1->truelist = r1->truelist;
        e1->falselist = r1->falselist;
        constCnt++;
    }

    notEmited = 0;

    return e1;
}

expr_p manage_exprORexpr(expr_p e1, int M, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }
    expr_p result = newexpr(boolexpr_e);

    if (e2->type != boolexpr_e || notEmited == 1) {
        expr_p r2 = manage_exprEQUALexpr(e2, newexpr_constbool(1));
        assert(r2 != NULL);

        if (isUndefined(e2->type)) {
            e2->truelist = merge_list(e2->truelist, r2->truelist);
            e2->falselist = merge_list(e2->falselist, r2->falselist);
        } else {
            assert(r2->type == constbool_e);

            e2->boolConst = r2->boolConst;

            e2->truelist = r2->truelist;
            e2->falselist = r2->falselist;
            constCnt++;
        }
    }

    if (constCnt == 2) {
        result->type = constbool_e;

        // - - - - - Debug Session - - - - - - - - -
        assert(e1->type == constbool_e || !isUndefined(e1->type));
        assert(e1->boolConst == 'T' || e1->boolConst == 'F');
        assert(e2->type == constbool_e || !isUndefined(e2->type));
        assert(e2->boolConst == 'T' || e2->boolConst == 'F');
        // - - - - - - - END - - - - - - - - - - - -

        if (e1->boolConst == 'T' || e2->boolConst == 'T') {
            result->boolConst = 'T';
        } else {
            result->boolConst = 'F';
        }
    }
    constCnt = pop(&constCntStack);

    notEmited = 0;

    backpatch(e1->falselist, M);
    result->truelist = merge_list(e1->truelist, e2->truelist);
    result->falselist = e2->falselist;

    return result;
}

expr_p manage_expr_term(expr_p e) {
    return e;
}

int manage_M(void) {
    return nextquad();
}

expr_p manage_term_expr(expr_p e) {
    return e;
}

expr_p manage_term_uminus(expr_p e) {
    if (e == NULL) {
        return NULL;
    }

    if (checkuminus(e) == 0) {
        return NULL;
    }
    expr_p term = newexpr(arithexpr_e);
    term->sym = newTemp();
    emit(mul, e, newexpr_constnum(-1), term, 0, line);
    return term;
}

expr_p manage_term_NOT(expr_p e) {
    if (e == NULL) {
        return NULL;
    }

    expr_p term = newexpr(boolexpr_e);

    if (e->type == constbool_e) {
        if (e->boolConst == 'T') {
            e->truelist = merge_list(e->truelist, create_list_node(nextquad()));
            emit(jump, NULL, NULL, NULL, 0, line);
        } else {
            e->falselist = merge_list(e->falselist, create_list_node(nextquad()));
            emit(jump, NULL, NULL, NULL, 0, line);
        }
        term->type = constbool_e;
        if (e->boolConst == 'T') {
            term->boolConst = 'F';
        } else {
            term->boolConst = 'T';
        }
    }

    term->truelist = e->falselist;
    term->falselist = e->truelist;
    if (term->truelist == NULL && term->falselist == NULL) {
        int test = trueTest(e);
        if (test == -1) {
            term->sym = newTemp();
            emit(not, e, NULL, term, 0, line);
            notEmited = 1;
        } else {
            term = manage_term_NOT(newexpr_constbool(test));
        }
    }
    return term;
}

expr_p manage_term_lvalue_PP(expr_p lval) {
    if (lval == NULL) {
        return NULL;
    }

    if (check_value_modification() == 0) {
        return NULL;
    }
    expr_p term = newexpr(var_e);
    term->sym = newTemp();

    if (lval->type == tableitem_e) {
        expr_p value = emit_iftableitem(lval);
        emit(assign, value, NULL, term, 0, line);
        emit(add, value, newexpr_constnum(1), value, 0, line);
        emit(tablesetelem, lval, lval->index, value, 0, line);
    } else {
        emit(assign, lval, NULL, term, 0, line);
        emit(add, lval, newexpr_constnum(1), lval, 0, line);
    }
    return term;
}

expr_p manage_term_lvalue_MM(expr_p lval) {
    if (lval == NULL) {
        return NULL;
    }

    if (check_value_modification() == 0) {
        return NULL;
    }

    expr_p term = newexpr(var_e);
    term->sym = newTemp();

    if (lval->type == tableitem_e) {
        expr_p value = emit_iftableitem(lval);
        emit(assign, value, NULL, term, 0, line);
        emit(sub, value, newexpr_constnum(1), value, 0, line);
        emit(tablesetelem, lval, lval->index, value, 0, line);
    } else {
        emit(assign, lval, NULL, term, 0, line);
        emit(sub, lval, newexpr_constnum(1), lval, 0, line);
    }
    return term;
}

expr_p manage_term_PP_lvalue(expr_p lval) {
    if (lval == NULL) {
        return NULL;
    }

    if (check_value_modification() == 0) {
        return NULL;
    }

    expr_p term;
    if (lval->type == tableitem_e) {
        term = emit_iftableitem(lval);
        emit(add, term, newexpr_constnum(1), term, 0, line);
        emit(tablesetelem, lval, lval->index, term, 0, line);
    } else {
        emit(add, lval, newexpr_constnum(1), lval, 0, line);
        term = newexpr(arithexpr_e);
        term->sym = newTemp();
        emit(assign, lval, NULL, term, 0, line);
    }
    return term;
}

expr_p manage_term_MM_lvalue(expr_p lval) {
    if (lval == NULL) {
        return NULL;
    }

    if (check_value_modification() == 0) {
        return NULL;
    }

    expr_p term;
    if (lval->type == tableitem_e) {
        term = emit_iftableitem(lval);
        emit(sub, term, newexpr_constnum(1), term, 0, line);
        emit(tablesetelem, lval, lval->index, term, 0, line);
    } else {
        emit(sub, lval, newexpr_constnum(1), lval, 0, line);
        term = newexpr(arithexpr_e);
        term->sym = newTemp();
        emit(assign, lval, NULL, term, 0, line);
    }
    return term;
}

expr_p manage_term_primary(expr_p e) {
    return e;
}

expr_p manage_assignexpr(expr_p lval, expr_p e) {
    if (lval == NULL || e == NULL) {
        return NULL;
    }
    expr_p assignexpr;
    backpatch_expr(e);

    if (lval->type == tableitem_e) {
        emit(tablesetelem, lval, lval->index, e, 0, line);
        assignexpr = emit_iftableitem(lval);
        assignexpr->type = assignexpr_e;
    } else {
        emit(assign, e, NULL, lval, 0, line);
        assignexpr = newexpr(assignexpr_e);
        assignexpr->sym = newTemp();
        emit(assign, lval, NULL, assignexpr, 0, line);
    }
    return assignexpr;
}

expr_p manage_assignlvalue(expr_p lval) {
    if (check_value_modification() == 0) {
        return NULL;
    }
    return lval;
}

expr_p manage_primary_call(expr_p e) {
    return e;
}

expr_p manage_primary_object(expr_p e) {
    return e;
}

expr_p manage_primary_funcdef(SymbolTableEntry * funcdef) {
    expr_p primary = newexpr(programfunc_e);
    primary->sym = funcdef;
    return primary;
}

expr_p manage_primary_const(expr_p e) {
    return e;
}

expr_p manage_lvalue_ID(const char * id) {
    hasDot = 0;
    isFunction = 0;
    lookUpResult = symbolTable_lookup(id);
    if (lookUpResult == NULL) {// den to brike
        // bazoume ena new var
        if (currScope == 0) {
            lookUpResult = symbolTable_insert(create_newSymbol(_GLOBAL, id, currScope, line, currscopespace(), currscopeoffset()));
        } else {
            lookUpResult = symbolTable_insert(create_newSymbol(_LOCAL, id, currScope, line, currscopespace(), currscopeoffset()));
        }
    } else {// to brike
        //elegxoume an borei na to kanei access
        if (lookUpResult->value.varVal->scope != 0) {// oxi global
            if (lookUpResult->value.varVal->scope != currScope) {//diaforetiko scope
                if (lookUpResult->type != _USERFUNC) {// oxi function
                    if (currScope - lookUpResult->value.varVal->scope > blockCnt) {// exei anamesa funcdef
                        newError("can't be accessed from this scope", line, lookUpResult->value.varVal->name);
                        return NULL;
                    }
                }
            }
        }
        if (lookUpResult->type == _LIBFUNC || lookUpResult->type == _USERFUNC) {
            isFunction = 1;
        }
    }

    return lvalue_expr(lookUpResult);
}

expr_p manage_lvalue_LOCAL_ID(const char * id) {
    hasDot = 0;
    isFunction = 0;
    lookUpResult = scopeTable_lookup(id, currScope);
    if (lookUpResult == NULL) {
        if (!libraryFunction_collision(id)) {//den einai lib func
            if (currScope == 0) {
                lookUpResult = symbolTable_insert(create_newSymbol(_GLOBAL, id, currScope, line, currscopespace(), currscopeoffset()));
            } else {
                lookUpResult = symbolTable_insert(create_newSymbol(_LOCAL, id, currScope, line, currscopespace(), currscopeoffset()));
            }
        } else {
            newError("can't have the same name with library function", line, id);
            return NULL;
        }
    } else {
        if (lookUpResult->type == _LIBFUNC || lookUpResult->type == _USERFUNC) {
            isFunction = 1;
        }
    }
    return lvalue_expr(lookUpResult);
}

expr_p manage_lvalue_CCOLON_ID(const char * id) {
    isFunction = 0;
    lookUpResult = scopeTable_lookup(id, 0);
    if (lookUpResult == NULL) {
        newError("isn't defined on global scope", line, id);
        return NULL;
    } else {
        if (lookUpResult->type == _LIBFUNC || lookUpResult->type == _USERFUNC) {
            isFunction = 1;
        }
    }
    return lvalue_expr(lookUpResult);
}

expr_p manage_lvalue_member(expr_p e) {
    return e;
}

expr_p manage_member_tableitem(expr_p e) {
    return e;
}

expr_p manage_tableitem_lvalue_dot_id(expr_p lval, const char * id) {
    if (lval == NULL) {
        return NULL;
    }
    hasDot = 1;

    return member_item(lval, id);
}

expr_p manage_tableitem_lvalue_expr(expr_p lval, expr_p e) {
    if (lval == NULL) {
        return NULL;
    }
    lval = emit_iftableitem(lval);
    expr_p tableitem = newexpr(tableitem_e);
    tableitem->sym = lval->sym;
    tableitem->index = e;
    return tableitem;
}

expr_p manage_call_funcdef(SymbolTableEntry * funcdef, expr_p elist) {
    expr_p func = newexpr(programfunc_e);
    func->sym = funcdef;
    return make_call(func, elist);
}

struct call_info * manage_callsufix_normcall(struct call_info * callinfo) {
    return callinfo;
}

struct call_info * manage_callsufix_methodcall(struct call_info * methodcall) {
    return methodcall;
}

expr_p manage_call_lvalue_callsuffix(expr_p lval, struct call_info * callsuffix) {
    if (lval == NULL) {
        return NULL;
    }

    assert(callsuffix != NULL);
    if (callsuffix->isMethod == 'T') {
        expr_p self = member_item(lval, callsuffix->name);
        lval = emit_iftableitem(self);
        self->next = callsuffix->elist;
        callsuffix->elist = self;
    }

    return make_call(lval, callsuffix->elist);
}

struct call_info * manage_normcall(expr_p elist) {
    struct call_info * normcall = create_call_info();

    normcall->elist = elist;
    normcall->isMethod = 'F';
    normcall->name = NULL;

    return normcall;
}

struct call_info * manage_methodcall(const char* id, expr_p elist) {
    struct call_info * methodcall = create_call_info();

    methodcall->elist = elist;
    methodcall->isMethod = 'T';
    methodcall->name = id;

    return methodcall;
}

expr_p manage_elist_expr(expr_p e, expr_p elists) {
    if (e != NULL) e->next = elists;
    return e;
}

expr_p manage_elist_empty(void) {
    return NULL;
}

expr_p manage_exp(expr_p e) {
    if (e != NULL) {
        backpatch_expr(e);
    }
    return e;
}

expr_p manage_indexed(expr_p indexed1, expr_p indexed2) {
    if (indexed1 == NULL) {
        return NULL;
    }
    (indexed1->next)->next = indexed2;
    return indexed1;
}

expr_p manage_indexedelems_empty(void) {
    return NULL;
}

expr_p manage_indexedelem(expr_p e1, expr_p e2) {
    if (e1 == NULL || e2 == NULL) {
        return NULL;
    }

    e1->next = e2;
    return e1;
}

expr_p manage_object_elem(expr_p e) {
    return e;
}

expr_p manage_elem_elist(expr_p elist) {
    expr_p table = newexpr(newtable_e);
    table->sym = newTemp();
    emit(tablecreate, table, NULL, NULL, 0, line);
    expr_p curr = elist;
    int i = 0;
    while (curr != NULL) {
        if (curr->type == boolexpr_e && curr->sym == NULL) {
            curr->sym = newTemp();
        }

        emit(tablesetelem, table, newexpr_constnum(i++), curr, 0, line);
        curr = curr->next;
    }
    return table;
}

expr_p manage_elem_indexed(expr_p indexed) {
    if (indexed == NULL) {
        return NULL;
    }

    expr_p table = newexpr(newtable_e);
    table->sym = newTemp();
    emit(tablecreate, table, NULL, NULL, 0, line);
    expr_p x = indexed;
    expr_p y = indexed->next;
    while (x != NULL) {
        assert(y != NULL);
        if (x->type == boolexpr_e && x->sym == NULL) {
            x->sym = newTemp();
        }
        if (y->type == boolexpr_e && y->sym == NULL) {
            y->sym = newTemp();
        }

        emit(tablesetelem, table, x, y, 0, line);

        x = y->next;
        if (x == NULL) break;
        y = x->next;
    }
    return table;
}

struct stmt_list_info* manage_block(struct stmt_list_info* stmt) {
    return stmt;
}

void manage_openblock(void) {
    currScope++;
    blockCnt++;
}

struct stmt_list_info* manage_endblock(struct stmt_list_info* stmt) {
    scopeTable_hide(currScope--);
    if (blockCnt != 0) {
        blockCnt--;
    }

    return stmt;
}

SymbolTableEntry * manage_funcdef(SymbolTableEntry *funcprefix, unsigned int counter) {
    blockCnt = counter;
    if (funcprefix != NULL) {
        funcprefix->value.funcVal->totallocals = functionLocalOffset;
        functionLocalOffset = pop(&offsetStack);
        emit(funcend, NULL, NULL, lvalue_expr(funcprefix), 0, line);
    }
    return funcprefix;
}

SymbolTableEntry * manage_funcprefix(const char * funcname) {
    currScope++;
    if (funcname != NULL) {
        funcNameFlag = 1;
        lookUpResult = symbolTable_lookup(funcname);
        assert(lookUpResult != NULL);
        lookUpResult->value.funcVal->iaddress = nextquad();
        emit(funcstart, NULL, NULL, lvalue_expr(lookUpResult), 0, line);
    } else {
        funcNameFlag = 0;
        lookUpResult = NULL;
    }
    push(&offsetStack, functionLocalOffset); //kratame to palio function local se stoiva
    enterscopespace();
    resetcurrscopeoffset(); /*kanoume reset to offset gia formal argument*/

    return lookUpResult;
}

void manage_funcargs(void) {
    enterscopespace();
    resetcurrscopeoffset(); /*reset sta function local*/
}

int manage_funcbody(int blockCounter) {
    exitscopespace();

    loopcounter = pop(&loopcntStack);

    return blockCounter;
}

int manage_openfuncblock() {
    funcNameFlag = 0;
    int tmp = blockCnt;
    blockCnt = 0;

    push(&loopcntStack, loopcounter);
    loopcounter = 0;
    return tmp;
}

const char * manage_funcname_ID(const char * id) {
    isFunction = 0;
    lookUpResult = scopeTable_lookup(id, currScope);
    if (lookUpResult == NULL) {
        symbolTable_insert(create_newSymbol(_USERFUNC, id, currScope, line, 0, 0));
        return id;
    }

    if (libraryFunction_collision(id)) {
        newError("can't have the same name with library function", line, id);
    } else if (lookUpResult->type == _USERFUNC) {
        newError("is already defined as function", line, id);
    } else {
        newError("is already defined as variable", line, id);
    }
    return NULL;
}

const char * manage_funcname_empty() {
    lookUpResult = newFuncTemp();
    return lookUpResult->value.funcVal->name;
}

expr_p manage_const_INTEGER(int value) {
    expr_p e = newexpr(constnum_e);
    e->numConst = value;
    return e;
}

expr_p manage_const_REAL(double value) {
    expr_p e = newexpr(constnum_e);
    e->numConst = value;
    return e;
}

expr_p manage_const_STRING(const char *value) {
    expr_p e = newexpr(conststring_e);
    e->strConst = strdup(value);
    return e;
}

expr_p manage_const_NIL(void) {
    expr_p e = newexpr(nil_e);
    return e;
}

expr_p manage_const_TRUE(void) {
    expr_p e = newexpr(constbool_e);

    e->boolConst = 'T';
    //e->truelist = create_list_node(nextquad());
    //emit(jump,NULL,NULL,NULL,0,line);
    return e;
}

expr_p manage_const_FALSE(void) {
    expr_p e = newexpr(constbool_e);

    e->boolConst = 'F';
    //e->falselist = create_list_node(nextquad());
    //emit(jump,NULL,NULL,NULL,0,line);
    return e;
}

void manage_formalid(const char * id) {
    if (!libraryFunction_collision(id)) {
        SymbolTableEntry * tmpResult = scopeTable_lookup(id, currScope);
        if (tmpResult != NULL) {
            newError("more than one arguments with the same name", line, NULL);
        } else {
            SymbolTableEntry * newSymbol = create_newSymbol(_FORMAL, id, currScope, line, currscopespace(), currscopeoffset());
            symbolTable_insert(newSymbol);
            if (funcNameFlag == 1) {// function in symTable
                add_funcArgs(lookUpResult->value.funcVal, create_funcArgs(newSymbol->value.varVal));
            }
        }
    } else {
        newError("formal argument can't shadow library function", line, NULL);
    }
}

int manage_ifprefix(expr_p e) {
    if (e == NULL) {
        return -1;
    }
    backpatch_expr(e);

    emit(if_eq, e, newexpr_constbool(1), NULL, nextquad() + 2, line);
    int ifprefix = nextquad();
    emit(jump, NULL, NULL, NULL, 0, line);
    return ifprefix;
}

struct stmt_list_info* manage_ifstmt(int if_incJump, struct stmt_list_info* stmt) {
    if (if_incJump == -1) {
        return stmt;
    }

    patchlabel(if_incJump, nextquad());
    return stmt;
}

struct stmt_list_info* manage_ifelsestmt(int if_incJump, struct stmt_list_info* stmt1, int else_incJump, struct stmt_list_info* stmt2) {
    if (if_incJump == -1) {
        return create_stmt_list_info();
    }
    assert(stmt1 != NULL);
    assert(stmt2 != NULL);

    struct stmt_list_info* tmp = create_stmt_list_info();
    tmp->breaklist = merge_list(stmt1->breaklist, stmt2->breaklist);
    tmp->continuelist = merge_list(stmt1->continuelist, stmt2->continuelist);

    patchlabel(if_incJump, else_incJump + 1);
    patchlabel(else_incJump, nextquad());

    return tmp;
}

int manage_elseprefix(void) {
    int elseprefix = nextquad();
    emit(jump, NULL, NULL, NULL, 0, line);
    return elseprefix;
}

void manage_loopstart(void) {
    ++loopcounter;
}

void manage_loopend(void) {
    --loopcounter;
}

struct stmt_list_info * manage_loopstmt(struct stmt_list_info* stmt) {
    return stmt;
}

int manage_whilestart(void) {
    return nextquad();
}

int manage_whilecond(expr_p e) {
    if (e == NULL) {
        return -1;
    }

    backpatch_expr(e);

    emit(if_eq, e, newexpr_constbool(1), NULL, nextquad() + 2, line);
    int whilecond = nextquad();
    emit(jump, NULL, NULL, NULL, 0, line);
    return whilecond;
}

void manage_whilestmt(int whilestart_incJump, int whilecond_incJump, struct stmt_list_info* stmt) {
    if (whilecond_incJump == -1) {
        return;
    }
    assert(stmt != NULL);

    emit(jump, NULL, NULL, NULL, whilestart_incJump, line);
    patchlabel(whilecond_incJump, nextquad());

    backpatch(stmt->breaklist, nextquad());
    backpatch(stmt->continuelist, whilestart_incJump);

}

int manage_forN(void) {
    int N = nextquad();
    emit(jump, NULL, NULL, NULL, 0, line);
    return N;
}

int manage_forM(void) {
    return nextquad();
}

struct for_info * manage_forprefix(int forM, expr_p e) {
    if (e == NULL) {
        return NULL;
    }

    backpatch_expr(e);

    struct for_info * tmp = create_for_info();
    tmp->test = forM;
    tmp->enter = nextquad();
    emit(if_eq, e, newexpr_constbool(1), NULL, 0, line);
    return tmp;
}

void manage_forstmt(struct for_info* forprefix, int forN1, int forN2, struct stmt_list_info* stmt, int forN3) {
    if (forprefix == NULL) {
        return;
    }

    assert(stmt != NULL);

    patchlabel(forprefix->enter, forN2 + 1); //true jump
    patchlabel(forN1, nextquad()); //false jump
    patchlabel(forN2, forprefix->test); //loop jump
    patchlabel(forN3, forN1 + 1); //closure jump

    backpatch(stmt->breaklist, nextquad()); //false jump
    backpatch(stmt->continuelist, forN1 + 1); //closure jump
}

void manage_retval_expr(expr_p e) {
    if (e == NULL) {
        return;
    }

    if (currScope <= blockCnt) {
        comperror("return not in a function", line);
    } else {
        backpatch_expr(e);

        emit(ret, e, NULL, NULL, 0, line);
    }
}

void manage_retval_empty(void) {
    if (currScope <= blockCnt) {
        comperror("return not in a function", line);
    } else {
        emit(ret, NULL, NULL, NULL, 0, line);
    }
}

SymbolTableEntry * newFuncTemp() {
    char _tmpNum[100];
    char * _tmpName = (char *) malloc(100);

    _tmpName[0] = '_';
    _tmpName[1] = 'f';
    _tmpName[2] = 'u';
    _tmpName[3] = 'n';
    _tmpName[4] = 'c';
    _tmpName[5] = '\0';

    sprintf(_tmpNum, "%d", funcNameTmpCnt);
    funcNameTmpCnt++;
    strcat(_tmpName, _tmpNum);
    symbolTable_insert(create_newSymbol(_USERFUNC, _tmpName, currScope, line, 0, 0));
    lookUpResult = symbolTable_lookup(_tmpName);

    free(_tmpName);
    return lookUpResult;
}

char * newTempName() {
    char _tmpNum[100];
    char * _tmpName = (char *) malloc(100);

    _tmpName[0] = '_';
    _tmpName[1] = 't';
    _tmpName[2] = 'm';
    _tmpName[3] = 'p';
    _tmpName[4] = '\0';

    sprintf(_tmpNum, "%d", varNameTmpCnt);
    varNameTmpCnt++;
    strcat(_tmpName, _tmpNum);

    return _tmpName;
}

SymbolTableEntry * newTemp() {
    char * name = newTempName();

    lookUpResult = scopeTable_lookup(name, currScope);
    if (lookUpResult == NULL) {
        return symbolTable_insert(create_newSymbol(_TEMPVAR, name, currScope, line, currscopespace(), currscopeoffset()));
    }

    return lookUpResult;
}

void resetTempCnt(void) {
    varNameTmpCnt = 0;
}
