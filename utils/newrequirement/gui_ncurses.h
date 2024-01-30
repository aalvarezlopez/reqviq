/**
 * @file gui_ncurses.h
 * @brief Print menus and get user actions
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-30
 */

#ifndef GUI_NCURSES_H
#define GUI_NCURSES_H

uint8_t gui_printlayout(void);
void gui_close(void);
uint8_t gui_getuseraction(void);
void gui_refresh(void);

typedef enum guimainstate_e{
    INIT,
    MAIN,
    EDIT
} guimainstate_et;

#endif
