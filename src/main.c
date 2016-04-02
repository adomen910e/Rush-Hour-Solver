/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: silvermoon
 *
 * Created on 15 mars 2016, 18:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "solv.h"
#include "levelHandler.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    bool defaut = false;
    //Ane rouge
    if (argc < 3) {
        fprintf(stderr, "%s : NumberOfLoop Game Path_to_lvl\n Game : \t1 Rush Hour\n\t\t0 Ane Rouge\n", argv[0]);
        return EXIT_FAILURE;
    }
    int nbLoop = atoi(argv[1]);
    nbLoop = abs(nbLoop);
    if (nbLoop == 0) {
        fprintf(stderr, "%s : NUMBEROfLoop Game Path_to_lvl\n Game : \t1 Rush Hour\n\t\t0 Ane Rouge\n", argv[0]);
        return EXIT_FAILURE;
    }
    int gameType = atoi(argv[2]);
    gameType = abs(gameType);
    if (gameType > 1) {
        fprintf(stderr, "%s : NUMBEROfLoop Game Path_to_lvl\n Game : \t1 Rush Hour\n\t\t0 Ane Rouge\n", argv[0]);
        return EXIT_FAILURE;
    }
    game newGame = NULL;
    piece pieces[10];
    if(argc == 4) {
        printf("Load level\n");
        newGame = parse_level(argv[3]);
    }
    if(!newGame) {
        defaut=true;
        printf("Load level failed\n");
        pieces[0] = new_piece(1, 3, 2, 2, true, true); //Rouge
        pieces[1] = new_piece(3, 3, 1, 2, true, true); // 2
        pieces[2] = new_piece(3, 1, 1, 2, true, true); // 3
        pieces[3] = new_piece(3, 0, 1, 1, true, true); // 4
        pieces[4] = new_piece(1, 2, 2, 1, true, true); // 5
        pieces[5] = new_piece(2, 1, 1, 1, true, true); // 6
        pieces[6] = new_piece(1, 1, 1, 1, true, true); // 7
        pieces[7] = new_piece(0, 0, 1, 1, true, true); // 8
        pieces[8] = new_piece(0, 1, 1, 2, true, true); // 9
        pieces[9] = new_piece(0, 3, 1, 2, true, true); // 10
        newGame = new_game(4, 5, 10, pieces);
    }
    gameStruct *result=NULL;
    
    
    for (int i = 0; i < nbLoop; i++) {
        result = solv(newGame,gameType);
        if(!result){
            printf("Unable to found a solution !\n");
            delete_game(newGame);
            return EXIT_FAILURE;
        }
	printf("Found in %d moves\n",game_nb_moves(result->current));
#ifdef SHOWPATH
	printf("Move : %s",result->move);
        free(result->move);
#endif
        delete_game(result->current);
        free(result);
    }
    
    if(defaut) {
        for(int i=0;i<game_nb_pieces(newGame);i++)
            delete_piece(pieces[i]);
    }
    delete_game(newGame);
    return (EXIT_SUCCESS);
}

