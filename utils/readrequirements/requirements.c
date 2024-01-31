/**
 * @file requirements.c
 * @brief Requirements storage and filter
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-31
 */

#include "stdint.h"

char g_title[1024][128];
char g_description[1024][512];
uint8_t g_layer[1024];
uint16_t g_n_requirements;

uint8_t requirement_gethardwareall(char title[][128],char description[][512])
{
    uint8_t count = 0;
    for(uint16_t i = 0; i < g_n_requirements; i++){
        if( g_layer[i] == 4 ){
            strcpy(title[count], g_title[i]);
            strcpy(description[count], g_description[i]);
            count++;
        }
        if( count >= 10 ){
            break;
        }
    }
    return count;
}

uint8_t requirement_getsoftwareall(char title[][128],char description[][512])
{
    uint8_t count = 0;
    for(uint16_t i = 0; i < g_n_requirements; i++){
        if( g_layer[i] == 3 ){
            strcpy(title[count], g_title[i]);
            strcpy(description[count], g_description[i]);
            count++;
        }
        if( count >= 10 ){
            break;
        }
    }
    return count;
}

uint8_t requirement_getsystemall(char title[][128],char description[][512])
{
    uint8_t count = 0;
    for(uint16_t i = 0; i < g_n_requirements; i++){
        if( g_layer[i] == 2 ){
            strcpy(title[count], g_title[i]);
            strcpy(description[count], g_description[i]);
            count++;
        }
        if( count >= 10 ){
            break;
        }
    }
    return count;
}

uint8_t requirement_getprojectall(char title[][128],char description[][512])
{
    uint8_t count = 0;
    for(uint16_t i = 0; i < g_n_requirements; i++){
        if( g_layer[i] == 1 ){
            strcpy(title[count], g_title[i]);
            strcpy(description[count], g_description[i]);
            count++;
        }
        if( count >= 10 ){
            break;
        }
    }
    return count;
}
