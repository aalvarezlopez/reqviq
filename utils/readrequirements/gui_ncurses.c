/**
 * @file gui_ncurses.c
 * @brief Print menus and get user actions
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-30
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include "mysql.h"
#include "gui_ncurses.h"

#include "requirements.h"
#include "databaseconnect.h"

WINDOW * mainwin;
WINDOW * requirementswindow[4];
WINDOW * projectselector;
WINDOW * footer;

guimainstate_et g_state = INIT;

uint8_t g_n_projects = 0;
uint8_t selectedproject = 0;
uint8_t selectedlayer = 0;
uint8_t selectedrequirement[] = {0, 0, 0, 0};
bool g_show_only_related = false;
uint8_t g_n_requirements[] = {0, 0, 0, 0};
uint16_t g_req_uid[10][REQ_TITLE_LEN];

void print_projects(void);

uint8_t gui_printlayout(void)
{
    uint16_t maxX, maxY;
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    getmaxyx(stdscr, maxX, maxY);
    maxX -= 5;
    curs_set(FALSE);
    noecho();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_CYAN);
    init_pair(2, COLOR_YELLOW, COLOR_CYAN);
    init_pair(3, COLOR_MAGENTA, COLOR_CYAN);
    init_pair(4, COLOR_WHITE, COLOR_CYAN);
    init_pair(5, COLOR_BLUE, COLOR_CYAN);
    init_pair(6, COLOR_WHITE, COLOR_GREEN);
    init_pair(7, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(8, COLOR_WHITE, COLOR_YELLOW);

    requirementswindow[0] = newwin(maxX/3, maxY, 0, 0);
    wbkgd(requirementswindow[0], COLOR_PAIR(1));
    wattron(requirementswindow[0], COLOR_PAIR(8));
    mvwprintw(requirementswindow[0], 0, 0, " + Project Requirements");
    whline(requirementswindow[0], ' ', maxY);
    wattron(requirementswindow[0], COLOR_PAIR(4));

    requirementswindow[1] = newwin(maxX/3, maxY, maxX/3 * 1, 0);
    wbkgd(requirementswindow[1], COLOR_PAIR(1));
    wattron(requirementswindow[1], COLOR_PAIR(8));
    mvwprintw(requirementswindow[1], 0, 0, " - System Requirements");
    whline(requirementswindow[1], ' ', maxY);
    wattron(requirementswindow[1], COLOR_PAIR(4));

    requirementswindow[2] = newwin(maxX/3, maxY/2, maxX/3 * 2, 0);
    wbkgd(requirementswindow[2], COLOR_PAIR(1));
    wattron(requirementswindow[2], COLOR_PAIR(8));
    mvwprintw(requirementswindow[2], 0, 0, " - Software Requirements");
    whline(requirementswindow[2], ' ', maxY/2);
    wattron(requirementswindow[2], COLOR_PAIR(4));

    requirementswindow[3] = newwin(maxX/3, maxY/2, maxX/3 * 2, maxY/2);
    wbkgd(requirementswindow[3], COLOR_PAIR(1));
    wattron(requirementswindow[3], COLOR_PAIR(8));
    mvwprintw(requirementswindow[3], 0, 0, " - Hardware Requirements");
    whline(requirementswindow[3], ' ', maxY/2);
    wattron(requirementswindow[3], COLOR_PAIR(4));

    footer = newwin(5, maxY, maxX, 0);
    wbkgd(footer, COLOR_PAIR(6));

    projectselector = newwin( 10, 40, maxX/2 - 5, maxY/2 - 20);
    wbkgd(footer, COLOR_PAIR(6));
    mvwprintw(projectselector, 1, 3, "Chose a project:");

    print_projects();

    wrefresh(requirementswindow[0]);
    wrefresh(requirementswindow[1]);
    wrefresh(requirementswindow[2]);
    wrefresh(requirementswindow[3]);
    wrefresh(footer);
    wrefresh(projectselector);
}

void gui_close(void)
{
    delwin(requirementswindow[0]);
    delwin(requirementswindow[1]);
    delwin(requirementswindow[2]);
    delwin(requirementswindow[3]);
    delwin(footer);
    endwin();
}

void gui_printmessage(char *message)
{
    WINDOW * popupmessage;
    popupmessage = newwin(10, 100, 20, 50);
    mvwprintw(popupmessage, 0, 2, message);
    wrefresh(popupmessage);
    wgetch(popupmessage);
    delwin(popupmessage);
    touchwin(requirementswindow[1]);
    wrefresh(requirementswindow[1]);
}

void gui_regreshproject(void)
{
    for(uint8_t i = 0; i < g_n_projects; i++){
        if( selectedproject == i ){
            mvwprintw(projectselector, i + 2, 0, "-->");
        }else{
            mvwprintw(projectselector, i + 2, 0, "   ");
        }
    }
    wrefresh(projectselector);
}

void gui_refreshselectedrequirement(void)
{
    for(uint8_t i = 0; i < N_REQ_IN_A_WINDOW; i++){
        if( selectedlayer == 0 && selectedrequirement[selectedlayer] == i){
            mvwprintw(requirementswindow[0], i + 2, 0, "-->");
        }else{
            mvwprintw(requirementswindow[0], i + 2, 0, "   ");
        }
        if( selectedlayer == 1 && selectedrequirement[selectedlayer] == i){
            mvwprintw(requirementswindow[1], i + 2, 0, "-->");
        }else{
            mvwprintw(requirementswindow[1], i + 2, 0, "   ");
        }
        if( selectedlayer == 2 && selectedrequirement[selectedlayer] == i){
            mvwprintw(requirementswindow[2], i + 2, 0, "-->");
        }else{
            mvwprintw(requirementswindow[2], i + 2, 0, "   ");
        }
        if( selectedlayer == 3 && selectedrequirement[selectedlayer] == i){
            mvwprintw(requirementswindow[3], i + 2, 0, "-->");
        }else{
            mvwprintw(requirementswindow[3], i + 2, 0, "   ");
        }
    }
}

void gui_regreshmain(void)
{
    char title[10][128];
    char description[10][512];
    uint16_t n_requirements;
    for(uint8_t layer = 0; layer < N_LAYERS; layer++){
        uint8_t uid = 1;
        if( g_show_only_related){
            g_n_requirements[layer] = requirement_getrequirement(title, description, NULL, layer + 1, g_req_uid[selectedlayer][selectedrequirement[selectedlayer]]);
            for(uint8_t i = 0; i < g_n_requirements[layer]; i++){
                mvwprintw(requirementswindow[layer], i + 2, 3, title[i]);
                wclrtoeol(requirementswindow[layer]);
            }
            for(uint8_t i = g_n_requirements[layer]; i < N_REQ_IN_A_WINDOW; i++){
                mvwprintw(requirementswindow[layer], i + 2, 3, "*");
                wclrtoeol(requirementswindow[layer]);
            }
        }else{
            g_n_requirements[layer] = requirement_getallrequirements(title, description, g_req_uid[layer], layer + 1);
            for(uint8_t i = 0; i < g_n_requirements[layer]; i++){
                mvwprintw(requirementswindow[layer], i + 2, 3, title[i]);
                wclrtoeol(requirementswindow[layer]);
            }
            for(uint8_t i = g_n_requirements[layer]; i < N_REQ_IN_A_WINDOW; i++){
                mvwprintw(requirementswindow[layer], i + 2, 3, "*");
                wclrtoeol(requirementswindow[layer]);
            }
        }
        wattron(requirementswindow[layer], COLOR_PAIR(8));
        mvwprintw(requirementswindow[layer],  0, 0, selectedlayer == layer ? " +" : " -");
        wattron(requirementswindow[layer], COLOR_PAIR(4));
        wrefresh(requirementswindow[layer]);
    }
}

void gui_refresh(void)
{
    if( g_state == INIT ){
        gui_regreshproject();
    }else{
        gui_regreshmain();
    }
}

void gui_getuseractioninit(char keychar)
{
    if( keychar == 'j' ){
        if( selectedproject == 0){
            selectedproject = g_n_projects - 1;
        }else{
            selectedproject--;
        }
    }else if(keychar == 'k'){
        selectedproject++;
        if( selectedproject >= g_n_projects){
            selectedproject = 0;
        }
    }else if(keychar == '\n' || keychar == '\r'){
        g_state = MAIN;
        delwin(projectselector);
        touchwin(requirementswindow[1]);
        wrefresh(requirementswindow[1]);
        requirement_init(selectedproject);
    }
}

void gui_getuseractionmain(char keychar)
{
    if( keychar == 'k' ){
        if( selectedlayer == 0){
            selectedlayer = 3;
        }else{
            selectedlayer--;
        }
    }else if(keychar == 'j'){
        selectedlayer++;
        if( selectedlayer >= 4){
            selectedlayer = 0;
        }
    }else if(keychar == 'l'){
        if(selectedrequirement[selectedlayer] == 0){
            selectedrequirement[selectedlayer] = (g_n_requirements[selectedlayer] - 1);
        }else{
            selectedrequirement[selectedlayer]--;
        }
    }else if(keychar == 'h'){
        selectedrequirement[selectedlayer]++;
        selectedrequirement[selectedlayer] %= g_n_requirements[selectedlayer];
    }else if(keychar == 'r'){
        g_show_only_related = true;
    }else if(keychar == 'R'){
        g_show_only_related = false;
    }
    gui_refreshselectedrequirement();
}

uint8_t gui_getuseraction(void)
{
    char keychar = 0;
    keychar = wgetch(footer);
    if(g_state == INIT){
        gui_getuseractioninit(keychar);
    }else{
        gui_getuseractionmain(keychar);
    }
    return keychar == 'q' ? 0 : 1;
}

void print_projects(void)
{
    char title[10][32];
    char description[10][512];
    g_n_projects = databaseconnect_getprojects(title, description, 10);
    for(uint8_t i = 0; i < g_n_projects; i++){
        mvwprintw(projectselector, 2 + i, 3, title[i]);
    }
    mvwprintw(projectselector, 2 , 0, "-->");
}
