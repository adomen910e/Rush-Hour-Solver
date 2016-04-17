/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: norips
 *
 * Created on 8 mars 2016, 11:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "solv.h"
#include "uthash.h"
#include "utlist.h"


tabGame *t = NULL;

//hashTableChrInt *s, *tempH, *hTable = NULL;


gameStruct *hList = NULL;

typedef bool(*game_over_func)(cgame); /*Pointer to function for game over*/

game_over_func game_over;


void create_tab(int n)
{
    t = malloc (sizeof(struct tabGame) * 1);
    t->tab = malloc (sizeof(game) * n);
    t->physicalSize = n;
    t->nbElem = 0;
}

void delete_tab()
{
    free(t->tab);
    free(t);
}

static void addNewConfiguration(game convertedGame)
{
  if(t->nbElem+1 == t->physicalSize)
    {
      t->tab = realloc (t->tab, t->physicalSize*2);
      t->physicalSize *=2;
    }

  t->tab[t->nbElem++] = convertedGame;
}


static gameStruct *tear_down(gameStruct *new){
  delete_tab();

  gameStruct *result;
  result = malloc(sizeof (gameStruct));
  game tempGame = new_game_hr(0, NULL);
  copy_game(new->current, tempGame);
  result->current = tempGame;

#ifdef SHOWPATH
            int len = strlen(new->move);
            result->move = malloc(sizeof (char)*(len + 1));
            strcpy(result->move, new->move);
#endif
            gameStruct *elt, *temp;

            /* now delete each element, use the safe iterator */
            DL_FOREACH_SAFE(hList, elt, temp)
            {
                DL_DELETE(hList, elt);
#ifdef SHOWPATH
                free(elt->move);
#endif
                delete_game(elt->current);
                free(elt);
            }

            free(hList);
            return result;
}

bool game_over_an(cgame newGame)
{
    return get_x(game_piece(newGame, 0)) == 1 && get_y(game_piece(newGame, 0)) == 0;
}

bool sup(cpiece p1, cpiece p2)
{
    if (get_x(p1) > get_x(p2)) {
        return true;
    }
    if (get_x(p1) == get_x(p2))
        if (get_y(p1) > get_y(p2))
            return true;
    return false;

}

game convertGame(game newGame)
{
    int n = game_nb_pieces(newGame);
    piece *arrPieces;
    
    arrPieces = malloc(sizeof (piece) * n);
    for (int i = 0; i < n; i++) {
        arrPieces[i] = new_piece_rh(0,0,0,0);
        copy_piece(game_piece(newGame,i), arrPieces[i]);
    }
    
    //Sort
    for (int i = 0; i < (n - 1); i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sup(arrPieces[j], arrPieces[j + 1])) {
                piece swap = arrPieces[j];
                arrPieces[j] = arrPieces[j + 1];
                arrPieces[j + 1] = swap;
            }
        }
    }
    return new_game(game_width(newGame),game_height(newGame),n,arrPieces);
}


static bool samePiecesInGame(game convertedGame, game storedGame, int pieceIndex)
{
  cpiece p1 = game_piece(storedGame,pieceIndex);
  cpiece p2 = game_piece(convertedGame,pieceIndex);
  
   return memcmp(p1,p2, sizeof(cpiece*)) == 0;
}


static bool sameGameConfiguration(game convertedGame, game storedGame)
{
  int n = game_nb_pieces(storedGame);

  for(int j=0; j < n; ++j)
    if(!samePiecesInGame(convertedGame, storedGame, j))
	return false;

    return true;
}


bool check_found_else_create(game newGame)
{
    game convertedGame = convertGame(newGame);

    for(int i=0; i< t->nbElem; ++i){
      if(sameGameConfiguration(convertedGame, t->tab[i]))
	return true;
    }
        
    addNewConfiguration(convertedGame);
    return false;
}


static void writePath(gameStruct *new,int nbPiece,int nb_move,char* direction,gameStruct *newPath)
{
  char *tmp;
  tmp = malloc(strlen(new->move) * sizeof (char) + 
	       (strlen(" 00 \n") + strlen(direction)) * sizeof (char) * nb_move +
	       1 * sizeof (char));

  if (!tmp || !newPath) 
    printf("Memory allocation error");

  sprintf(tmp, "%s", new->move);

  for (int j = 0; j < nb_move; j++) 
    sprintf(tmp, "%s %d %s\n", tmp, nbPiece,direction);

  newPath->move = tmp;
}

void save_or_continue(game tmpGame,gameStruct *new,int nbPiece,int nb_move,char* direction)
{
    if (check_found_else_create(tmpGame)) 
        return;
    gameStruct *newPath = malloc(sizeof (gameStruct));

#ifdef SHOWPATH
    writePath(new,nbPiece,nb_move,direction, newPath);
#endif

    game tmpGame2 = new_game_hr(0, NULL);

    copy_game(tmpGame, tmpGame2);
    newPath->current = tmpGame2;
    DL_APPEND(hList, newPath);
}


static void movePieceInDirection(gameStruct *new, int pieceNumber, dir d, char *directionName)
{
  game tmpGame = new_game_hr(0, NULL);
  copy_game(new->current, tmpGame);
  int nb_move = 1;

  while (play_move(tmpGame, pieceNumber, d, 1)) {
    save_or_continue(tmpGame,new,pieceNumber,nb_move,directionName);
    nb_move++;
  }

  delete_game(tmpGame);
}


static void explore(gameStruct *new)
{
    for (int i = 0; i < game_nb_pieces(new->current); i++) {
      movePieceInDirection(new,i, LEFT, "LEFT");
      movePieceInDirection(new,i, RIGHT, "RIGHT");
      movePieceInDirection(new,i, UP, "UP");
      movePieceInDirection(new,i, DOWN, "DOWN");
    }
}

static void selectGameType(int gameType)
{
  switch (gameType) {
  case ANE:
    game_over = game_over_an;
    break;
  case RUSH:
    game_over = game_over_hr;
    break;
  default:
    game_over = game_over_hr;
    break;
  }
}

gameStruct *solv(cgame newGame, int gameType)
{

  create_tab(25000);
  gameStruct *new = malloc(sizeof (gameStruct));
  game tmp = new_game_hr(0, NULL);

  copy_game(newGame, tmp);
  new->current = tmp;
  
#ifdef SHOWPATH
    new->move = malloc(sizeof (char)*10);
    new->move[0] = 0;
#endif


    DL_APPEND(hList, new);
    selectGameType(gameType);
    do {
      new = hList;
      if (game_over(new->current)) {
	//printf("Found !\n");
	return tear_down(new); // ok pour le tear_down
      }
      explore(new);
      DL_DELETE(hList, new);

#ifdef SHOWPATH
      free(new->move);
#endif

      delete_game(new->current);
      free(new);

    } while (hList);

    gameStruct *s;
    s = tear_down(new);
    free(s->move);
    delete_game(s->current);
    free(s);
    return NULL;
}
