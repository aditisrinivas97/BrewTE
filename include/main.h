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
    int status;
    char * filename;
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
void add_line(int pos, char * textline, size_t len);

/*
Vertical scrolling through large files
*/
void scroll();

/*
Display menu bar at the bottom of the page
*/
void menu_bar(int choice);

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

/*
Wrapper function for remove_char
*/
void remove_char_wrapper();

/*
Removes a character from a given line
*/
void remove_char(erow * line, int pos);

/*
Free the memory allocated to a line
*/
void mem_remove_line(erow * line);

/*
Remove a line of text from the document
*/
void remove_line(int pos);

/*
Concatenate 'line1' and 'line2' as 'line1line2'
*/
void concat_lines(erow * line1, char * line2, size_t len);

/*
Inserts a newline at the cursor position
*/
void insert_newline();

#endif 