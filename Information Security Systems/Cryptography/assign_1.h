/*
 * HY-457 / Exercise 1
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2017-2018
 */
#ifndef ASSIGN_1_H
#define ASSIGN_1_H
/* function prototypes */
void print_hex(unsigned char *, size_t);

void print_string(unsigned char *, size_t);

void usage(void);

void check_args(char *, char *, unsigned char *, int, int);

int keygen(unsigned char *, unsigned char *, unsigned char *, int);

int encrypt(unsigned char *, int, unsigned char *, unsigned char *,
        unsigned char *, int);

int decrypt(unsigned char *, int, unsigned char *, unsigned char *,
        unsigned char *, int);

int gen_cmac(unsigned char *, size_t, unsigned char *, unsigned char *, int);

int verify_cmac(unsigned char *, unsigned char *);

/* TODO Declare your function prototypes here... */

int readText(unsigned char **, char *);

int writeText(unsigned char *, int, char*);

int appendText(unsigned char*, int, char*);

#endif /* ASSIGN_1_H */
/*
 * HY-457 / Exercise 1
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2017-2018
 */