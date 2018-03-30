/* --------------------------- Include Guards ---------------------------- */

#ifndef CHAROPS_H
#define CHAROPS_H

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

typedef struct editor_line editor_line;

extern editor_config brewte;

extern int check_unsaved;

/* --------------------------- Function Headers -------------------------- */

/*
Insert a character into a line
*/
void insert_char(editor_line * line, int pos, int ch);

/*
Wrapper function for insert_char
*/
void insert_char_wrapper(int ch);

/*
Wrapper function for remove_char
*/
void remove_char_wrapper();

/*
Removes a character from a given line
*/
void remove_char(editor_line * line, int pos);

#endif