#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "symtable.h"

 struct binding{
	 char *key;
	 void *value;
	 struct binding *next;
 };

 struct SymTable{
	unsigned int length;;	/*increase that counter after 1 succesfull put*/
	struct binding *head; /*pointer on first element of our linked list bindings*/
 };

SymTable_T SymTable_new(void){
	SymTable_T tmp = malloc(sizeof(struct SymTable));
	tmp->length = 0U;
	tmp->head = NULL;
	return tmp;
}

void SymTable_free(SymTable_T oSymTable){
	assert(oSymTable!=NULL);
	struct binding* head;
	struct binding* tmp;
	head = oSymTable->head;
	while(head!=NULL){
		tmp = head->next;
		head->next=NULL;
		free(head->key);
		free(head);
		head = tmp;
	}
	oSymTable->head = NULL;
	free(oSymTable);
}

unsigned int SymTable_getLength(SymTable_T oSymTable){
	assert(oSymTable!=NULL);
	return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable,const char *pcKey,const void *pvValue){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	struct binding* head = oSymTable->head;
	if(head==NULL){
		struct binding *tmp = malloc(sizeof(struct binding));
		tmp->key = strdup(pcKey);
		tmp->value = (void *)pvValue;
		tmp->next=NULL;
		oSymTable->head = tmp;
		oSymTable->length++;
		return 1;
	}
	while(strcmp(head->key,pcKey)!=0){/*while key isnt found*/
		if (head->next == NULL){
			struct binding *tmp = malloc(sizeof(struct binding));//TODO se funct
			tmp->key = strdup(pcKey);
			tmp->value = (void *)pvValue;
			tmp->next=NULL;
			head->next=tmp;
			oSymTable->length++;
			return 1;
		}
		head=head->next;
	}
	return 0;
}

int SymTable_remove(SymTable_T oSymTable,const char *pcKey){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	struct binding *head = oSymTable->head;
	assert(head!=NULL);
	struct binding *parent = NULL;
	if(head == NULL) return 0;
	while(strcmp(head->key,pcKey)!=0){/*if we still dont find the key*/
		if(head->next==NULL) return 0;
		parent = head;
		head = head->next;
	}
	if(parent==NULL){/*key it's our list head*/
		parent = head;
		oSymTable->head = head->next;
	}else{
		parent->next = head->next;
	}
	oSymTable->length--;
	head->next = NULL;
	free(head);
	return 1;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);
	struct binding *head = oSymTable->head;
	if(head==NULL) return 0;
	while(head!=NULL){
		if(strcmp(head->key,pcKey)==0) return 1;
		head = head->next;
	}
	return 0;
}

void * SymTable_get(SymTable_T oSymTable, const char *pcKey){
	assert(oSymTable!=NULL);
	assert(pcKey!=NULL);

	struct binding *head = oSymTable->head;
	if(head==NULL) return NULL;
	while(head!=NULL){
		if( strcmp(head->key,pcKey) == 0) return (void *)head->value;
		head = head->next;
	}
	return NULL;
}

void SymTable_map(SymTable_T oSymTable,void (*pfApply)(const char *pcKey, void *pvValue,void *pvExtra),const void *pvExtra){
	assert(oSymTable!=NULL);
	assert(pfApply!=NULL);
	struct binding *head = oSymTable->head;
	if(head==NULL) return;
	while(head!=NULL){
		pfApply(head->key,&(head->value),(void*)pvExtra);
		head = head->next;
	}
}

void SymTable_print(SymTable_T oSymTable){
	assert(oSymTable!=NULL);
	struct binding *head;
	head = oSymTable->head;
	printf("Symbol Table Size are: %u\n",oSymTable->length);
	while(head!=NULL){
		printf("Key-> %s\t",head->key);
		printf("with Value-> %d\n",(int)(head->value));
		head = head->next;
	}
}
