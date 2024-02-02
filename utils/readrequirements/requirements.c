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

#include "requirements_pri.h"

requirement_st requirements[MAX_N_REQ];
static uint16_t g_n_total_requirements = 0;
static uint16_t g_offset[] = {0, 0, 0, 0};
static uint16_t g_n_req[] = {0, 0, 0, 0};
uint8_t g_project = 0;

void requirement_init(uint8_t project)
{
    g_project = project + 1;
    g_n_total_requirements = databaseconnect_getrequirements_project( requirements, project + 1, 1024);
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( requirements[i].layer > 0 ){
            g_n_req[requirements[i].layer - 1]++;
        }
    }
}

uint16_t requirement_getoffset(uint8_t layer)
{
    return g_offset[layer];
}

uint16_t requirement_getn(uint8_t layer)
{
    return g_n_req[layer];
}

uint8_t requirement_getrequirement(char title[][REQ_TITLE_LEN], char description[][REQ_DESC_LEN], uint16_t *uid,
    uint8_t layer, uint8_t link)
{
    uint8_t count = 0;
    uint16_t requirement_ref_n = 0;
    uint8_t target_layer;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( link == requirements[i].uid ){
            target_layer = requirements[i].layer;
            requirement_ref_n = i;
        }
    }

    if( link == 0 ){
        uint16_t offset = 0;
        for(uint16_t i = 0; i < g_n_total_requirements; i++){
            if( offset < g_offset[layer - 1]){
                offset++;
                continue;
            }
            if( requirements[i].layer == layer ){
                REQ_COPY_REQUIREMENT( count, i);
            }
            if( count >= MAX_REQUIREMENTS_REQUEST ){
                break;
            }
        }
    }else{
        if( layer == target_layer ){
            count = 0;
            REQ_COPY_REQUIREMENT( count, requirement_ref_n);
        }
        if( target_layer == 1 && layer == 2 ){
            count = requirement_findsystemreqlinkedtoproject( title, description, uid, link );
        }
        if( target_layer == 1 && (layer == 3 || layer == 4) ){
            count = requirement_findbottomreqlinkedtoproject( title, description, uid, link, layer );
        }
        if( target_layer == 2 && layer == 1 ){
            count = requirement_findprojectreqlinkedtosystem( title, description, uid, link);
        }
        if( target_layer == 2 && (layer == 3 || layer == 4) ){
            count = requirement_findbottomreqlinkedtosystem( title, description, uid, link, layer );
        }
        if( (target_layer == 3 || target_layer == 4) && layer == 1){
            count = requirement_findprojectreqlinkedtobottom( title, description, uid, link);
        }
        if( (target_layer == 3 || target_layer == 4) && layer == 2){
            count = requirement_findsystemreqlinkedtobottom( title, description, uid, link);
        }
    }
    return count;
}

void requirement_increaseoffset(uint8_t layer)
{
    if( g_n_req[layer] > MAX_REQUIREMENTS_REQUEST){
        g_offset[layer] = ( g_offset[layer] >= (g_n_req[layer] - MAX_REQUIREMENTS_REQUEST - 1) ) ?
            g_n_req[layer] - MAX_REQUIREMENTS_REQUEST : g_offset[layer] + 1;
    }
}

void requirement_decreaseoffset(uint8_t layer)
{
    g_offset[layer] = ( g_offset[layer] > 0 ) ? g_offset[layer] - 1 : 0;
}

uint8_t requirement_getallrequirements(char title[][REQ_TITLE_LEN], char description[][REQ_DESC_LEN], uint16_t uid[],
    uint8_t layer)
{
    return requirement_getrequirement(title, description, uid, layer, 0);
}

uint8_t requirement_findsystemreqlinkedtoproject(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid, uint16_t ref_uid)
{
    uint8_t count = 0;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( (requirements[i].layer == 2 ) &&
            ( ref_uid == requirements[i].link[0] )){
            REQ_COPY_REQUIREMENT( count, i);
        }
    }
    return count;
}

