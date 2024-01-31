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

#include "databaseconnect.h"

WINDOW * mainwin;
WINDOW * projects_window;
WINDOW * newrequirement_window;
WINDOW * latestrequirements_window;
WINDOW * footer;

guimainstate_et g_state = INIT;
uint8_t g_current_project = 0;
uint8_t g_n_projects = 0;
uint8_t g_current_row = 0;

char title_value[128];
char description_value[512];
uint8_t level_value = 0;

#define N_OF_ROWS 2
#define N_OF_LEVELS 4
#define DEFAULT_FOOTER "Press: 'q' to exit; 'a' to add new requirement; 'pX' to chose project X"

void gui_actioninmain(char keychar);
void gui_actioninedit(keychar);

uint8_t gui_printlayout(void)
{
    strcpy(title_value, "Insert title here");
    strcpy(description_value, "Insert description here");
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
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

    projects_window = newwin(10, 200, 0, 0);
    wbkgd(projects_window, COLOR_PAIR(1));
    mvwprintw(projects_window, 0, 2, "Projects:");
    mvwprintw(projects_window, 1, 5, "Title");
    mvwprintw(projects_window, 1, 30, "Description");
    mvwhline(projects_window, 9, 0, 0, 200);

    newrequirement_window = newwin(8, 200, 10, 0);
    wbkgd(newrequirement_window, COLOR_PAIR(1));
    mvwprintw(newrequirement_window, 0, 0, "Insert new requirement");
    mvwprintw(newrequirement_window, 1, 2, "Title:");
    mvwprintw(newrequirement_window, 2, 2, "Description:");
    mvwprintw(newrequirement_window, 3, 2, "Level:");
    wattron(newrequirement_window, COLOR_PAIR(4));
    mvwprintw(newrequirement_window, 1, 15, title_value);
    mvwprintw(newrequirement_window, 2, 15, description_value);
    mvwprintw(newrequirement_window, 3, 15, "Project");

    latestrequirements_window = newwin(40, 200, 18, 0);
    wbkgd(latestrequirements_window, COLOR_PAIR(1));
    mvwprintw(latestrequirements_window, 1, 1, "Title");
    mvwprintw(latestrequirements_window, 1, 40, "Description");
    mvwprintw(latestrequirements_window, 1, 180, "Layer");
    wattron(latestrequirements_window, COLOR_PAIR(3));

    footer = newwin(2, 200, 60, 0);
    wbkgd(footer, COLOR_PAIR(6));
    mvwprintw(footer, 0, 2, DEFAULT_FOOTER);

    wrefresh(projects_window);
    wrefresh(newrequirement_window);
    wrefresh(latestrequirements_window);
    wrefresh(footer);
    gui_refresh();
}

void gui_refresh_main(void)
{
    if( g_current_row == 0 ){
        wattron(newrequirement_window, COLOR_PAIR(8));
    }else{
        wattron(newrequirement_window, COLOR_PAIR(4));
    }
    wmove(newrequirement_window, 1, 15);
    wclrtoeol(newrequirement_window);
    if( strlen(title_value) == 0){
        mvwprintw(newrequirement_window, 1, 15, "->");
    }else{
        mvwprintw(newrequirement_window, 1, 15, title_value);
    }

    if( g_current_row == 1 ){
        wattron(newrequirement_window, COLOR_PAIR(8));
    }else{
        wattron(newrequirement_window, COLOR_PAIR(4));
    }
    wmove(newrequirement_window, 2, 15);
    wclrtoeol(newrequirement_window);
    if( strlen(description_value) == 0){
        mvwprintw(newrequirement_window, 2, 15, "->");
    }else{
        mvwprintw(newrequirement_window, 2, 15, description_value);
    }

    wattron(newrequirement_window, COLOR_PAIR(4));
    wmove(newrequirement_window, 3, 15);
    wclrtoeol(newrequirement_window);
    if(level_value == 0){
        mvwprintw(newrequirement_window, 3, 15, "Project");
    }else if(level_value == 1){
        mvwprintw(newrequirement_window, 3, 15, "System");
    }else if(level_value == 2){
        mvwprintw(newrequirement_window, 3, 15, "Software");
    }else{
        mvwprintw(newrequirement_window, 3, 15, "Hardware");
    }

    wclear(footer);
    mvwprintw(footer, 0, 2, DEFAULT_FOOTER);
    wrefresh(footer);
}

