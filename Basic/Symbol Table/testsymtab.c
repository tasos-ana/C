#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

 void pfApply(const char *pcKey, void *pvValue, void *pvExtra)
{
    *(int *)pvValue += *(int *)pvExtra;
}

int main(void){
  SymTable_T oSymTable;
  const char Keys[] = "abcdefgaDe";
  const char *Key2 = "secret";
  const char *Key3 = "topSecret";
  char strKey[2];
  strKey[1] ='\0';
  int x[]  = {11,8,80,74394,1908,96,666,51335,1213,344,40};
  int i,pvExtra;
  oSymTable = SymTable_new();
    /*put*/
  for(i = 0; i < sizeof(Keys)-1; i++){
    strKey[0] = Keys[i];
    printf("Putted: %s -> %d \tSucceed:%d\n" ,strKey,x[i],SymTable_put(oSymTable,strKey,x[i]));
  }
  printf("Putted: %s -> %d \tSucceed:%d\n" ,Key2,1908,SymTable_put(oSymTable,Key2,(void*)(1908)));
  printf("Putted: %s -> %d \tSucceed:%d\n" ,Key3,19,SymTable_put(oSymTable,Key3,(void*)(19)));

  /*remove*/
  printf("\nRemoved: %s \tSucceed:%d\n","a",SymTable_remove(oSymTable, "a"));
  printf("Removed: %s \tSucceed:%d\n","g",SymTable_remove(oSymTable, "g"));
  printf("Removed: %s \tSucceed:%d\n","X",SymTable_remove(oSymTable, "X"));
  printf("Removed: %s \tSucceed:%d\n","t",SymTable_remove(oSymTable, "t"));
  printf("Removed: %s \tSucceed:%d\n","D",SymTable_remove(oSymTable, "D"));
  printf("Removed: %s \tSucceed:%d\n","secret",SymTable_remove(oSymTable, "secret"));
  printf("Removed: %s \tSucceed:%d\n","b",SymTable_remove(oSymTable, "b"));
  printf("Removed: %s \tSucceed:%d\n\n","a",SymTable_remove(oSymTable, "a"));

  /*getLength*/
  printf("\nLength of the Table: %d \n\n" , SymTable_getLength(oSymTable));

  /*contains*/
  for(i = 0; i < sizeof(Keys)-1; i++){
    strKey[0] = Keys[i];
    if(SymTable_contains(oSymTable,strKey)==1) printf("Table contains : %s\n",strKey);
    else printf("Table does NOT contains : %s\n",strKey);
  }
  if(SymTable_contains(oSymTable,"topSecret")==1) printf("Table contains : %s\n","topSecret");
  else printf("Table does NOT contains : %s\n","topSecret");
  printf("\n");

  /*get*/
  for(i = 0; i < sizeof(Keys)-1; i++){
      strKey[0] = Keys[i];
    if(SymTable_get(oSymTable,strKey) != NULL)
      printf("\nKey: %s is on %d\n",strKey,(int)SymTable_get(oSymTable,strKey));
  }
  printf("\n");

  /*leitourgia map*/
  pvExtra=2;
  SymTable_map(oSymTable, &pfApply, &pvExtra);

  /*leitourgia print*/
  SymTable_print(oSymTable);

  /*leitourgia free*/
  SymTable_free(oSymTable);

  return 0;
}
