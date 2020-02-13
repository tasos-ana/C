/* * * * * * * * * * * * * * * * *\
 *                               *
 *        Linux keylogger        *
 *                               *
 * File:    	keylogger.h      *
 * Author:  	Tasos Anastasas  *
 * A.M:     	3166             *
 * Course:  	CS457            *
 * Exercises: 	2017             *
 * Number:   	3                *
\* * * * * * * * * * * * * * * * */
#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <stdbool.h>


/* unknown keystroke */
#define UK "UNKNOWN"
/* change it with your keyboard device event number */
#define KEYBOARD "/dev/input/event3"

/*
 * returns true when the keycode is a Shift (left or right)
 */
bool isShift(int code);

/*
 * returns true when the keycode is Esc
 */
bool isEsc(int code);

/*
 * checks if the user has root privileges
 */
bool isRoot(void);

/*
 * prints the usage message
 */
void usage(void);

/*
 * The keylogger's core functionality
 * Takes as argument the keyboard file descriptor
 * You can add more arguments if needed
 */
void keylogger(int keyboard, int outputFD);

void sigfun(int sig);

#endif // KEYLOGGER_H