/* ------------------------------ Includes ------------------------------- */

#include "../include/main.h"

/* -------------------------- Global Variables --------------------------- */

Econfig E;

/* ---------------------------- Main Function ---------------------------- */

int main(void){

    enable_raw_mode();

    init();
    editor_open();

    while (1) {

        refresh();
        process_key();
    }

    return 0;
}

/* ------------------------ Function Definitions ------------------------ */

void init() {

    E.x = 0;
    E.y = 0;
    E.numlines = 0;
    
    write(STDOUT_FILENO, "\x1b[2J", 4);

    if (window_size(&E.rows, &E.columns) == -1){
        die("getWindowSize");
    }

    return;
}

void disable_raw_mode() {

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1){
        die("tcsetattr");
    }

    return;
}

void enable_raw_mode() {
    
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1){
        die("tcgetattr");
    } 

    atexit(disable_raw_mode);

    struct termios raw = E.orig_termios;
    
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 100;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){
        die("tcsetattr");
    } 

    return;
}

void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(1);
}

int editor_read() {

    int nread;
    char c;

    while((nread = read(STDIN_FILENO, &c, 1)) != 1){

        if(nread == -1 && errno != EAGAIN){
            die("read");
        }

    }

    if (c == '\x1b') {

        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {

            switch (seq[1]) {
                case 'A': return ARROW_UP;
                case 'B': return ARROW_DOWN;
                case 'C': return ARROW_RIGHT;
                case 'D': return ARROW_LEFT;
            }

        }
        return '\x1b';
    } 
    else {
        return c;
    }
}

void process_key() {

    int ch = editor_read();

    switch(ch){
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            disable_raw_mode();
            exit(0);
            break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            move_cursor(ch);
            break;
    }

    return;
}

void refresh() {

    write(STDOUT_FILENO, "\x1b[H", 3);
    
    display();
    
    return;
    
}

int cursor_position(int *rows, int *cols) {

    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4){
        return -1;
    }

    while (i < sizeof(buf) - 1) {

        if (read(STDIN_FILENO, &buf[i], 1) != 1){
            break;
        }

        if (buf[i] == 'R'){
            break;
        }
        i++;
    }

    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '['){
        return -1;
    }
    
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2){
        return -1;
    }

    return 0;
}

int window_size(int *rows, int *cols) {

    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) 
            return -1;
        return cursor_position(rows, cols);
    } 
    else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }

}

void move_cursor(int key) {

    switch (key) {
        case ARROW_LEFT:
            if (E.x != 0) {
                E.x--;
            }
            break;
        case ARROW_RIGHT:
            if (E.x != E.columns - 1) {
                E.x++;
            }
            break;
        case ARROW_UP:
            if (E.y != 0) {
                E.y--;
            }
            break;
        case ARROW_DOWN:
            if (E.y != E.rows - 1) {
                E.y++;
            }
            break;
    }

    return;

}

void display(){

    int i = 0, j = 0;
    int textread = 0;

    for(i = 0; i < E.rows; i++){
        if(i == 0){
            j = 0;
            while(j < E.columns/2 - 25){
                printf(BANNER " " RESET);
                j++;
            }
            fflush(stdout);
            j += 48;

            printf(BANNER "Welcome to BrewTE! The Minimalistic Text Editor!" RESET);
            fflush(stdout);

            while(j < E.columns){
                printf(BANNER " " RESET);
                j++;
            }
            printf(RESET);
            fflush(stdout);
        }
        if(E.numlines > 0 && i > 0 && !textread){
            textread = 1;
            int len = E.line.size;
            if (len > E.columns) 
                len = E.columns;
            write(STDIN_FILENO, E.line.chars, len);
        }
        if(i < E.rows - 1){
            write(STDIN_FILENO, "\r\n", 2);   
        }
        if(i == E.rows - 1){
            j = 0;
            while(j < E.columns/2 - 10){
                printf(BANNER " " RESET);
                j++;
            }
            fflush(stdout);
            j += 24;

            printf(BANNER "Quit : ^Q     Save : ^S" RESET);
            fflush(stdout);

            while(j < E.columns){
                printf(BANNER " " RESET);
                j++;
            }
            printf(RESET);
            fflush(stdout);
        }
    }
    
    printf("\x1b[%d;%dH", E.y + 1, E.x + 1);
    fflush(stdout);

    return;
}


void editor_open() {
    char * textline = "Hello, world!";
    ssize_t linelen = 13;

    E.line.size = linelen;
    E.line.chars = malloc(linelen + 1);
    
    memcpy(E.line.chars, textline, linelen);
    
    E.line.chars[linelen] = '\0';
    E.numlines = 1;

    return;
}