/* ------------------------------ Includes ------------------------------- */

#include "../include/main.h"

/* -------------------------- Global Variables --------------------------- */

editor_config brewte;

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
