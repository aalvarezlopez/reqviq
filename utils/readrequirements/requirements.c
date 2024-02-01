/**
 * @file requirements.c
 * @brief Requirements storage and filter
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-31
 */

#define REQ_MAIN_FILE
#include "stdio.h"
#include "stdint.h"
#include "requirements.h"
#include "databaseconnect.h"

requirement_st requirements[MAX_N_REQ];
static uint16_t g_n_total_requirements = 0;

void requirement_init(uint8_t project)
{
    g_n_total_requirements = databaseconnect_getrequirements_project( requirements, project + 1, 1024);
}

uint8_t requirement_getrequirement(char title[][REQ_TITLE_LEN], char description[][REQ_DESC_LEN], uint16_t *uid,
    uint8_t layer, uint8_t link)
{
    uint8_t count = 0;
    uint16_t reference = 0;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( requirements[i].layer == layer ){
            reference = requirements[i].uid;
            if( link == 0 || link == reference ){
                strcpy(title[count], requirements[i].title);
                strcpy(description[count], requirements[i].description);
                if( uid != NULL){
                    uid[count] = requirements[i].uid;
                }
                count++;
            }
        }
        if( count >= MAX_REQUIREMENTS_REQUEST ){
            break;
        }
    }
    return count;
}

uint8_t requirement_getallrequirements(char title[][REQ_TITLE_LEN], char description[][REQ_DESC_LEN], uint16_t uid[],
    uint8_t layer)
{
    return requirement_getrequirement(title, description, uid, layer, 0);
}

