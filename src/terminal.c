#include "../include/terminal.h"

void init() {

    brewte.x = 0;
    brewte.y = 0;
    brewte.numlines = 0;
    brewte.row_offset = 0;
    brewte.status = 0;
    brewte.line = NULL;
    brewte.filename = NULL;
    
    write(STDOUT_FILENO, "\x1b[2J", 4);

    if (window_size(&brewte.rows, &brewte.columns) == -1){
        die("getWindowSize");
    }

    return;
}

void disable_raw_mode() {

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &brewte.orig_termios) == -1){
        die("tcsetattr");
    }

    return;
}

void enable_raw_mode() {
    
    if (tcgetattr(STDIN_FILENO, &brewte.orig_termios) == -1){
        die("tcgetattr");
    } 

    atexit(disable_raw_mode);

    struct termios raw = brewte.orig_termios;
    
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

    int num_read;
    char ch;

    while((num_read = read(STDIN_FILENO, &ch, 1)) != 1){

        if(num_read == -1 && errno != EAGAIN){
            die("read");
        }

    }

    if (ch == '\x1b') {

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
        return ch;
    }
}

void process_key() {

    int ch = editor_read();

    static int confirm_exit = CONFIRM_EXIT;

    switch(ch){
        case CTRL_KEY('q'):
            if(brewte.status && confirm_exit > 0){
                check_unsaved = 1;
                menu_bar(check_unsaved);
                confirm_exit--;
                return;
            }
            else{
                write(STDOUT_FILENO, "\x1b[2J", 4);
                write(STDOUT_FILENO, "\x1b[H", 3);
                disable_raw_mode();
                exit(0);
            }
            break;
        case ARROW_UP:
            move_cursor(ch);
            break;
        case ARROW_DOWN:
            move_cursor(ch);
            break;
        case ARROW_LEFT:
            move_cursor(ch);
            break;
        case ARROW_RIGHT:
            move_cursor(ch);
            break;
        case '\r':
            insert_newline();
            break;
        case BACKSPACE:
            remove_char_wrapper();
            break;
        case CTRL_KEY('s'):
            save();
            break;
        default:
            insert_char_wrapper(ch);
            break;
    }
    
    confirm_exit = CONFIRM_EXIT;
    check_unsaved = 0;
    return;
}

void refresh() {
    
    scroll();

    write(STDOUT_FILENO, "\x1b[H", 3);
    
    display();
    menu_bar(check_unsaved);

    printf("\x1b[%d;%dH", (brewte.y - brewte.row_offset) + 2, brewte.x + 1);
    fflush(stdout);
    
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

    editor_line * line = (brewte.y >= brewte.numlines) ? NULL : &brewte.line[brewte.y];

    switch (key) {
        case ARROW_LEFT:
            if (brewte.x != 0) {
                brewte.x--;
            }
            else if (brewte.y > 0) {
                brewte.y--;
                brewte.x = brewte.line[brewte.y].size;
            }
            break;
        case ARROW_RIGHT:
            if (line && brewte.x < line->size) {
                brewte.x++;
            }
            else if(line && brewte.x == line->size){
                brewte.x = 0;
                brewte.y++;
            }
            break;
        case ARROW_UP:
            if (brewte.y != 0) {
                brewte.y--;
            }
            break;
        case ARROW_DOWN:
            if (brewte.y < brewte.numlines) {
                brewte.y++;
            }
            break;
    }

    return;

}