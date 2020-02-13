#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "instruction.h"


void txtwrite(char* fname);

void arrays_write(FILE* f);

void code_write(FILE* f);

void strings_write(FILE* f);

void numbers_write(FILE* f);

void userfunctions_write(FILE* f);

void libfunctions_write(FILE* f);


void instr_write(FILE* f, instr_p in);

void operand_write(FILE* f, vmarg_p arg);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*BINARY*/

void abcwrite(void);

void abc_arrays_write(FILE* f);

void abc_code_write(FILE* f);

void abc_strings_write(FILE* f);

void abc_numbers_write(FILE* f);

void abc_userfunctions_write(FILE* f);

void abc_libfunctions_write(FILE* f);

void abc_instr_write(FILE* f, instr_p in);

void abc_operand_write(FILE* f, vmarg_p arg);

#endif // FILEWRITER_H
