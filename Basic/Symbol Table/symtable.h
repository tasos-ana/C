typedef struct SymTable * SymTable_T;

/*Returns pointer on new empty SymTable*/
SymTable_T SymTable_new(void);

/*Free all the memory that allocated for symTable*/
void SymTable_free(SymTable_T oSymTable);

/*Return the number of the bindings*/
unsigned int SymTable_getLength(SymTable_T oSymTable);

/*Add new key if that isn't contained on symTable and return 1 otherwise 0*/
int	SymTable_put(SymTable_T oSymTable, const char *pcKey,const void *pvValue);

/*Removing the key if that contained on symTable and returning 1, 0 otherwise*/
int	SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*Return 1 if the key are on symTable, 0 otherwise*/
int	SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*Return the value of the key if that contained on symTable*/
void* SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*Applying the pfApply function on every binding*/
void SymTable_map(SymTable_T oSymTable,void (*pfApply)(const char *pcKey, void *pvValue,void *pvExtra),const void *pvExtra);

/*Printing the sumTable just for test if it's working*/
void SymTable_print(SymTable_T oSymTable);
