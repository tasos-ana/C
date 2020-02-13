 /*
 * Anastasas Anastasios 3166
 * Ex02-Hy255-String-modules
 * mystring.h
 */
 
#include <stddef.h>

/* 
 * Function ms_length.
 * Returns the length of the string pcStr.
 * It is a checked runtime error for pcStr to be NULL.
 */
size_t ms_length(const char *);

/* 
 * Function ms_copy.
 * Copies the string pointed by souStr into the array pointed by desStr.
 * It is a checked runtime error for pcStr and souStr to be NULL.
 */
char * ms_copy(char *, const char *);

/* 
 * Function ms_ncopy.
 * Copies the first num characters of souStr to desStr.
 * It is a checked runtime error for pcStr and souStr to be NULL.
 * It is a checked runtime error for num to be higher or equal with zero.
 */
char * ms_ncopy(char *, const char *, size_t);

/* 
 * Function ms_concat.
 * Appends a copy of the souStr string to the desStr string.
 * It is a checked runtime error for  souStr to be NULL.
 */
char * ms_concat(char *, const char *);

/* 
 * Function ms_nconcat.
 * Appends the first num characters of souSstr to desStr.
 * It is a checked runtime error for souStr to be NULL and num to be higher or equal of zero.
 */
char * ms_nconcat(char *, const char *, size_t);

/* 
 * Function ms_compare.
 * Compares the string desStr to the string souStr.
 * It's not a checked runtime error.
 */
int ms_compare(const char *, const char *);

/* 
 * Function ms_ncompare.
 * Compares up to num characters of the string desStr to those of the string souStr.
 * It is a checked runtime error for num to be higher or equal of zero.
 */
int ms_ncompare(const char *, const char *, size_t);

/* 
 * Function ms_search.
 * Returns a pointer to the first occurrence of souStr in desStr, or a null pointer if souStr is not part of desStr
 * It is a checked runtime error for souStr & desStr to be NULL.
 */
char * ms_search(char *, const char *);
