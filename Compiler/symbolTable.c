#include "symbolTable.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

unsigned int currScope;

SymbolTableEntry** scopeTable;
SymbolTableEntry** symbolTable;

struct errorList * head_errorList;

orderedList_p orderedList_head = NULL, orderedList_tail = NULL;

static unsigned int symbolTable_hashFunc(const char *name) {
    assert(name != NULL);
    size_t ui;
    unsigned int index = 0U;
    for (ui = 0U; name[ui] != '\0'; ui++) {
        index = index * HASH_MULTIPLIER + name[ui];
    }
    return index % SYMBOL_BUCKETS;
}

void symbolTable_init(void) {
    symbolTable = malloc(SYMBOL_BUCKETS * sizeof (struct SymbolTableEntry*));
    int i;
    for (i = 0; i < SYMBOL_BUCKETS; i++) {
        symbolTable[i] = NULL;
    }
}

void scopeTable_init(void) {
    scopeTable = malloc(SCOPE_BUCKETS * sizeof (struct SymbolTableEntry*));
    int i;
    for (i = 0; i < SCOPE_BUCKETS; i++) {
        scopeTable[i] = NULL;
    }
}

SymbolTableEntry * symbolTable_insert(SymbolTableEntry *newSymbol) {
    unsigned int index;
    const char * symbolName = newSymbol ->value.varVal -> name;
    index = symbolTable_hashFunc(symbolName);
    if (newSymbol->type != _LIBFUNC && newSymbol->type != _USERFUNC) {
        inccurrscopeoffset();
    }
    if (symbolTable[index] == NULL) {
        symbolTable[index] = newSymbol;
        scopeTable_insert(newSymbol);
        orderedList_insert(newSymbol);
        return newSymbol;
    }
    SymbolTableEntry * head = symbolTable[index];
    while (head->next != NULL) {
        head = head->next;
    }
    assert(head != NULL);
    head->next = newSymbol;
    scopeTable_insert(newSymbol);
    orderedList_insert(newSymbol);
    return newSymbol;
}

SymbolTableEntry * symbolTable_lookup(const char *name) {
    int i;
    SymbolTableEntry *isFound;
    for (i = currScope; i >= 0; i--) {
        isFound = scopeTable_lookup(name, i);
        if (isFound != NULL) {
            return isFound;
        }
    }
    assert(isFound == NULL);
    return NULL;

}

void symbolTable_addLibFunc(void) {
    int i;
    for (i = 0; i < 12; i++) {
        symbolTable_insert(create_newSymbol(_LIBFUNC, get_libraryFunction(i), 0, 0, 0, 0));
    }
}

void scopeTable_insert(SymbolTableEntry *newSymbol) {
    unsigned int newSymbolScope = newSymbol->value.varVal->scope;
    SymbolTableEntry * head = scopeTable[newSymbolScope];
    if (head == NULL) {
        scopeTable[newSymbolScope] = newSymbol;
        return;
    }
    while (head->nextScope != NULL) {
        head = head ->nextScope;
    }
    assert(head != NULL);
    head->nextScope = newSymbol;
    // insert sth list
    return;
}

SymbolTableEntry * scopeTable_lookup(const char* name, unsigned int scope) {
    SymbolTableEntry * head = scopeTable[scope];
    while (head != NULL) {
        if (strcmp(head->value.varVal->name, name) == 0 && (head->isActive == 1)) {
            return head;
        }
        head = head->nextScope;
    }
    return NULL;
}

void scopeTable_hide(unsigned int scope) {
    SymbolTableEntry *head = scopeTable[scope];
    while (head != NULL) {
        head->isActive = 0;
        head = head->nextScope;
    }
}

SymbolTableEntry * create_newSymbol(enum SymbolTableType type, const char *name, unsigned int scope, unsigned int line, enum scopespace_t space, unsigned int offset) {
    SymbolTableEntry * tmp2 = malloc(sizeof (struct SymbolTableEntry));
    if (type == _LIBFUNC || type == _USERFUNC) {
        tmp2->value.funcVal = create_function(name, scope, line);
    } else {
        tmp2->value.varVal = create_variable(name, scope, line, space, offset);
    }
    tmp2->type = type;
    tmp2->isActive = 1;
    tmp2->next = NULL;
    tmp2->nextScope = NULL;
    return tmp2;
}

Variable * create_variable(const char* name, unsigned int scope, unsigned int line, enum scopespace_t space, unsigned int offset) {
    Variable * tmp = malloc(sizeof (struct Variable));

    tmp->name = strdup(name);
    tmp->scope = scope;
    tmp->line = line;
    tmp->space = space;
    tmp->offset = offset;

    return tmp;
}

Function * create_function(const char* name, unsigned int scope, unsigned int line) {
    Function * tmp = malloc(sizeof (struct Function));

    tmp->name = strdup(name);
    tmp->scope = scope;
    tmp->line = line;
    tmp->iaddress = 0U;
    tmp->taddress = 0U;
    tmp->returnList = NULL;
    tmp->totallocals = 0U;
    tmp->argsList = NULL;

    return tmp;
}

FuncArgs * create_funcArgs(Variable * varValue) {
    FuncArgs * tmp = malloc(sizeof (struct FuncArgs));
    tmp ->varValue = varValue;
    tmp->next = NULL;
    return tmp;
}

