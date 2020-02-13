#ifndef AVMHASHTABLE_H
#define	AVMHASHTABLE_H

#include <stdio.h>
#include "avm.h"
#include "avmLibfuncs.h"

#define HASH_MULTIPLIER 6599

typedef struct avm_table_bucket {
    avm_memcell_s key;
    avm_memcell_s value;
    struct avm_table_bucket* next;
} *avm_table_bucket_p, avm_table_bucket_s;

typedef struct avm_table {
    unsigned int refCounter;
    avm_table_bucket_p strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket_p numIndexed[AVM_TABLE_HASHSIZE];
    unsigned int total;
} *avm_table_p, avm_table_s;

avm_table_p avm_tablenew(void);
void avm_tabledestroy(avm_table_p t);

void avm_table_print(avm_table_p t, unsigned int total_table_printed);
unsigned int avm_table_bucket_print(avm_table_bucket_p head, unsigned int total_table_printed);

avm_table_bucket_p avm_bucket_new(avm_memcell_p key, avm_memcell_p value);
void avm_bucket_check(avm_table_p table, avm_memcell_p value, avm_table_bucket_p head);

unsigned int str_hashFunc(char * key, int hsize);
unsigned int num_hashFunc(double key);

avm_memcell_p avm_tablegetelem(avm_table_p table, avm_memcell_p key);
void avm_tablesetelem(avm_table_p table, avm_memcell_p key, avm_memcell_p value);

void avm_tablebucketsinit(avm_table_bucket_p* p);

void avm_tablebucketsdestroy(avm_table_bucket_p* p);

/*~~~~~~~~~~~~~~~~~~LIBFUNCS HASH TABLE~~~~~~~~~~~~~~~~~~~~*/

typedef struct avm_libfuncs_bucket {
    char* key;
    library_func_t value;
    struct avm_libfuncs_bucket* next;
} avm_libfuncs_bucket_s, *avm_libfuncs_bucket_p;

avm_libfuncs_bucket_p * libfuncs_hashtable;

avm_libfuncs_bucket_p new_libfuncs_bucket(char * id, library_func_t addr);

#endif	/* AVMHASHTABLE_H */

