#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include "gui_ncurses.h"
#include "requirements.h"
#include "databaseconnect.h"


int main(void) {
    if(databaseconnect_start() != 0){
        fprintf(stderr, "Error initialising databaseconnect.\n");
        exit(EXIT_FAILURE);
    }

    if(gui_printlayout() != 0){
        fprintf(stderr, "Error initialising layout.\n");
        exit(EXIT_FAILURE);
    }

    while( gui_getuseraction() != 0){
        gui_refresh();
    }

    databaseconnect_close();

    gui_close();

    return EXIT_SUCCESS;
}
