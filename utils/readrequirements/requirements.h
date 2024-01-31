/**
 * @file requirements.h
 * @brief Requirements header file
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-01-31
 */

extern char g_title[][128];
extern char g_description[][512];
extern uint8_t g_layer[];
extern uint16_t g_n_requirements;


uint8_t requirement_gethardwareall(char title[][128],char description[][512]);
uint8_t requirement_getsoftwareall(char title[][128],char description[][512]);
uint8_t requirement_getsystemall(char title[][128],char description[][512]);
uint8_t requirement_getprojectall(char title[][128],char description[][512]);
