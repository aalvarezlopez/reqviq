/**
 * @file databaseconnect.c
 * @brief Connect to mysql api, get queries and insert values
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-30
 */
#include "stdio.h"
#include "stdint.h"

#include "mysql.h"
#include "requirements.h"
#include "databaseconnect.h"

MYSQL* mysql;

uint8_t databaseconnect_start(void)
{
    mysql_library_init(0, NULL, NULL);
    mysql = mysql_init(NULL);
    mysql = mysql_real_connect(mysql, G_MYSQL_HOST, G_MYSQL_USER, G_MYSQL_PASS, "requirements",
        3306, NULL, CLIENT_FOUND_ROWS);
    return (mysql == NULL ? 1 : 0);
}

uint8_t databaseconnect_getprojects(char title[][32], char description[][512], uint8_t maxn)
{
    MYSQL_ROW row;
    uint8_t j = 0;
    int result, resultquery, ncolumns, num_fields;
    MYSQL_RES * mysql_result;
    resultquery = mysql_query(mysql, "SELECT * from project");
    mysql_result = mysql_store_result(mysql);
    if (mysql_result)
    {
        num_fields = mysql_num_fields(mysql_result);
        while ((row = mysql_fetch_row(mysql_result)))
        {
            unsigned long *lengths;
            lengths = mysql_fetch_lengths(mysql_result);
            if( num_fields >= 3){
                strcpy(title[j], row[1]);
                strcpy(description[j], row[2]);
            }
            j++;
            if( j > maxn){
                break;
            }
        }
        mysql_free_result(mysql_result);
    }
    return j;
}

uint16_t databaseconnect_getrequirements_project(requirement_st *requirements, uint8_t project, uint16_t maxn)
{
    MYSQL_ROW row;
    uint8_t j = 0;
    char query[512];
    int result, resultquery, ncolumns, num_fields;
    MYSQL_RES * mysql_result;
    sprintf(query, "SELECT * from requirement where project_ref=\"%d\" ORDER BY "\
        "modified DESC LIMIT %d", project, maxn);
    resultquery = mysql_query(mysql, query);
    mysql_result = mysql_store_result(mysql);
    if (mysql_result)
    {
        num_fields = mysql_num_fields(mysql_result);
        while ((row = mysql_fetch_row(mysql_result)))
        {
            unsigned long *lengths;
            lengths = mysql_fetch_lengths(mysql_result);
            if( num_fields >= 3){
                strcpy(requirements[j].title, row[2]);
                strcpy(requirements[j].description, row[3]);
                requirements[j].layer = atoi(row[5]);
                requirements[j].uid = atoi(row[0]);
                if( row[1] != NULL){
                    requirements[j].link[0] = atoi(row[1]);
                }
                requirements[j].n_links = 1;
            }
            j++;
            if( j > maxn){
                break;
            }
        }
        mysql_free_result(mysql_result);
    }
    return j;
}

void databaseconnect_insertrequirement(char *title, char *description, uint8_t layer,
    uint8_t project)
{
    char query[512];
    int resultquery;
    sprintf(query, "INSERT INTO requirement (title, description, project_ref,"\
        " layer, version) values (\"%s\", \"%s\", %d, %d, 1)", title, description, project, layer);
    resultquery = mysql_query(mysql, query);
}

void databaseconnect_link(uint16_t link, uint16_t uid)
{
    char query[512];
    int resultquery;
    sprintf(query, "UPDATE requirement set linkup=%d where req_uid=%d", link, uid);
    gui_printmessage(query);
    resultquery = mysql_query(mysql, query);
}

uint16_t databaseconnect_newrequirement( char title[], char description[], uint8_t layer, uint8_t project)
{
    char query[512];
    int resultquery;
    MYSQL_ROW row;
    MYSQL_RES * mysql_result;
    sprintf(query, "INSERT INTO requirement (title, description, project_ref,"\
        " layer, version) values (\"%s\", \"%s\", %d, %d, 1)", title, description, project, layer);
    gui_printmessage(query);
    resultquery = mysql_query(mysql, query);

    sprintf(query, "SELECT LAST_INSERT_ID()");
    resultquery = mysql_query(mysql, query);
    mysql_result = mysql_store_result(mysql);
    if (mysql_result)
    {
        row = mysql_fetch_row(mysql_result);
        mysql_free_result(mysql_result);
    }
    return atoi(row[0]);
}

void databaseconnect_close(void)
{
    mysql_close(mysql);
    mysql_library_end();
}

