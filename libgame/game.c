#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

struct game_s {
    int nbMove;
    int nbPieces;
    piece *arrPieces;
    int height;
    int width;
};

static void failure(char *msg)
{
    fprintf(stderr, "Error : %s", msg);
    exit(EXIT_FAILURE);
}

game new_game(int width, int height, int nb_pieces, piece *pieces)
{
    game newGame = malloc(sizeof (struct game_s));
    if (!newGame)
        failure("new_game_hr alloc newGame");
    newGame->nbMove = 0;
    newGame->nbPieces = nb_pieces;
    newGame->height = height;
    newGame->width = width;
    newGame->arrPieces = malloc(sizeof (piece) * nb_pieces);
    if (!newGame->arrPieces)
        failure("new_game_hr alloc arrPieces");
    for (int i = 0; i < nb_pieces; i++) {
        newGame->arrPieces[i] = new_piece_rh(0, 0, false, false); //Create new piece
        copy_piece(pieces[i], newGame->arrPieces[i]); //Overwrite new piece with the copy
    }
    return newGame;

}

game new_game_hr(int nb_pieces, piece *pieces)
{
    game newGame = malloc(sizeof (struct game_s));
    if (!newGame)
        failure("new_game_hr alloc newGame");
    newGame->nbMove = 0;
    newGame->nbPieces = nb_pieces;
    newGame->height = 6;
    newGame->width = 6;
    newGame->arrPieces = malloc(sizeof (piece) * nb_pieces);
    if (!newGame->arrPieces)
        failure("new_game_hr alloc arrPieces");
    for (int i = 0; i < nb_pieces; i++) {
        newGame->arrPieces[i] = new_piece_rh(0, 0, false, false); //Create new piece
        copy_piece(pieces[i], newGame->arrPieces[i]); //Overwrite new piece with the copy
    }
    return newGame;
}

void delete_game(game g)
{
    for (int i = 0; i < g->nbPieces; i++)
        delete_piece(g->arrPieces[i]);
    free(g->arrPieces);
    free(g);
}

void copy_game(cgame src, game dst)
{
    for (int i = 0; i < dst->nbPieces; i++)
        delete_piece(dst->arrPieces[i]);
    free(dst->arrPieces);
    *dst = *src;
    dst->arrPieces = malloc(sizeof (piece) * src->nbPieces);
    if (!dst->arrPieces)
        failure("copy_game alloc arrPieces");
    for (int i = 0; i < src->nbPieces; i++) {
        dst->arrPieces[i] = new_piece_rh(0, 0, false, false); //Create new piece
        copy_piece(src->arrPieces[i], dst->arrPieces[i]); //Overwrite new piece with the copy
    }
}

int game_nb_pieces(cgame g)
{
    if (!g)
        failure("game_nb_pieces g is NULL");
    return g->nbPieces;
}

cpiece game_piece(cgame g, int piece_num)
{
    if (!g)
        failure("game_piece g is NULL");
    return g->arrPieces[piece_num];
}

bool game_over_hr(cgame g)
{
    if (!g)
        failure("game_over_hr g is NULL");
    return get_x(g->arrPieces[0]) == g->width - 2 && get_y(g->arrPieces[0]) == 3;
}

bool play_move(game g, int piece_num, dir d, int distance)
{
    if (!g)
        failure("play_move g is NULL");
    piece tmp = new_piece_rh(0, 0, false, false);
    if (!tmp)
        failure("play_move alloc tmp");
    copy_piece(game_piece(g, piece_num), tmp);
    move_piece(tmp, d, distance);
    if (get_x(tmp) == get_x(game_piece(g, piece_num)) && get_y(tmp) == get_y(game_piece(g, piece_num))) {
        delete_piece(tmp);
        return false;
    }
    if (d == LEFT || d == RIGHT) {
        if ((get_x(tmp) + get_width(tmp)) > game_width(g) || get_x(tmp) < 0) {
            delete_piece(tmp);
            return false;
        }
    } else {
        if ((get_y(tmp) + get_height(tmp)) > game_height(g) || get_y(tmp) < 0) {
            delete_piece(tmp);
            return false;
        }
    }
    for (int i = 0; i < game_nb_pieces(g); i++) {
        if (i == piece_num)
            continue;
        if (intersect(tmp, game_piece(g, i))) {
            delete_piece(tmp);
            return false;
        }
    }
    delete_piece(g->arrPieces[piece_num]);
    g->arrPieces[piece_num] = tmp;
    g->nbMove += distance;
    return true;
}

int game_nb_moves(cgame g)
{
    if (!g)
        failure("game_nb_moves g is NULL");
    return g->nbMove;
}

int game_width(cgame g)
{
    return g->width;
}

int game_height(cgame g)
{
    return g->height;
}

int game_square_piece(game g, int x, int y)
{
    int nb_pieces = 0;
    for (int i = 0; i < game_nb_pieces(g); i++)
        if (get_x(game_piece(g, i)) >= x && get_y(game_piece(g, i)) >= y)
            nb_pieces++;
    return nb_pieces;
}