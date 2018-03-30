/* --------------------------- Include Guards ---------------------------- */

#ifndef DISPLAY_H
#define DISPLAY_H

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

extern editor_config brewte;

extern int check_unsaved;

/* --------------------------- Function Headers -------------------------- */

/*
Display the banners and other output to stdout
*/
void display();

/*
Display menu bar at the bottom of the page
*/
void menu_bar(int choice);

#endif