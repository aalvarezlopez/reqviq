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
#include "requirements.h"

WINDOW * mainwin;
WINDOW * projectrequirements;
WINDOW * systemrequirements;
WINDOW * softwarerequirements;
WINDOW * hardwarerequirements;
WINDOW * projectselector;
WINDOW * footer;

guimainstate_et g_state = INIT;

uint8_t g_n_projects = 0;
uint8_t selectedproject = 0;
uint8_t selectedlayer = 0;

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

    projectrequirements = newwin(maxX/3, maxY, 0, 0);
    wbkgd(projectrequirements, COLOR_PAIR(1));
    wattron(projectrequirements, COLOR_PAIR(8));
    mvwprintw(projectrequirements, 0, 0, " + Project Requirements");
    whline(projectrequirements, ' ', maxY);
    wattron(projectrequirements, COLOR_PAIR(4));

    systemrequirements = newwin(maxX/3, maxY, maxX/3 * 1, 0);
    wbkgd(systemrequirements, COLOR_PAIR(1));
    wattron(systemrequirements, COLOR_PAIR(8));
    mvwprintw(systemrequirements, 0, 0, " - System Requirements");
    whline(systemrequirements, ' ', maxY);
    wattron(systemrequirements, COLOR_PAIR(4));

    softwarerequirements = newwin(maxX/3, maxY/2, maxX/3 * 2, 0);
    wbkgd(softwarerequirements, COLOR_PAIR(1));
    wattron(softwarerequirements, COLOR_PAIR(8));
    mvwprintw(softwarerequirements, 0, 0, " - Software Requirements");
    whline(softwarerequirements, ' ', maxY/2);
    mvwvline(softwarerequirements, 0, maxY/2 - 1, 0, maxX/3);
    wattron(softwarerequirements, COLOR_PAIR(4));

    hardwarerequirements = newwin(maxX/3, maxY/2, maxX/3 * 2, maxY/2);
    wbkgd(hardwarerequirements, COLOR_PAIR(1));
    wattron(hardwarerequirements, COLOR_PAIR(8));
    mvwprintw(hardwarerequirements, 0, 0, " - Hardware Requirements");
    whline(hardwarerequirements, ' ', maxY/2);
    wattron(hardwarerequirements, COLOR_PAIR(4));

    footer = newwin(5, maxY, maxX, 0);
    wbkgd(footer, COLOR_PAIR(6));

    projectselector = newwin( 10, 40, maxX/2 - 5, maxY/2 - 20);
    wbkgd(footer, COLOR_PAIR(6));
    mvwprintw(projectselector, 1, 3, "Chose a project:");

    print_projects();

    wrefresh(projectrequirements);
    wrefresh(systemrequirements);
    wrefresh(softwarerequirements);
    wrefresh(hardwarerequirements);
    wrefresh(footer);
    wrefresh(projectselector);
}

void gui_close(void)
{
    delwin(projectrequirements);
    delwin(systemrequirements);
    delwin(softwarerequirements);
    delwin(hardwarerequirements);
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
    touchwin(systemrequirements);
    wrefresh(systemrequirements);
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

void gui_regreshmain(void)
{
    char title[10][128];
    char description[10][512];
    uint16_t n_requirements;
    n_requirements = requirement_getprojectall(title, description);
    for(uint8_t i = 0; i < n_requirements; i++){
        mvwprintw(projectrequirements, i + 2, 0, title[i]);
    }
    n_requirements = requirement_getsystemall(title, description);
    for(uint8_t i = 0; i < n_requirements; i++){
        mvwprintw(systemrequirements, i + 2, 0, title[i]);
    }
    n_requirements = requirement_getsoftwareall(title, description);
    for(uint8_t i = 0; i < n_requirements; i++){
        mvwprintw(softwarerequirements, i + 2, 0, title[i]);
    }
    n_requirements = requirement_gethardwareall(title, description);
    for(uint8_t i = 0; i < n_requirements; i++){
        mvwprintw(hardwarerequirements, i + 2, 0, title[i]);
    }
    wattron(projectrequirements, COLOR_PAIR(8));
    mvwprintw(projectrequirements,  0, 0, selectedlayer == 0 ? " +" : " -");
    mvwprintw(systemrequirements,   0, 0, selectedlayer == 1 ? " +" : " -");
    mvwprintw(softwarerequirements, 0, 0, selectedlayer == 2 ? " +" : " -");
    mvwprintw(hardwarerequirements, 0, 0, selectedlayer == 3 ? " +" : " -");
    wattron(projectrequirements, COLOR_PAIR(4));
    wrefresh(projectrequirements);
    wrefresh(systemrequirements);
    wrefresh(hardwarerequirements);
    wrefresh(softwarerequirements);
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
        touchwin(systemrequirements);
        wrefresh(systemrequirements);
        g_n_requirements = databaseconnect_getrequirements_project( g_title, g_description, g_layer,
            selectedproject + 1, 1024);
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
    }else if(keychar == 'r'){
    }
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
