/**
 * @file databaseconnect.h
 * @brief Connect to mysql api, get queries and insert values
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-30
 */

#ifndef MYSQLCLIENT_H
#define MYSQLCLIENT_H


uint8_t databaseconnect_start(void);
uint8_t databaseconnect_getprojects(char title[][32],
                                char description[][512], uint8_t maxn);
uint8_t databaseconnect_getrequirements(char title[][128], char description[][512],
    uint8_t layer[], uint8_t maxn);
void databaseconnect_insertrequirement(char *title, char *description, uint8_t layer,
    uint8_t project);
void databaseconnect_close(void);

#endif
