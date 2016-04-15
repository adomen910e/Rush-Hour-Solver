/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   solvTest.h
 * Author: norips
 *
 * Created on 29 mars 2016, 10:52
 */

#ifndef SOLVTEST_H
#define SOLVTEST_H
#include "solv.h"
#ifdef __cplusplus
extern "C" {
#endif

void create_tab(int n);
void delete_tab();
bool game_over_an(cgame newGame);
char* serialize(cpiece *arrPieces, int n);
bool sup(cpiece p1, cpiece p2);
game convertGame(game newGame);
bool check_found_else_create(game newGame);
void save_or_continue(game tmpGame,gameStruct *new,int nbPiece,int nb_move,char* direction);
void explore(gameStruct *new);
gameStruct *solv(cgame newGame, int gameType);

#ifdef __cplusplus
}
#endif

#endif /* SOLVTEST_H */

