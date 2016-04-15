/* 
 * File:   solv.h
 * Author: silvermoon
 *
 * Created on 8 mars 2016, 11:51
 */


#ifndef SOLV_H
#define SOLV_H
#include "uthash.h"
#include "utlist.h"
#include "game.h"
#ifdef __cplusplus
extern "C" {
#endif
#define SHOWPATH
#define RUSH 1
#define ANE 0
    
//Structure tab
typedef struct tabGame {
    game * tab;
    int nbElem;
} tabGame;
    
//Hash table
typedef struct hashTableChrInt {
    char* key; /* key (string is WITHIN the structure) */
    UT_hash_handle hh; /* makes this structure hashable */
} hashTableChrInt;

//List
typedef struct gameStruct {
    game current;
    char *move;
    struct gameStruct *prev; /* needed for a doubly-linked list only */
    struct gameStruct *next; /* needed for singly- or doubly-linked lists */
} gameStruct;
gameStruct *solv(cgame newGame,int game);


#ifdef __cplusplus
}
#endif

#endif /* SOLV_H */