uint8_t requirement_findbottomreqlinkedtoproject(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid, uint8_t layer)
{
    uint8_t count = 0;
    uint16_t refs[REQ_MAX_LINKS];
    uint8_t nrefsfound = 0;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( ref_uid == requirements[i].link[0] ){
            refs[nrefsfound] = requirements[i].uid;
            nrefsfound++;
            if(nrefsfound == REQ_MAX_LINKS){
                break;
            }
        }
    }
    for(uint16_t j = 0; j < nrefsfound; j++){
        for(uint16_t i = 0; i < g_n_total_requirements; i++){
            if( (requirements[i].layer == layer ) &&
                ( refs[j] == requirements[i].link[0] )){
                REQ_COPY_REQUIREMENT( count, i);
            }
        }
    }
    return count;
}

uint8_t requirement_findprojectreqlinkedtosystem(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid )
{
    uint8_t count = 0;
    uint16_t project_id;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( ref_uid == requirements[i].uid ){
            project_id = requirements[i].link[0];
        }
    }
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( project_id == requirements[i].uid ){
            REQ_COPY_REQUIREMENT( count, i);
        }
    }
    return count;
}

uint8_t requirement_findprojectreqlinkedtobottom(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid)
{
    uint8_t count = 0;
    uint16_t project_id;
    uint16_t system_id;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( ref_uid == requirements[i].uid ){
            system_id = requirements[i].link[0];
        }
    }
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( system_id == requirements[i].uid ){
            project_id = requirements[i].link[0];
        }
    }
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( project_id == requirements[i].uid ){
            REQ_COPY_REQUIREMENT( count, i);
        }
    }
    return count;
}

uint8_t requirement_findsystemreqlinkedtobottom(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid)
{
    uint8_t count = 0;
    uint16_t system_id;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( ref_uid == requirements[i].uid ){
            system_id = requirements[i].link[0];
        }
    }
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( system_id == requirements[i].uid ){
            REQ_COPY_REQUIREMENT( count, i);
        }
    }
    return count;
}

uint8_t requirement_findbottomreqlinkedtosystem(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid, uint8_t layer)
{
    uint8_t count = 0;
    uint16_t refs[REQ_MAX_LINKS];
    uint8_t nrefsfound = 0;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( ref_uid == requirements[i].link[0] ){
            refs[nrefsfound] = requirements[i].uid;
            nrefsfound++;
            if(nrefsfound == REQ_MAX_LINKS){
                break;
            }
        }
    }
    for(uint16_t j = 0; j < nrefsfound; j++){
        for(uint16_t i = 0; i < g_n_total_requirements; i++){
            if( (requirements[i].layer == layer ) &&
                ( refs[j] == requirements[i].uid )){
                REQ_COPY_REQUIREMENT( count, i);
            }
        }
    }
    return count;
}

requirement_st requirement_getdetailedinformation(uint16_t uid)
{
    requirement_st result;
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( uid == requirements[i].uid ){
            result = requirements[i];
        }
    }
    return result;
}

void requirement_link(uint16_t link, uint16_t uid)
{
    databaseconnect_link(link, uid);
    for(uint16_t i = 0; i < g_n_total_requirements; i++){
        if( uid == requirements[i].uid ){
            requirements[i].link[0] = link;
        }
    }
}

void requirement_newrequirement( char title[], char description[], uint8_t layer)
{
    uint16_t uid = databaseconnect_newrequirement(title, description, layer, g_project);
    requirements[g_n_total_requirements].uid = uid;
    strcpy( requirements[g_n_total_requirements].title, title);
    strcpy( requirements[g_n_total_requirements].description, description);
    requirements[g_n_total_requirements].layer = layer;
    requirements[g_n_total_requirements].link[0] = 0;
    g_n_total_requirements++;
    g_n_req[layer -1]++;
}
