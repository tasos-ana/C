#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "fileReader.h"
#include "dataStructs.h"
#include "instruction.h"

#define magicNum 123456789

extern unsigned int totalStringConsts;
extern unsigned int totalNumConsts;
extern unsigned int totalUserFuncs;
extern unsigned int totalNamedLibfuncs;
extern unsigned int globalCounter;

extern unsigned int currInstr;

typedef struct vmarg* vmarg_p;

void txtread(void) {
    FILE * f;
    if (!(f = fopen("tcode.txt", "r"))) {
        fprintf(stderr, "Cannot read file: %s\n", "tcode.txt");
        return;
    }
    unsigned int magicN;
    fscanf(f, "%u", &magicN);
    if (magicN != magicNum) {
        fprintf(stderr, "%s\n", "Wrong magic number");
        exit(0);
    }
    arrays_read(f);
    code_read(f);
    fclose(f);
}

void arrays_read(FILE* f) {
    strings_read(f);
    numbers_read(f);
    userfunctions_read(f);
    libfunctions_read(f);
}

void code_read(FILE* f) {
    unsigned int total_code;
    fscanf(f, "%u", &total_code);
    int i;
    for (i = 0; i < total_code; ++i) {
        instr_read(f);
    }
    assert(total_code == nextinstr());
}

void strings_read(FILE* f) {
    unsigned int total_str;
    fscanf(f, "%u", &total_str);
    int i;
    int len;
    for (i = 0; i < total_str; ++i) {
        fscanf(f, "%d", &len);
        char array_tmp[len + 1];

        char trash;
        fscanf(f, "%c", &trash);
        char c;
        int i;
        c = fgetc(f);
        for (i = 0; i < len; ++i) {
            c = fgetc(f);
            array_tmp[i] = c;
        }
        array_tmp[len] = '\0';
        c = fgetc(f);
        ;

        consts_newstring(array_tmp);
    }

    assert(total_str == totalStringConsts);
}

void numbers_read(FILE* f) {
    unsigned int total_num;
    fscanf(f, "%u", &total_num);
    int i;
    double val;
    for (i = 0; i < total_num; ++i) {
        fscanf(f, "%lf", &val);

        consts_newnumber(val);
    }

    assert(total_num == totalNumConsts);
}

void userfunctions_read(FILE* f) {
    unsigned int total_uf;
    fscanf(f, "%u", &total_uf);
    int i, tmp;
    unsigned int addr, locSize;
    for (i = 0; i < total_uf; ++i) {

        fscanf(f, "%u", &addr);
        fscanf(f, "%u", &locSize);
        fscanf(f, "%d", &tmp);

        const char array_tmp[tmp + 1];

        fscanf(f, "%s", array_tmp);

        userfuncs_add(addr, array_tmp, locSize);
    }
    assert(total_uf == totalUserFuncs);
}

void libfunctions_read(FILE* f) {
    unsigned int total_lf;
    fscanf(f, "%u", &total_lf);
    int i;
    int tmp;
    for (i = 0; i < total_lf; ++i) {
        fscanf(f, "%d", &tmp);

        const char array_tmp[tmp + 1];
        fscanf(f, "%s", array_tmp);

        libfuncs_newused(array_tmp);
    }
    assert(total_lf == totalNamedLibfuncs);
}

void instr_read(FILE* f) {
    instr_s t;
    instr_reset(&t);

    unsigned int instr_line;
    fscanf(f, "%u", &instr_line);
    t.line = instr_line;

    char trash;
    fscanf(f, "%c", &trash);
    fscanf(f, "%c", &trash);

    char opcode_str[13];
    fscanf(f, "%s", opcode_str);
    enum vmopcode op = getRealOp(opcode_str);
    t.opcode = op;
    switch (op) {
        case funcenter_v:
        case funcexit_v:
        case jump_v:
        {
            t.result = operand_read(f, t.result);
            break;
        }
        case newtable_v:
        case callfunc_v:
        case pusharg_v:
        {
            t.arg1 = operand_read(f, t.arg1);
            break;
        }
        case assign_v:
        {
            t.arg1 = operand_read(f, t.arg1);
            t.result = operand_read(f, t.result);
            break;
        }
        default:
        {
            t.arg1 = operand_read(f, t.arg1);
            t.arg2 = operand_read(f, t.arg2);
            t.result = operand_read(f, t.result);
        }
    }
    instr_emit(t);
}

vmarg_p operand_read(FILE* f, vmarg_p arg) {
    arg = (vmarg_p) malloc(sizeof (vmarg_s));
    fscanf(f, "%d", &(arg->type));
    if (arg->type == retval_a || arg->type == nil_a) {
        return arg;
    }
    char trash;
    fscanf(f, "%c", &trash);
    fscanf(f, "%u", &(arg->val));
    if (arg->type == global_a) {
        ++globalCounter;
    }

    return arg;
}

