/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   levelHandler.h
 * Author: silvermoon
 *
 * Created on 3 mars 2016, 11:38
 */


#ifndef LEVELHANDLER_H
#define LEVELHANDLER_H
#include <stdbool.h>
#include <game.h>
#ifdef __cplusplus
extern "C" {
#endif

bool handle_level(game *newLevel);
game parse_level(char *levelFound);

#ifdef __cplusplus
}
#endif

#endif /* LEVELHANDLER_H */

