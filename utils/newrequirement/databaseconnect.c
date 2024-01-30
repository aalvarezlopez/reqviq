/**
 * @file databaseconnect.c
 * @brief Connect to mysql api, get queries and insert values
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-30
 */

#include "mysql.h"
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

void databaseconnect_close(void)
{
    mysql_close(mysql);
    mysql_library_end();
}
