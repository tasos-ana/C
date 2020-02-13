#ifndef AVM_H
#define	AVM_H

#include <string.h>

#include "dataStructs.h"
#include "instruction.h"

#define AVM_STACKSIZE 4096
#define AVM_TABLE_HASHSIZE 211
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))
#define AVM_STACKENV_SIZE 4

#define avm_error printf
#define avm_warning printf

enum avm_memcell_t {
    number_m = 0,
    string_m,
    bool_m,
    table_m,
    userfunc_m,
    libfunc_m,
    nil_m,
    undef_m
};

typedef struct avm_memcell {
    enum avm_memcell_t type;

    union {
        double numVal;
        char * strVal;
        unsigned char boolVal;
        struct avm_table * tableVal;
        unsigned int funcVal;
        char * libfuncVal;
    } data;
} avm_memcell_s, *avm_memcell_p;

void avm_initstack(void);
void avm_stack_destroy(void);


// ------------------------ TRANSLATION -----------------------------
unsigned int warning_line;
char* warning_msg;

double avm_consts_getnumber(unsigned int index);
char * avm_consts_getstring(unsigned int index);
char * avm_libfuncs_getused(unsigned int index);
userfunc_p avm_getuserfuncs(unsigned int index);

avm_memcell_p avm_translate_operand(vmarg_p arg, avm_memcell_p reg);

typedef void (*memclear_func_t)(avm_memcell_p);

void memclear_string(avm_memcell_p m);
void memclear_table(avm_memcell_p m);
void avm_memcellclear(avm_memcell_p m);

char* avm_tostring(avm_memcell_p cell);

int avm_warning_check(char* msg);

#endif	/* AVM_H */

