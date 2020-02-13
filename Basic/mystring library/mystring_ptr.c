/*
 * Anastasas Anastasios 3166
 * Ex02-Hy255-String-modules
 * mystring_ptr.c
 */
 
#include <stdio.h>
#include <assert.h>
#include "mystring.h"

/* 
 * Function ms_length.
 * Returns the length of the string pcStr.
 * It is a checked runtime error for pcStr to be NULL.
 */
size_t ms_length(const char * pcStr){
    size_t uiLength= 0U;
    assert(pcStr != NULL);
    while (* (pcStr + uiLength) != '\0')
      uiLength++;
    return uiLength;
  }

/* 
 * Function ms_copy.
 * Copies the string pointed by souStr into the array pointed by desStr.
 * It is a checked runtime error for pcStr and souStr to be NULL.
 */
char * ms_copy(char * desStr, const char * souStr){
		size_t i;
		size_t size;
		size=ms_length(souStr);
		assert(souStr != NULL);
		for (i=0; i<size; i++){
			*(desStr+i)=* (souStr+i);
		}
		*(desStr+i)='\0';
		return desStr;
}

/* 
 * Function ms_ncopy.
 * Copies the first num characters of souStr to desStr.
 * It is a checked runtime error for pcStr and souStr to be NULL.
 * It is a checked runtime error for num to be higher or equal with zero.
 */
char * ms_ncopy(char * desStr, const char * souStr, size_t num){
	size_t i;
	assert(souStr!= NULL);
	for(i=0; i<num; i++){
		if (i<=ms_length(souStr)){
			*(desStr+i)=* (souStr+i);
		}
		else {
			break;
		}
	}
	*(desStr+i)='\0';
	return desStr;
}

/* 
 * Function ms_concat.
 * Appends a copy of the souStr string to the desStr string.
 * It is a checked runtime error for  souStr to be NULL.
 */
char * ms_concat(char * desStr, const char * souStr){
	size_t i,j=0;
	size_t size;
	size=ms_length(desStr);
	assert(souStr!=NULL);
	for(i=size; * (souStr+j)!='\0'; i++){
		*(desStr+i)=* (souStr+j);
		j++;
	}
	*(desStr+i)='\0';
	return desStr;
}

/* 
 * Function ms_nconcat.
 * Appends the first num characters of souSstr to desStr.
 * It is a checked runtime error for souStr to be NULL and num to be higher or equal of zero.
 */
char * ms_nconcat(char * desStr, const char * souStr, size_t num){
	size_t i,j=0;
	size_t size;
	size=ms_length(desStr);
	assert(souStr!=NULL);
	assert(num>=0);
	for(i=size; * (souStr+j)!='\0'; i++){
		if(j<num){
		*(desStr+i)=* (souStr+j);
		j++;
		}
		else{ break; }
	}
	*(desStr+i)='\0';
	return desStr;
}

/* 
 * Function ms_compare.
 * Compares the string desStr to the string souStr.
 * It's not a checked runtime error.
 */
int ms_compare(const char * desStr,const char * souStr){
	size_t i=0;
		while(* (desStr+i)!='\0'){
			if (* (souStr+i)=='\0') return 1;
			if (* (souStr+i)>* (desStr+i)) return -1;
			if (* (souStr+i)<* (desStr+i)) return 1;
			i++;
		}
		if(* (souStr+i)!='\0') return -1;
		return 0;
}

/* 
 * Function ms_ncompare.
 * Compares up to num characters of the string desStr to those of the string souStr.
 * It is a checked runtime error for num to be higher or equal of zero.
 */
int ms_ncompare(const char * desStr, const char * souStr, size_t num){
	size_t i=0;
	assert(num>=0);
		while(* (desStr+i)!='\0' && i<num){
			if (* (souStr+i)=='\0') return 1;
			if (* (souStr+i)>* (desStr+i)) return -1;
			if (* (souStr+i)<* (desStr+i)) return 1;
			i++;
		}
		if(souStr[i]!='\0' && i<num) return -1;
		return 0;
}

/* 
 * Function ms_search.
 * Returns a pointer to the first occurrence of souStr in desStr, or a null pointer if souStr is not part of desStr
 * It is a checked runtime error for souStr & desStr to be NULL.
 */
char * ms_search(char * desStr, const char * souStr){
	 size_t i=0,j=0,l=0;
	 char * p;
	 assert(desStr!=NULL);
	 assert(souStr!=NULL);
    for(i=0; * (desStr+i)!='\0'; i++){
		p=&(* (desStr+i));
		l=i;
      for(j=0; * (souStr+j)!= '\0'; j++){
            if(* (souStr+j)!= * (desStr+l)){    
                break;
			}
				l++;
        }
        if(* (souStr+j) == '\0')
            return p;
            
        }

    return NULL;
    }

