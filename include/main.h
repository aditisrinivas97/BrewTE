/* --------------------------- Include Guards ---------------------------- */

#ifndef MAIN_H
#define MAIN_H

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
#include "terminal.h"
#include "display.h"
#include "lineops.h"
#include "charops.h"

/* ------------------------------- Macros -------------------------------- */

/*
Bitwise AND of k with 0x1f to recognise the action to be taken
*/
#define CTRL_KEY(k) ((k) & 0x1f)

/*
Highlight a given string on the terminal
*/
#define BANNER "\033[7m"

/*
Undo the highlighting of string on terminal
*/
#define RESET "\x1B[0m"

/*
Number of times the user has to enter ^Q to exit with unsaved changes
*/
#define CONFIRM_EXIT 2


/* ------------------------- Structure Definitions ------------------------ */


struct editor_line {
    int size;
    char * content;
    int buffer_size;
    char * buffer;
};

typedef struct editor_line editor_line;

struct editor_config {
    int x;
    int y;
    int row_offset;
    int numlines;
    int rows;
    int columns;
    int status;
    char * filename;
    editor_line * line;
    struct termios orig_termios;
};

typedef struct editor_config editor_config;

/* ------------------------- Global Variables ------------------------ */

enum editorKey {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
};

extern editor_config brewte;

extern int check_unsaved;


#endif 