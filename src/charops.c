#include "../include/charops.h"

void insert_char_wrapper(int ch){

    if (brewte.y == brewte.numlines) {
        add_line(brewte.numlines, "", 0);
    }

    insert_char(&brewte.line[brewte.y], brewte.x, ch);
    
    brewte.x++;

    return;
}

void insert_char(editor_line * line, int pos, int ch) {
    
    if(pos < 0 || pos > line->size){
        pos = line->size;
    }

    line->content = (char *)realloc(line->content, sizeof(char) * (line->size + 2));

    memmove(&line->content[pos + 1], &line->content[pos], line->size - pos + 1);

    line->size++;
    line->content[pos] = ch;
    
    brewte.status = 1;

    update_line(line);

    return;
}

void remove_char_wrapper() {

    if (brewte.y == brewte.numlines){
        return;
    }
    if (brewte.x == 0 && brewte.y == 0){
        return;
    } 
    
    editor_line * line = &brewte.line[brewte.y];

    if (brewte.x > 0) {
        remove_char(line, brewte.x - 1);
        brewte.x--;
    }
    else {
        brewte.x = brewte.line[brewte.y - 1].size;
        concat_lines(&brewte.line[brewte.y - 1], line->content, line->size);
        remove_line(brewte.y);
        brewte.y--;
    }

    return;
}

void remove_char(editor_line * line, int pos) {

    if (pos < 0 || pos >= line->size){
        return;
    }

    memmove(&line->content[pos], &line->content[pos + 1], line->size - pos);

    line->size--;

    brewte.status = 1;

    update_line(line);

    return;
}
