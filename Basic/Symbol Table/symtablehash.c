#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "symtable.h"

#define BUCKETS 509

#define HASH_MULTIPLIER 65599

struct binding{
	char *key;
	void *value;
};

struct SymTable{
	unsigned int length;;	/*increase that counter after 1 succesfull put*/
	struct binding *table[BUCKETS]; /*pointer on our hash table*/
};

static unsigned int SymTable_hash(const char *pcKey){
  size_t ui;
  unsigned int uiHash = 0U;
  for (ui = 0U; pcKey[ui] != '\0'; ui++)
    uiHash = uiHash * HASH_MULTIPLIER + pcKey[ui];
  return uiHash%BUCKETS;
}

SymTable_T SymTable_new(void){
	SymTable_T oSymTable = malloc(sizeof(struct SymTable));
	oSymTable->length = 0U;
	size_t ui;
	for(ui=0U; ui<BUCKETS;ui++){
		oSymTable->table[ui]=NULL;
	}
	return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
	assert(oSymTable!=NULL);
	size_t ui;
	for(ui=0U;ui<BUCKETS; ui++){
		if(oSymTable->table[ui]!=NULL){
			free( (oSymTable->table[ui])->key );
			free( oSymTable->table[ui] );
			oSymTable->table[ui]=NULL;
		}
	}
	free(oSymTable);
}

unsigned int SymTable_getLength(SymTable_T oSymTable){
	assert(oSymTable!=NULL);
	return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable,const char* pcKey,const void* pvValue){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	size_t index;
	index = SymTable_hash(pcKey);
	if(oSymTable->table[index]!=NULL) return 0;
	struct binding *tmp;
	tmp = malloc(sizeof(struct binding));
	tmp->key = strdup(pcKey);
	tmp->value = (void *)pvValue;
	oSymTable->table[index] = tmp;
	oSymTable->length++;
	return 1;
}

int SymTable_remove(SymTable_T oSymTable,const char *pcKey){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	size_t index;
	index = SymTable_hash(pcKey);
	if(oSymTable->table[index]==NULL) return 0;
	free(oSymTable->table[index]);
	oSymTable->table[index]=NULL;
	oSymTable->length--;
	return 1;
}

int SymTable_contains(SymTable_T oSymTable,const char *pcKey){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	size_t index;
	index = SymTable_hash(pcKey);
	if(oSymTable->table[index]==NULL) return 0;
	return 1;
}

void* SymTable_get(SymTable_T oSymTable,const char *pcKey){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	size_t index;
	index = SymTable_hash(pcKey);
	if(oSymTable->table[index]==NULL) return NULL;
	return (void *)oSymTable->table[index]->value;
}

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey,void *pvValue, void *pvExtra),const void *pvExtra){
	assert(oSymTable!=NULL);
	assert(pfApply!=NULL);
	size_t ui;
	struct binding *tmp;
	for(ui=0U;ui<BUCKETS;ui++){
		tmp = oSymTable->table[ui];
		if(tmp!=NULL){
			pfApply(tmp->key,&(tmp->value),(void *)pvExtra);
		}
	}
}

void SymTable_print(SymTable_T oSymTable){
	assert(oSymTable!=NULL);
	struct binding *tmp;
	size_t ui;
	printf("Symbol Table Size are: %u\n",oSymTable->length);
	for(ui=0U;ui<BUCKETS;ui++){
		tmp = oSymTable->table[ui];
		if(tmp!=NULL){
			printf("Key-> %s\t",tmp->key);
			printf("with Value-> %d\n",(int)(tmp->value));
		}
	}
}






