/* --------------------------- Include Guards ---------------------------- */

#ifndef TERMINAL_H
#define TERMINAL_H

/* ------------------------------ Includes ------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <time.h>
#include "main.h"

/* ------------------------- Global Variables ------------------------ */

typedef struct editor_config editor_config;

extern editor_config brewte;

extern int check_unsaved;

/* --------------------------- Function Headers -------------------------- */

/* 
Initialise editor window
*/
void init();

/*
Set terminal attributes to suit text editor needs.
*/
void enable_raw_mode();

/*
Undo all the terminal attributes set by this program.
*/
void disable_raw_mode();

/*
Error handling. Prints error message when exception arises and quits.
*/
void die(const char *s);

/*
Reads input one character at a time
*/
int editor_read();

/*
Process the input
*/
void process_key();

/*
Refresh the screen / clear screen
*/
void refresh();

/*
Calculates the window size of the terminal
*/
int window_size(int *rows, int *cols) ;

/*
Moves the cursor in the editor
*/
void move_cursor(int key);

/*
Validated cursor position
*/
int cursor_position(int *rows, int *cols);

#endif