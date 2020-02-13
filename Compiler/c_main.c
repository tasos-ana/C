#include "instruction.h"
#include "generator.h"
#include "fileWriter.h"
#include <stdio.h>
#include <stdlib.h>

#include "syn.h"

extern FILE* yyin;
extern int yyparse(void);
extern unsigned int currScope;

extern struct errorList * head_errorList;
extern struct compileError* comperror_list;

int execute_option(char * option) {
    if (strcmp(option, "-q") == 0) {
        printQuads();
    } else if (strcmp(option, "-s") == 0) {
        symbolTable_print();
    } else if (strcmp(option, "-i") == 0) {
        instr_print();
    } else {
        fprintf(stderr, "Invalid option '%s'\n", option);
        fprintf(stderr, "Options::  -q: Print Quads\n           -s: Print Symbol Table\n"
                "           -i: Print Instructions\n");
        exit(1);
    }
}

int main(int argc, char ** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot read file: %s\n", argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    currScope = 0;
    symbolTable_init();
    scopeTable_init();
    symbolTable_addLibFunc();

    yyparse();

    if (comperror_list == NULL && head_errorList == NULL) {
        FILE * f;
        if (!(f = fopen("quads.txt", "w"))) {
            fprintf(stderr, "Cannot write file: %s\n", "quads.txt");
            return 1;
        }
        writeQuads(f);

        generate_tc();
        patch_incomplete_jumps();
        if (argc > 2) {
            if (argc == 3 || argc == 4) {
                execute_option(argv[2]);
                if (argc == 4) {
                    execute_option(argv[3]);
                }
            } else {// argc > 4
                fprintf(stderr, "Too many command arguments\n", "quads.txt");
                return 1;
            }
        }
        txtwrite("tcode.txt");
        abcwrite();
        free_consts_arrays();
    } else {
        errorList_print();
        compileError_print();
    }

    return 0;
}
