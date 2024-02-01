/**
 * @file gui_ncurses.h
 * @brief Print menus and get user actions
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-30
 */

#ifndef GUI_NCURSES_H
#define GUI_NCURSES_H

#define N_REQ_IN_A_WINDOW 10u
#define N_LAYERS 4u

uint8_t gui_printlayout(void);
void gui_close(void);
void gui_refresh(void);
uint8_t gui_getuseraction(void);
void gui_printmessage(char *message);

typedef enum guimainstate_e{
    INIT,
    MAIN
} guimainstate_et;

#endif
