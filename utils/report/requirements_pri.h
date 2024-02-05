/**
 * @file requirements_pri.h
 * @brief Requirements private header
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2024-02-02
 */

#ifndef REQUIREMENTS_PRI_H
#define REQUIREMENTS_PRI_H

#define REQ_COPY_REQUIREMENT(index_dst, index_src) do{ \
                strcpy(title[index_dst], requirements[index_src].title); \
                strcpy(description[index_dst], requirements[index_src].description); \
                if( uid != NULL){ \
                    uid[index_dst] = requirements[index_src].uid; \
                } \
                index_dst++; \
                }while(0)

uint8_t requirement_findsystemreqlinkedtoproject(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid );
uint8_t requirement_findbottomreqlinkedtoproject(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid, uint8_t layer);
uint8_t requirement_findbottomreqlinkedtosystem(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid, uint8_t layer);
uint8_t requirement_findprojectreqlinkedtosystem(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid );
uint8_t requirement_findprojectreqlinkedtobottom(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid);
uint8_t requirement_findsystemreqlinkedtobottom(char title[][REQ_TITLE_LEN],
                                                 char description[][REQ_DESC_LEN],
                                                 uint16_t *uid,
                                                 uint16_t ref_uid);
#endif
