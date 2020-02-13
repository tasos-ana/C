#ifndef FILEREADER_H
#define FILEREADER_H

#include "instruction.h"


void txtread(void);

void arrays_read(FILE* f);

void code_read(FILE* f);

void strings_read(FILE* f);

void numbers_read(FILE* f);

void userfunctions_read(FILE* f);

void libfunctions_read(FILE* f);

void instr_read(FILE* f);

vmarg_p operand_read(FILE* f, vmarg_p arg);

int getRealOp(char* name);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*BINARY*/

void abcread();

void abc_arrays_read(FILE* f);

void abc_code_read(FILE* f);

void abc_strings_read(FILE* f);

void abc_numbers_read(FILE* f);

void abc_userfunctions_read(FILE* f);

void abc_libfunctions_read(FILE* f);

void abc_instr_read(FILE* f);

vmarg_p abc_operand_read(FILE* f, vmarg_p arg);

#endif // FILEREADER_H
