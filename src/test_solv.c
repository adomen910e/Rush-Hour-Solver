#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "solvTest.h"
#include "game.h"

tabGame* t2 = NULL;
hashTableChrInt *s2, *tempH2, *hTable2 = NULL;
/**
 * @brief test if value is equal to expected; if not, displays an error message containing msg to standard error output 
 * @return true if expected==value
 */
bool test_equality_int(int expected, int value, char * msg)
{
    if (expected != value)
        fprintf(stderr, "ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
    return expected == value;
}

bool test_equality_adress(void *expected, void *value, char * msg){
    if (expected != value)
        fprintf(stderr, "ERR: value expected %p ; value computed %p. %s\n", expected, value, msg);
    return expected == value;
}
/**
 * @brief test if value is equal to expected; if not, displays an error message containing msg to standard error output 
 */
bool test_equality_bool(bool expected, bool value, char * msg)
{
    if (expected != value)
        fprintf(stderr, "ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
    return expected == value;
}

bool test_equality_string(char *expected, char *value, char * msg)
{
    int result = strcmp(expected,value);
    if (result != 0)
        fprintf(stderr, "ERR: value expected %s ; value computed %s. %s\n", expected, value, msg);
    return !result;
}

bool test_solv()
{
    bool result = true;
    piece pieces[10];
    pieces[0] = new_piece_rh(3, 3, true, true);
    pieces[1] = new_piece_rh(3, 0, true, false);
    pieces[2] = new_piece_rh(4, 1, true, true);
    pieces[3] = new_piece_rh(5, 3, false, false);
    game newGame = new_game(6, 6, 4, pieces);
    gameStruct *resultSolv = solv(newGame,RUSH);
    result = result && test_equality_int(game_nb_moves(resultSolv->current),13,"Error on nb of move");
    result = result && test_equality_bool(game_over_hr(resultSolv->current),true,"Game is not over");
    delete_game(newGame);
    free(resultSolv);
    newGame = new_game(6, 6, 4, pieces);
    resultSolv = solv(newGame,-1);
    result = result && test_equality_int(game_nb_moves(resultSolv->current),13,"Error on nb of move");
    result = result && test_equality_bool(game_over_hr(resultSolv->current),true,"Game is not over");
    
    for(int i=0;i<game_nb_pieces(newGame);i++){
        delete_piece(pieces[i]);
    }
    delete_game(newGame);
    free(resultSolv);
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
    resultSolv = solv(newGame,ANE);
    result = result && test_equality_int(game_nb_moves(resultSolv->current),118,"Error on nb of move");
    result = result && test_equality_bool(game_over_an(resultSolv->current),true,"Game is not over");
    
    for(int i=0;i<game_nb_pieces(newGame);i++){
        delete_piece(pieces[i]);
    }
    delete_game(newGame);
    free(resultSolv);
    return result;

}

bool test_convertGame()
{
    bool result = true;
    piece pieces[2];
    pieces[0] = new_piece(3, 3, 1, 2, true, true); // 2 
    pieces[1] = new_piece(1, 3, 2, 2, true, true); //Rouge
    game newGame = new_game(6,6,2,pieces);
    
    game final = convertGame(newGame);
    
    result = result && test_equality_int(1,get_x(game_piece(final,0)),"Failed to serialize game");
    result = result && test_equality_int(3,get_y(game_piece(final,0)),"Failed to serialize game");
    result = result && test_equality_int(3,get_x(game_piece(final,1)),"Failed to serialize game");
    result = result && test_equality_int(3,get_y(game_piece(final,1)),"Failed to serialize game");
    delete_piece(pieces[0]);
    delete_piece(pieces[1]);
    delete_game(newGame);
    return result;
}

bool test_check_found_else_create()
{
    bool result = true;
    piece pieces[2];
    pieces[0] = new_piece(3, 3, 1, 2, true, true); // 2
    pieces[1] = new_piece(1, 3, 2, 2, true, true); //Rouge
    game newGame = new_game(6,6,2,pieces);
    result = result && test_equality_adress(NULL,t2,"Game found this is an error");
    create_tab(10);
    result = result && test_equality_bool(false,check_found_else_create(newGame),"Game found game this is an error");
    result = result && test_equality_bool(true,check_found_else_create(newGame),"Game not found game this is an error");
    delete_piece(pieces[0]);
    delete_piece(pieces[1]);
    delete_game(newGame);
    delete_tab();

    
    return result;
}

int main(int argc, char *argv[])
{
    bool result = true;
    result = result && test_equality_bool(true, test_convertGame(), "test_convertGame");
    result = result && test_equality_bool(true, test_check_found_else_create(), "test_check_found_else_create");
    //result = result && test_equality_bool(true, test_solv(), "test_solv");
    if (result) {
        printf("Youpi !\n");
        return EXIT_SUCCESS;
    } else
        return EXIT_FAILURE;
}