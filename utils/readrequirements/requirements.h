/**
 * @file requirements.h
 * @brief Requirements header file
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-31
 */

#ifndef REQUIREMENTS_H
#define REQUIREMENTS_H

#include "requirements_cfg.h"

#include "requirements.h"

typedef struct requirement_s{
    char title[REQ_TITLE_LEN];
    char description[REQ_DESC_LEN];
    uint8_t layer;
    uint8_t link[REQ_MAX_LINKS];
    uint8_t uid;
    uint8_t n_links;
}requirement_st;

uint8_t requirement_getallrequirements(char title[][REQ_TITLE_LEN], char description[][REQ_DESC_LEN], uint16_t uid[],
    uint8_t layer);
uint8_t requirement_getrequirement(char title[][REQ_TITLE_LEN], char description[][REQ_DESC_LEN], uint16_t *uid,
    uint8_t layer, uint8_t link);
void requirement_init(uint8_t);
requirement_st requirement_getdetailedinformation(uint16_t uid);
void requirement_link(uint16_t link, uint16_t uid);
void requirement_newrequirement( char title[], char description[], uint8_t layer);

#endif
