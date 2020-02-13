#ifndef SYMBOLTABLE_H
#define	SYMBOLTABLE_H

#include <stdio.h>
#include "scopeSpaces.h"
#include "dataStructs.h"

#define SCOPE_BUCKETS 512
#define SYMBOL_BUCKETS 509

#define HASH_MULTIPLIER 6599

typedef struct list* list_p;

enum SymbolTableType {
    _GLOBAL, _LOCAL, _FORMAL,
    _USERFUNC, _LIBFUNC, _TEMPVAR
};

enum LibraryFunctions {
    print, input, objectmemberkeys, objecttotalmembers,
    objectcopy, totalarguments, argument, _typeof, strtonum,
    _sqrt, _cos, _sin
};

typedef struct Variable {
    const char * name;
    unsigned int scope;
    unsigned int line;
    enum scopespace_t space;
    unsigned int offset;
} Variable;

typedef struct Function {
    const char * name;
    unsigned int scope;
    unsigned int line;
    unsigned int iaddress;
    unsigned int taddress;
    list_p returnList;
    unsigned int totallocals;
    struct FuncArgs *argsList;
} Function;

typedef struct FuncArgs {
    Variable * varValue;
    struct FuncArgs *next;
} FuncArgs;

typedef struct SymbolTableEntry {
    int isActive;

    union {
        Variable * varVal;
        Function * funcVal;
    } value;
    enum SymbolTableType type;

    struct SymbolTableEntry * next;
    struct SymbolTableEntry * nextScope;
} SymbolTableEntry;

SymbolTableEntry * create_newSymbol(enum SymbolTableType type, const char *name, unsigned int scope, unsigned int line, enum scopespace_t space, unsigned int offset);
Variable * create_variable(const char* name, unsigned int scope, unsigned int line, enum scopespace_t space, unsigned int offset);
Function * create_function(const char* name, unsigned int scope, unsigned int line);
FuncArgs * create_funcArgs(Variable * varValue);
Function * add_funcArgs(Function * func, FuncArgs * newArg);

static unsigned int symbolTable_hashFunc(const char *name);

void symbolTable_init(void);
SymbolTableEntry * symbolTable_insert(SymbolTableEntry *newSymbol);
SymbolTableEntry * symbolTable_lookup(const char* name);
void symbolTable_addLibFunc(void);

void scopeTable_init(void);
void scopeTable_insert(SymbolTableEntry *newSymbol);
SymbolTableEntry * scopeTable_lookup(const char* name, unsigned int scope);
void scopeTable_hide(unsigned int scope);

char *get_symbolTableType(int value);
char *get_VariableScopeSpace(int value);

char *get_libraryFunction(int value);
int libraryFunction_collision(const char *name);

void symbolTable_print(void);

struct errorList {
    char * message;
    unsigned int line;
    char * extraInfo;
    struct errorList * next;
};

struct errorList *create_error(const char* msg, unsigned int lineNum, const char * extraInfo);

void newError(const char* msg, unsigned int lineNum, const char * extraInfo);
void errorList_print(void);



// ordered by line list

typedef struct symbolTable_orderedList {
    SymbolTableEntry * symEntry;
    struct symbolTable_orderedList * next;
} *orderedList_p;

void orderedList_insert(SymbolTableEntry * symEntry);
orderedList_p orderedList_createNode(SymbolTableEntry * symEntry);

#endif	/* SYMBOLTABLE_H */