void gui_refresh_edit(void)
{
    wclear(footer);
    if( g_current_row == 0){
        mvwprintw(footer, 0, 2, "[TITLE] - Press enter to insert the new value:");
        mvwprintw(footer, 1, 2, title_value);
    }else{
        mvwprintw(footer, 0, 2, "[DESCRIPTION] - Press enter to insert the new value:");
        mvwprintw(footer, 1, 2, description_value);
    }
    wrefresh(footer);
}

void gui_refresh(void)
{
    if( g_state == INIT){
        char title[10][32];
        char description[10][512];
        g_n_projects = databaseconnect_getprojects(title, description, 10);
        for(uint8_t i = 0; i < g_n_projects; i++){
            mvwprintw(projects_window, i + 2, 5, title[i]);
            mvwprintw(projects_window, i + 2, 30, description[i]);
        }
        g_state = MAIN;
        wrefresh(projects_window);
    }else if(g_state == MAIN){
        gui_refresh_main();
    }else{
        gui_refresh_edit();
    }
    {
        char title[10][128];
        char description[10][512];
        uint8_t layer[10];
        uint8_t n_requirements = databaseconnect_getrequirements(title, description, layer, 10);
        for(uint8_t i = 0; i < n_requirements; i++){
            wmove(latestrequirements_window, i + 2, 0);
            wclrtoeol(latestrequirements_window);
            mvwprintw(latestrequirements_window, i + 2, 5, title[i]);
            mvwprintw(latestrequirements_window, i + 2, 5, title[i]);
            mvwprintw(latestrequirements_window, i + 2, 40, description[i]);
            switch(layer[i]){
                case 1:
                    mvwprintw(latestrequirements_window, i + 2, 180, "Project");
                    break;
                case 2:
                    mvwprintw(latestrequirements_window, i + 2, 180, "System");
                    break;
                case 3:
                    mvwprintw(latestrequirements_window, i + 2, 180, "Software");
                    break;
                case 4:
                    mvwprintw(latestrequirements_window, i + 2, 180, "Hardware");
                    break;
            }
        }
        wrefresh(latestrequirements_window);
    }
}

uint8_t gui_getuseraction(void)
{
    char keychar = 0;
    keychar = wgetch(newrequirement_window);
    switch(g_state){
        case MAIN:
            gui_actioninmain(keychar);
            break;
        case EDIT:
            gui_actioninedit(keychar);
            break;
    }
    return keychar == 'q' ? 0 : 1;
}

void gui_close(void)
{
    delwin(projects_window);
    delwin(newrequirement_window);
    delwin(mainwin);
    endwin();
    refresh();
}

void gui_actioninmain(char keychar)
{
    switch(keychar){
        case 'i':
            g_state = EDIT;
            break;
        case 'j':
            g_current_row = g_current_row == 0 ? (N_OF_ROWS - 1) : (g_current_row-1);
            break;
        case 'k':
            g_current_row++;
            if( g_current_row >= N_OF_ROWS ){
                g_current_row = 0;
            }
            break;
        case 't':
            level_value++;
            if(level_value >= N_OF_LEVELS){
                level_value = 0;
            }
            break;
        case 'T':
            if(level_value == 0){
                level_value = N_OF_LEVELS - 1;
            }else{
                level_value--;
            }
            break;
        case 'a':
            databaseconnect_insertrequirement(title_value, description_value, level_value + 1, g_current_project + 1);
            strcpy(title_value, "");
            strcpy(description_value, "");
            break;
    }
}

void gui_actioninedit(keychar)
{
    char *str;
    if( g_current_row == 0){
        str = title_value;
    }else{
        str = description_value;
    }

    if( keychar == '\n' || keychar == '\r' || keychar == KEY_ENTER ){
        g_state = MAIN;
    }else if( keychar == 127 ){
        uint8_t len = strlen(str);
        if( len > 0 ){ str[len-1] = 0; }
    }else{
        uint8_t len = strlen(str);
        str[len] = keychar;
        str[len+1] = 0;
    }
}
 void gui_printmessage(char *message)
{
    WINDOW * popupmessage;
    popupmessage = newwin(10, 100, 20, 50);
    mvwprintw(popupmessage, 0, 2, message);
    wrefresh(popupmessage);
    wgetch(popupmessage);
    delwin(popupmessage);
    touchwin(newrequirement_window);
    touchwin(latestrequirements_window);
    wrefresh(newrequirement_window);
}
