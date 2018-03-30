/* --------------------------- Include Guards ---------------------------- */

#ifndef LINEOPS_H
#define LINEOPS_H

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
Open editor for reading / writing to file
*/
void editor_open(char * filename);

/*
Add line of text to display buffer
*/
void add_line(int pos, char * textline, size_t len);

/*
Vertical scrolling through large files
*/
void scroll();

/*
Updates the content of a line
*/
void update_line(editor_line * line);

/*
Remove a line of text from the document
*/
void remove_line(int pos);

/*
Concatenate 'line1' and 'line2' as 'line1line2'
*/
void concat_lines(editor_line * line1, char * line2, size_t len);

/*
Inserts a newline at the cursor position
*/
void insert_newline();

/*
Format contents of a file to be stored on disk
*/
char * format_lines(int * len);

/*
Save the content of edited file to disk
*/
void save();

/*
Free the memory allocated to a line
*/
void mem_remove_line(editor_line * line);

#endif