int getRealOp(char* name) {
    int i;
    for (i = 0; i < 20; ++i) {
        if (strcmp(name, instrName(i)) == 0) {
            return i;
        }
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*BINARY*/

void abcread(void) {
    FILE * f;
    if (!(f = fopen("tcode.abc", "rb"))) {
        fprintf(stderr, "Cannot read file: %s\n", "tcode.abc");
        return;
    }
    unsigned int magicN;

    size_t res = fread(&magicN, 1, sizeof (unsigned int), f);

    if (magicN != magicNum) {
        fprintf(stderr, "%s\n", "Wrong magic number");
        exit(0);
    }
    abc_arrays_read(f);
    abc_code_read(f);

    fclose(f);
}

void abc_arrays_read(FILE* f) {
    abc_strings_read(f);
    abc_numbers_read(f);
    abc_userfunctions_read(f);
    abc_libfunctions_read(f);
}

void abc_code_read(FILE* f) {
    unsigned int total_code;
    fread(&total_code, sizeof (unsigned int), 1, f);
    int i;
    for (i = 0; i < total_code; ++i) {
        abc_instr_read(f);
    }
    assert(total_code == currInstr);
}

void abc_strings_read(FILE* f) {
    unsigned int total_str;
    fread(&total_str, sizeof (unsigned int), 1, f);
    int i;
    unsigned int len;
    char trash;
    for (i = 0; i < total_str; ++i) {
        fread(&len, sizeof (unsigned int), 1, f);
        char array_tmp[len + 1];

        fread(&trash, sizeof (char), 1, f);
        fread(array_tmp, len, 1, f);
        fread(&trash, sizeof (char), 1, f);

        array_tmp[len] = '\0';
        consts_newstring(array_tmp);
    }

    assert(total_str == totalStringConsts);
}

void abc_numbers_read(FILE* f) {
    unsigned int total_num;
    fread(&total_num, sizeof (unsigned int), 1, f);

    int i;
    double val;
    for (i = 0; i < total_num; ++i) {
        fread(&val, sizeof (double), 1, f);
        consts_newnumber(val);
    }
    assert(total_num == totalNumConsts);
}

void abc_userfunctions_read(FILE* f) {
    unsigned int total_uf;
    fread(&total_uf, sizeof (unsigned int), 1, f);

    int i;
    unsigned int addr, locSize, len;

    for (i = 0; i < total_uf; ++i) {

        fread(&addr, sizeof (unsigned int), 1, f);
        fread(&locSize, sizeof (unsigned int), 1, f);

        fread(&len, sizeof (unsigned int), 1, f);

        char array_tmp[len + 1];
        fread(array_tmp, len, 1, f);

        array_tmp[len] = '\0';

        userfuncs_add(addr, array_tmp, locSize);
    }
    assert(total_uf == totalUserFuncs);
}

void abc_libfunctions_read(FILE* f) {
    unsigned int total_lf;
    unsigned int len;
    fread(&total_lf, sizeof (unsigned int), 1, f);

    int i;
    for (i = 0; i < total_lf; ++i) {

        fread(&len, sizeof (unsigned int), 1, f);
        char array_tmp[len + 1];
        fread(array_tmp, len, 1, f);
        array_tmp[len] = '\0';
        libfuncs_newused(array_tmp);
    }
    assert(total_lf == totalNamedLibfuncs);
}

void abc_instr_read(FILE* f) {
    instr_s t;
    instr_reset(&t);

    uint8_t op;
    fread(&op, sizeof (uint8_t), 1, f);

    t.opcode = op;
    switch (op) {
        case funcenter_v:
        case funcexit_v:
        case jump_v:
        {
            t.result = abc_operand_read(f, t.result);
            break;
        }
        case newtable_v:
        case callfunc_v:
        case pusharg_v:
        {
            t.arg1 = abc_operand_read(f, t.arg1);
            break;
        }
        case assign_v:
        {
            t.arg1 = abc_operand_read(f, t.arg1);
            t.result = abc_operand_read(f, t.result);
            break;
        }
        default:
        {
            t.arg1 = abc_operand_read(f, t.arg1);
            t.arg2 = abc_operand_read(f, t.arg2);
            t.result = abc_operand_read(f, t.result);
        }
    }
    fread(&(t.line), sizeof (unsigned int), 1, f);
    instr_emit(t);
}

vmarg_p abc_operand_read(FILE* f, vmarg_p arg) {
    arg = (vmarg_p) malloc(sizeof (vmarg_s));
    fread(&(arg->type), sizeof (uint8_t), 1, f);

    if (arg->type == retval_a || arg->type == nil_a) {
        return arg;
    }
    if (arg->type == global_a) {
        ++globalCounter;
    }
    fread(&(arg->val), sizeof (unsigned int), 1, f);
    return arg;
}
