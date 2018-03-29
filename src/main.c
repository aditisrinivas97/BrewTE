/* ------------------------------ Includes ------------------------------- */

#include "../include/main.h"

/* -------------------------- Global Variables --------------------------- */

Econfig E;

int check_unsaved = 0;

/* ---------------------------- Main Function ---------------------------- */

int main(int argc, char ** argv){

    enable_raw_mode();

    init();

    if(argc >= 2){
        editor_open(argv[1]);
    }

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
    E.row_offset = 0;
    E.status = 0;
    E.line = NULL;
    E.filename = NULL;
    
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

    static int confirm_exit = CONFIRM_EXIT;

    switch(ch){
        case CTRL_KEY('q'):
            if(E.status && confirm_exit > 0){
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
        case CTRL_KEY('h'):
            break;
        case CTRL_KEY('l'):
            break;
        case '\x1b':
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

    printf("\x1b[%d;%dH", (E.y - E.row_offset) + 2, E.x + 1);
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

    erow * line = (E.y >= E.numlines) ? NULL : &E.line[E.y];

    switch (key) {
        case ARROW_LEFT:
            if (E.x != 0) {
                E.x--;
            }
            else if (E.y > 0) {
                E.y--;
                E.x = E.line[E.y].size;
            }
            break;
        case ARROW_RIGHT:
            if (line && E.x < line->size) {
                E.x++;
            }
            else if(line && E.x == line->size){
                E.x = 0;
                E.y++;
            }
            break;
        case ARROW_UP:
            if (E.y != 0) {
                E.y--;
            }
            break;
        case ARROW_DOWN:
            if (E.y < E.numlines) {
                E.y++;
            }
            break;
    }

    return;

}

void display(){

    int i = 0, j = 0, max_len = 0;
    int textread = 0, count_lines = 0, file_line = 0;

    for(i = 0; i < E.rows; i++){
        
        write(STDOUT_FILENO, "\x1b[K", 3);
        
        file_line = i + E.row_offset - 1;
        
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
            int len = E.line[file_line].buffer_size;
            count_lines++;

            if (len > E.columns){
                len = E.columns;
            } 

            if(len > max_len){
                max_len = len;
            }

            if(count_lines > E.numlines){
                textread = 1;
            }
            else{
                write(STDOUT_FILENO, E.line[file_line].buffer, len);
            }
        }
        if(i < E.rows - 1){
            write(STDIN_FILENO, "\r\n", 2);   
        }
    }

    return;
}


void editor_open(char * filename) {
    
    FILE *fp = fopen(filename, "r");

    free(E.filename);
    E.filename = strdup(filename);

    if (!fp){
        die("fopen");
    }
    
    char * textline = NULL;
    
    size_t linecap = 0;
    ssize_t linelen;
  

    while ((linelen = getline(&textline, &linecap, fp)) != -1) {
        while (linelen > 0 && (textline[linelen - 1] == '\n' || textline[linelen - 1] == '\r')){
            linelen--;
        }

        add_line(E.numlines, textline, linelen);
    }

    E.status = 0;

    free(textline);
    fclose(fp);

    return;
}

void add_line(int pos, char * textline, size_t len) {

    if (pos < 0 || pos > E.numlines){
        return;
    }

    E.line = (erow *)realloc(E.line, sizeof(erow) * (E.numlines + 1));
    memmove(&E.line[pos + 1], &E.line[pos], sizeof(erow) * (E.numlines - pos));
    
    E.line[pos].size = len;
    E.line[pos].chars = (char *)malloc(sizeof(char) * (len + 1));

    memcpy(E.line[pos].chars, textline, len);

    E.line[pos].chars[len] = '\0';
    E.numlines++;
    
    E.line[pos].buffer_size = 0;
    E.line[pos].buffer = NULL;

    E.status = 1;

    update_line(&E.line[pos]);

    return;
}

void update_line(erow * line) {

    int j;
    int index = 0;

    free(line->buffer);
    
    line->buffer = (char *)malloc(sizeof(char) * (line->size + 1));
    
    for (j = 0; j < line->size; j++) {
        line->buffer[index++] = line->chars[j];
    }
    
    line->buffer[index] = '\0';
    line->buffer_size = index;

    return;
}

void scroll() {

    if (E.y < E.row_offset) {
        E.row_offset = E.y;
    }

    if (E.y >= E.row_offset + E.rows) {
        E.row_offset = E.y - E.rows + 1;
    }
    

    return;
}

void menu_bar(int choice){

    int j = 0;

    switch(choice){
        case 0:
            printf("\x1b[%d;%dH", E.rows, 1);
            fflush(stdout);

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
            break;
        case 1:
            printf("\x1b[%d;%dH", E.rows, 1);
            fflush(stdout);

            while(j < E.columns/2 - 30){
                printf(BANNER " " RESET);
                j++;
            }
            fflush(stdout);
            j += 58;

            printf(BANNER "You have unsaved changes! To quit anyway, enter ^Q twice!" RESET);
            fflush(stdout);

            while(j < E.columns){
                printf(BANNER " " RESET);
                j++;
            }

            printf(RESET);
            fflush(stdout);
            break;
    }
    return;
}

void insert_char_wrapper(int ch){

    if (E.y == E.numlines) {
        add_line(E.numlines, "", 0);
    }

    insert_char(&E.line[E.y], E.x, ch);
    
    E.x++;

    return;
}

void insert_char(erow * line, int pos, int ch) {
    
    if(pos < 0 || pos > line->size){
        pos = line->size;
    }

    line->chars = (char *)realloc(line->chars, sizeof(char) * (line->size + 2));

    memmove(&line->chars[pos + 1], &line->chars[pos], line->size - pos + 1);

    line->size++;
    line->chars[pos] = ch;
    
    E.status = 1;

    update_line(line);

    return;
}

void remove_char_wrapper() {

    if (E.y == E.numlines){
        return;
    }
    if (E.x == 0 && E.y == 0){
        return;
    } 
    
    erow * line = &E.line[E.y];

    if (E.x > 0) {
        remove_char(line, E.x - 1);
        E.x--;
    }
    else {
        E.x = E.line[E.y - 1].size;
        concat_lines(&E.line[E.y - 1], line->chars, line->size);
        remove_line(E.y);
        E.y--;
    }

    return;
}

void remove_char(erow * line, int pos) {

    if (pos < 0 || pos >= line->size){
        return;
    }

    memmove(&line->chars[pos], &line->chars[pos + 1], line->size - pos);

    line->size--;

    E.status = 1;

    update_line(line);

    return;
}

void mem_remove_line(erow * line) {

    free(line->buffer);
    free(line->chars);

    return;
}

void remove_line(int pos) {

    if (pos < 0 || pos >= E.numlines){
        return;
    }

    mem_remove_line(&E.line[pos]);
    
    memmove(&E.line[pos], &E.line[pos + 1], sizeof(erow) * (E.numlines - pos - 1));

    E.numlines--;
    E.status = 1;

    return;
}

void concat_lines(erow * line1, char * line2, size_t len) {

    line1->chars = (char *)realloc(line1->chars, sizeof(char) * (line1->size + len + 1));

    memcpy(&line1->chars[line1->size], line2, len);

    line1->size += len;
    line1->chars[line1->size] = '\0';

    update_line(line1);

    E.status = 1;

    return;
}

void insert_newline() {

    if (E.x == 0) {
        add_line(E.y, "", 0);
    }
    else {
        erow * line = &E.line[E.y];

        add_line(E.y + 1, &line->chars[E.x], line->size - E.x);
        
        line = &E.line[E.y];

        line->size = E.x;
        line->chars[line->size] = '\0';

        update_line(line);
    }

    E.y++;
    E.x = 0;

    return;
}

char * format_lines(int * len) {

    int lines_length = 0, i = 0;

    for(i = 0; i < E.numlines; i++){
        lines_length += E.line[i].size + 1;
    }
    
    *len = lines_length;
    
    char * buffer = (char * )malloc(sizeof(char) * lines_length);
    char * temp = buffer;

    for (i = 0; i < E.numlines; i++) {

        memcpy(temp, E.line[i].chars, E.line[i].size);

        temp += E.line[i].size;
        *temp = '\n';

        temp++;

    }

  return buffer;

}

void save() {

    int len = -1, fd = -1;
    char * buffer = format_lines(&len);

    if (E.filename == NULL){
        return;
    } 
    
    fd = open(E.filename, O_CREAT | O_RDWR , 0644);
    ftruncate(fd, len);
    write(fd, buffer, len);
    
    E.status = 0;

    close(fd);
    free(buffer);

    return;
}