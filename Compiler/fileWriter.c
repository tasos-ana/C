#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "fileWriter.h"
#include "dataStructs.h"
#include "instruction.h"

#define magicNum 123456789

extern unsigned int totalStringConsts;
extern unsigned int totalNumConsts;
extern unsigned int totalUserFuncs;
extern unsigned int totalNamedLibfuncs;


extern char ** stringConsts;
extern double * numConsts;
extern userfunc_p userFuncs;
extern char ** namedLibfuncs;

extern unsigned int currInstr;
extern instr_p instructions;

void txtwrite(char* fname) {
    FILE * f;
    if (!(f = fopen(fname, "w"))) {
        fprintf(stderr, "Cannot write file: %s\n", fname);
        return;
    }

    fprintf(f, "%u\n", magicNum);
    arrays_write(f);
    code_write(f);

    fclose(f);
}

void arrays_write(FILE* f) {
    strings_write(f);
    numbers_write(f);
    userfunctions_write(f);
    libfunctions_write(f);
}

void code_write(FILE* f) {
    unsigned int l = currInstr;
    fprintf(f, "%u\n", l);
    int i;
    for (i = 0; i < l; ++i) {
        instr_write(f, &instructions[i]);
    }
}

void strings_write(FILE* f) {
    unsigned int l = totalStringConsts;
    fprintf(f, "%u\n", l);
    int i;
    for (i = 0; i < l; ++i) {
        fprintf(f, "%d ", strlen(stringConsts[i]) - 2);
        fprintf(f, "%s\n", stringConsts[i]);
    }
}

void numbers_write(FILE* f) {
    unsigned int l = totalNumConsts;
    fprintf(f, "%u\n", l);
    int i;
    for (i = 0; i < l; ++i) {
        fprintf(f, "%.3lf\n", numConsts[i]);
    }
}

void userfunctions_write(FILE* f) {
    unsigned int l = totalUserFuncs;
    fprintf(f, "%u\n", l);
    int i;
    for (i = 0; i < l; ++i) {
        fprintf(f, "%u ", userFuncs[i].address);
        fprintf(f, "%u ", userFuncs[i].localSize);
        fprintf(f, "%d ", strlen(userFuncs[i].id));
        fprintf(f, "%s\n", userFuncs[i].id);
    }
}

void libfunctions_write(FILE* f) {
    unsigned int l = totalNamedLibfuncs;
    fprintf(f, "%u\n", l);
    int i;
    for (i = 0; i < l; ++i) {
        fprintf(f, "%d ", strlen(namedLibfuncs[i]));
        fprintf(f, "%s\n", namedLibfuncs[i]);
    }
}

void instr_write(FILE* f, instr_p in) {
    assert(in != NULL);
    fprintf(f, "%u::", in->line);
    fprintf(f, "%s", instrName(in->opcode));

    if (in->arg1 != NULL) {
        operand_write(f, in->arg1);
    }
    if (in->arg2 != NULL) {
        operand_write(f, in->arg2);
    }
    if (in->result != NULL) {
        operand_write(f, in->result);
    }

    fprintf(f, "\n");
}

void operand_write(FILE* f, vmarg_p arg) {
    if (arg->type == retval_a || arg->type == nil_a) {
        fprintf(f, " %d", arg->type);
        return;
    }

    fprintf(f, " %d:%u", arg->type, arg->val);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*BINARY*/

void abcwrite(void) {
    FILE * f;
    if (!(f = fopen("tcode.abc", "wb"))) {
        fprintf(stderr, "Cannot write file: %s\n", "tcode.abc");
        return;
    }
    unsigned int mN = magicNum;
    fwrite(&mN, sizeof (unsigned int), 1, f);
    abc_arrays_write(f);
    abc_code_write(f);

    fclose(f);

}

void abc_arrays_write(FILE* f) {
    abc_strings_write(f);
    abc_numbers_write(f);
    abc_userfunctions_write(f);
    abc_libfunctions_write(f);
}

void abc_code_write(FILE* f) {
    unsigned int l = currInstr;
    fwrite(&l, sizeof (unsigned int), 1, f);
    int i;
    for (i = 0; i < l; ++i) {
        abc_instr_write(f, &instructions[i]);
    }
}

void abc_strings_write(FILE* f) {
    unsigned int l = totalStringConsts;
    unsigned int len;
    fwrite(&l, sizeof (unsigned int), 1, f);

    int i;
    for (i = 0; i < l; ++i) {
        len = (unsigned int) strlen(stringConsts[i]) - 2;

        fwrite(&len, sizeof (unsigned int), 1, f);
        fwrite(stringConsts[i], len + 2, 1, f);
    }
}

void abc_numbers_write(FILE* f) {
    unsigned int l = totalNumConsts;
    fwrite(&l, sizeof (unsigned int), 1, f);

    int i;
    for (i = 0; i < l; ++i) {
        fwrite(&(numConsts[i]), sizeof (double), 1, f);
    }
}

void abc_userfunctions_write(FILE* f) {
    unsigned int l = totalUserFuncs;
    unsigned int len;
    fwrite(&l, sizeof (unsigned int), 1, f);

    int i;
    for (i = 0; i < l; ++i) {
        len = (unsigned int) strlen(userFuncs[i].id);

        fwrite(&(userFuncs[i].address), sizeof (unsigned int), 1, f);
        fwrite(&(userFuncs[i].localSize), sizeof (unsigned int), 1, f);

        fwrite(&len, sizeof (unsigned int), 1, f);
        fwrite(userFuncs[i].id, len, 1, f);
    }

}

void abc_libfunctions_write(FILE* f) {
    unsigned int l = totalNamedLibfuncs;
    unsigned int len;
    fwrite(&l, sizeof (unsigned int), 1, f);

    int i;
    for (i = 0; i < l; ++i) {
        len = (unsigned int) strlen(namedLibfuncs[i]);
        fwrite(&len, sizeof (unsigned int), 1, f);

        fwrite(namedLibfuncs[i], len, 1, f);
    }

}

void abc_instr_write(FILE* f, instr_p in) {
    assert(in != NULL);
    uint8_t op;
    op = (uint8_t) in->opcode;
    fwrite(&op, sizeof (uint8_t), 1, f);

    if (in->arg1 != NULL) {
        abc_operand_write(f, in->arg1);
    }
    if (in->arg2 != NULL) {
        abc_operand_write(f, in->arg2);
    }
    if (in->result != NULL) {
        abc_operand_write(f, in->result);
    }
    fwrite(&(in->line), sizeof (unsigned int), 1, f);
}

void abc_operand_write(FILE* f, vmarg_p arg) {
    uint8_t type;
    type = (uint8_t) arg->type;
    fwrite(&type, sizeof (uint8_t), 1, f);

    if (arg->type == retval_a || arg->type == nil_a) {
        return;
    }

    unsigned int value;
    value = arg->val;

    fwrite(&value, sizeof (unsigned int), 1, f);
}
