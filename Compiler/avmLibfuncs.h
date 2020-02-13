#ifndef AVMLIBFUNCS_H
#define AVMLIBFUNCS_H

#include "avm.h"

#define AVM_LIBFUNCS_HASHSIZE 12

typedef void (*library_func_t)(void);

unsigned int avm_totalactuals(void);

avm_memcell_p avm_getactual(unsigned int i);

void avm_init_libfuncs_hashtable();

library_func_t avm_getlibraryfunc(char * id); // typical hashing

void avm_registerlibfunc(char* id, library_func_t addr);


/*
    Implementantion of the library functions
 */

void libfunc_print(void); //DONE

void libfunc_input(void); //DONE

void libfunc_objectmemberkeys(void);

void libfunc_objecttotalmembers(void); //DONE

void libfunc_objectcopy(void);

void libfunc_totalarguments(void); //DONE

void libfunc_argument(void); //DONE

void libfunc_typeof(void); //DONE

void libfunc_strtonum(void); //DONE

void libfunc_sqrt(void); //DONE

void libfunc_cos(void); //DONE

void libfunc_sin(void); //DONE


#endif // AVMLIBFUNCS_H
