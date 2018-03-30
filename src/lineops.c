#include "../include/lineops.h"

void editor_open(char * filename) {
    
    int fd = open(filename, O_CREAT | O_RDWR , 0644);
    close(fd);

    FILE *fp = fopen(filename, "r");

    free(brewte.filename);
    brewte.filename = strdup(filename);
    
    char * textline = NULL;
    
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&textline, &linecap, fp)) != -1) {
        while (linelen > 0 && (textline[linelen - 1] == '\n' || textline[linelen - 1] == '\r')){
            linelen--;
        }

        add_line(brewte.numlines, textline, linelen);
    }

    brewte.status = 0;

    free(textline);
    fclose(fp);

    return;
}

void add_line(int pos, char * textline, size_t len) {

    if (pos < 0 || pos > brewte.numlines){
        return;
    }

    brewte.line = (editor_line *)realloc(brewte.line, sizeof(editor_line) * (brewte.numlines + 1));
    memmove(&brewte.line[pos + 1], &brewte.line[pos], sizeof(editor_line) * (brewte.numlines - pos));
    
    brewte.line[pos].size = len;
    brewte.line[pos].content = (char *)malloc(sizeof(char) * (len + 1));

    memcpy(brewte.line[pos].content, textline, len);

    brewte.line[pos].content[len] = '\0';
    brewte.numlines++;
    
    brewte.line[pos].buffer_size = 0;
    brewte.line[pos].buffer = NULL;

    brewte.status = 1;

    update_line(&brewte.line[pos]);

    return;
}

void update_line(editor_line * line) {

    int j;
    int index = 0;

    free(line->buffer);
    
    line->buffer = (char *)malloc(sizeof(char) * (line->size + 1));
    
    for (j = 0; j < line->size; j++) {
        line->buffer[index++] = line->content[j];
    }
    
    line->buffer[index] = '\0';
    line->buffer_size = index;

    return;
}

void scroll() {

    if (brewte.y < brewte.row_offset) {
        brewte.row_offset = brewte.y;
    }

    if (brewte.y >= brewte.row_offset + brewte.rows) {
        brewte.row_offset = brewte.y - brewte.rows + 1;
    }
    
    return;
}

void mem_remove_line(editor_line * line) {

    free(line->buffer);
    free(line->content);

    return;
}

void remove_line(int pos) {

    if (pos < 0 || pos >= brewte.numlines){
        return;
    }

    mem_remove_line(&brewte.line[pos]);
    
    memmove(&brewte.line[pos], &brewte.line[pos + 1], sizeof(editor_line) * (brewte.numlines - pos - 1));

    brewte.numlines--;
    brewte.status = 1;

    return;
}

void concat_lines(editor_line * line1, char * line2, size_t len) {

    line1->content = (char *)realloc(line1->content, sizeof(char) * (line1->size + len + 1));

    memcpy(&line1->content[line1->size], line2, len);

    line1->size += len;
    line1->content[line1->size] = '\0';

    update_line(line1);

    brewte.status = 1;

    return;
}

void insert_newline() {

    if (brewte.x == 0) {
        add_line(brewte.y, "", 0);
    }
    else {
        editor_line * line = &brewte.line[brewte.y];

        add_line(brewte.y + 1, &line->content[brewte.x], line->size - brewte.x);
        
        line = &brewte.line[brewte.y];

        line->size = brewte.x;
        line->content[line->size] = '\0';

        update_line(line);
    }

    brewte.y++;
    brewte.x = 0;

    return;
}

char * format_lines(int * len) {

    int lines_length = 0, i = 0;

    for(i = 0; i < brewte.numlines; i++){
        lines_length += brewte.line[i].size + 1;
    }
    
    *len = lines_length;
    
    char * buffer = (char * )malloc(sizeof(char) * lines_length);
    char * temp = buffer;

    for (i = 0; i < brewte.numlines; i++) {

        memcpy(temp, brewte.line[i].content, brewte.line[i].size);

        temp += brewte.line[i].size;
        *temp = '\n';

        temp++;

    }

  return buffer;

}

void save() {

    int len = -1, fd = -1;
    char * buffer = format_lines(&len);

    if (brewte.filename == NULL){
        
        time_t t = time(NULL);

        struct tm * tm = localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%c", tm);

        brewte.filename = s;
    } 
    
    fd = open(brewte.filename, O_CREAT | O_RDWR , 0644);
    ftruncate(fd, len);
    write(fd, buffer, len);
    
    brewte.status = 0;

    close(fd);
    free(buffer);

    return;
}