Function * add_funcArgs(Function * func, FuncArgs * newArg) {
    assert(func != NULL);
    assert(newArg != NULL);
    FuncArgs * tmp = func->argsList;
    if (tmp == NULL) {
        func->argsList = newArg;
        return func;
    }
    while (tmp != NULL) {
        if (tmp->next == NULL) {
            tmp->next = newArg;
            return func;
        }
        tmp = tmp->next;
    }
    return func;
}

char *get_VariableScopeSpace(int value) {
    switch (value) {
        case 0: return "Program Variable";
        case 1: return "Function Local";
        case 2: return "Formal Argument";
        default: assert(0);
    }
}

char *get_symbolTableType(int value) {
    switch (value) {
        case 0: return "Global";
        case 1: return "Local";
        case 2: return "Formal";
        case 3: return "UserFunc";
        case 4: return "LibFunc";
        case 5: return "TempVar";
        default: assert(0);
    }
    return NULL;
}

char *get_libraryFunction(int value) {
    switch (value) {
        case 0: return "print";
        case 1: return "input";
        case 2: return "objectmemberkeys";
        case 3: return "objecttotalmembers";
        case 4: return "objectcopy";
        case 5: return "totalarguments";
        case 6: return "argument";
        case 7: return "typeof";
        case 8: return "strtonum";
        case 9: return "sqrt";
        case 10: return "cos";
        case 11: return "sin";
        default: assert(0);
    }
    return NULL;

}

int libraryFunction_collision(const char *name) {
    int i;
    for (i = 0; i < 12; i++) {
        if (strcmp(name, get_libraryFunction(i)) == 0) return 1;
    }
    return 0;
}

void symbolTable_print(void) {
    unsigned int i;
    unsigned int counter = 0;
    char * str_scope;
    orderedList_p head;
    printf("\n- - - - - - - - - S Y M B O L  T A B L E - - - - - - - - -\n");
    head = orderedList_head;
    while (head != NULL) {
        counter++;
        printf("\n%2d:\t", counter);
        printf("Name: %s\n", head->symEntry->value.varVal->name);
        printf("  \tType: %s\n", get_symbolTableType(head->symEntry->type));
        printf("  \tLine: %u\n", head->symEntry->value.varVal->line);
        if (head->symEntry->value.varVal->scope == 0) {
            str_scope = "Global";
        } else str_scope = "Local";
        printf("  \tScope: %s (%u)\n", str_scope, head->symEntry->value.varVal->scope);
        if (head->symEntry->type == _USERFUNC) {
            printf("  \tFunction Address: %u\n", head->symEntry->value.funcVal->iaddress);
            printf("   \tFunction Total Local var: %u\n", head->symEntry->value.funcVal->totallocals);
            struct FuncArgs * tmpHead = head->symEntry->value.funcVal->argsList;
            printf("  \tFormal Arguments: ");
            if (tmpHead == NULL)printf("None");
            while (tmpHead != NULL) {
                printf("%s", tmpHead->varValue->name);
                tmpHead = tmpHead->next;
                if (tmpHead != NULL) printf(", ");
            }
            printf("\n");
        } else if (head->symEntry->type != _LIBFUNC) {
            printf("  \tScope Space: %s\n", get_VariableScopeSpace(head->symEntry->value.varVal->space));
            printf("  \tOffset: %u\n", head->symEntry->value.varVal->offset);
        }
        head = head->next;
    }
    printf("\nTotal Symbols on Symbol Table: %u\n", counter);
}

struct errorList * head_errorList = NULL;

struct errorList *create_error(const char* msg, unsigned int lineNum, const char * extraInfo) {
    struct errorList *tmp = malloc(sizeof (struct errorList));
    tmp->message = strdup(msg);
    tmp->line = lineNum;
    tmp->extraInfo = NULL;
    if (extraInfo != NULL) {
        tmp->extraInfo = strdup(extraInfo);
    }
    tmp->next = NULL;
};

void newError(const char* msg, unsigned int lineNum, const char * extraInfo) {
    struct errorList * head = head_errorList;
    struct errorList * tmp = create_error(msg, lineNum, extraInfo);
    if (head == NULL) {
        head_errorList = tmp;
        return;
    }
    while (head->next != NULL) {
        head = head->next;
    }
    head->next = tmp;
    return;
}

void errorList_print(void) {
    struct errorList * head = head_errorList;
    if (head != NULL) {
        printf("\n\n");
        printf("- - - - - - - - - S Y N T A X  E R R O R S - - - - - - - - -\n\n");
    }

    while (head != NULL) {
        if (head->extraInfo != NULL) {
            printf("Line: %2d\tSyntax Error: '%s'  %s \n", head->line, head->extraInfo, head->message);
        } else {
            printf("Line: %2d\tSyntax Error:  %s \n", head->line, head->message);
        }
        head = head->next;
    }
}

// ordered by line list

void orderedList_insert(SymbolTableEntry * symEntry) {
    orderedList_p newNode = orderedList_createNode(symEntry);
    if (orderedList_tail == NULL) {
        orderedList_head = newNode;
        orderedList_tail = newNode;
    } else {
        orderedList_tail->next = newNode;
        orderedList_tail = newNode;
    }
}

orderedList_p orderedList_createNode(SymbolTableEntry * symEntry) {
    orderedList_p newNode = malloc(sizeof (struct symbolTable_orderedList));
    newNode->symEntry = symEntry;
    newNode->next = NULL;

    return newNode;
}
