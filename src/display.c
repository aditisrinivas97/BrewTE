#include "../include/display.h"

void display(){

    int i = 0, j = 0;
    int textread = 0, count_lines = 0, file_line = 0;

    for(i = 0; i < brewte.rows; i++){
        
        write(STDOUT_FILENO, "\x1b[K", 3);
        
        file_line = i + brewte.row_offset - 1;
        
        if(i == 0){
            j = 0;
            while(j < brewte.columns/2 - 25){
                printf(BANNER " " RESET);
                j++;
            }
            fflush(stdout);
            j += 48;

            printf(BANNER "Welcome to BrewTE! The Minimalistic Text Editor!" RESET);
            fflush(stdout);

            while(j < brewte.columns){
                printf(BANNER " " RESET);
                j++;
            }
            printf(RESET);
            fflush(stdout);
        }
        if(brewte.numlines > 0 && i > 0 && !textread){
            int len = brewte.line[file_line].buffer_size;
            count_lines++;

            if (len > brewte.columns){
                len = brewte.columns;
            } 

            if(count_lines > brewte.numlines){
                textread = 1;
            }
            else{
                write(STDOUT_FILENO, brewte.line[file_line].buffer, len);
            }
        }
        if(i < brewte.rows - 1){
            write(STDIN_FILENO, "\r\n", 2);   
        }
    }

    return;
}

void menu_bar(int choice){

    int j = 0;

    switch(choice){
        case 0:
            printf("\x1b[%d;%dH", brewte.rows, 1);
            fflush(stdout);

            while(j < brewte.columns/2 - 10){
                printf(BANNER " " RESET);
                j++;
            }
            fflush(stdout);
            j += 24;

            printf(BANNER "Quit : ^Q     Save : ^S" RESET);
            fflush(stdout);

            while(j < brewte.columns){
                printf(BANNER " " RESET);
                j++;
            }

            printf(RESET);
            fflush(stdout);
            break;
        case 1:
            printf("\x1b[%d;%dH", brewte.rows, 1);
            fflush(stdout);

            while(j < brewte.columns/2 - 30){
                printf(BANNER " " RESET);
                j++;
            }
            fflush(stdout);
            j += 58;

            printf(BANNER "You have unsaved changes! To quit anyway, enter ^Q twice!" RESET);
            fflush(stdout);

            while(j < brewte.columns){
                printf(BANNER " " RESET);
                j++;
            }

            printf(RESET);
            fflush(stdout);
            break;
    }
    return;
}
