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

/* ------------------------------- Macros -------------------------------- */

#define CTRL_KEY(k) ((k) & 0x1f)
#define BANNER "\033[7m"
#define RESET "\x1B[0m"
#define ENDH "\033[27m"

/* ------------------------- Structure Definitions ------------------------ */


struct erow {
    int size;
    char *chars;
    int buffer_size;
    char * buffer;
};

typedef struct erow erow;

struct editorConfig {
    int x;
    int y;
    int row_offset;
    int numlines;
    int rows;
    int columns;
    char *filename;
    erow * line;
    struct termios orig_termios;
};

typedef struct editorConfig Econfig;

/* ------------------------- Global Variables ------------------------ */

enum editorKey {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
};

extern Econfig E;

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

/*
Display the banners and other output to stdout
*/
void display();

/*
Open editor for reading / writing to file
*/
void editor_open(char * filename);

/*
Add line of text to display buffer
*/
void add_line(char * textline, size_t len);

/*
Vertical scrolling through large files
*/
void scroll();

/*
Display menu bar at the bottom of the page
*/
void menu_bar();

/*
Updates the content of a line
*/
void update_line(erow * line);

/*
Insert a character into a line
*/
void insert_char(erow * line, int pos, int ch);

/*
Wrapper function for insert_char
*/
void insert_char_wrapper(int ch);

/*
Format contents of a file to be stored on disk
*/
char * format_lines(int * len);

/*
Save the content of edited file to disk
*/
void save();

#endif 