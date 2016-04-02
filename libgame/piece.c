#include "piece.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static const int smallSize = 2;
static const int bigSize = 3;
static const int thickness = 1;

struct position {
    int x; // (x;y) est le point de la piece en bas a gauche
    int y;
};

struct piece_s {
    struct position position;
    int width;
    int height;
    bool move_x;
    bool move_y;

};

static int failure(char *msg)
{
    printf("Error : %s\n", msg);
    return (EXIT_FAILURE);
}

piece new_piece_rh(int x, int y, bool small, bool horizontal)
{
    piece newp = NULL;
    if (small) {
        if (horizontal) {
            newp = new_piece(x, y, smallSize, thickness, horizontal, !horizontal);
        } else {
            newp = new_piece(x, y, thickness, smallSize, horizontal, !horizontal);
        }
    } else {
        if (horizontal) {
            newp = new_piece(x, y, bigSize, thickness, horizontal, !horizontal);
        } else {
            newp = new_piece(x, y, thickness, bigSize, horizontal, !horizontal);
        }
    }
    if (!newp) {
        failure("new_piece_rh alloc newp");
        return NULL;
    }
    return newp;
}

void delete_piece(piece p)
{
    free(p);
}

void copy_piece(cpiece src, piece dst)
{
    if (!src || !dst) {
        failure("copy_piece src or dst are NULL");
        return;
    }

    dst->position.x = src->position.x;
    dst->position.y = src->position.y;
    dst-> width = src->width;
    dst->height = src->height;
    dst->move_x = src->move_x;
    dst-> move_y = src->move_y;

}

void move_piece(piece p, dir d, int distance)
{
    if (!p) {
        failure("new_piece_rh p is NULL");
        return;
    }

    if (can_move_x(p) && !can_move_y(p)) {
        if (d == LEFT)
            p->position.x -= distance;
        if (d == RIGHT)
            p->position.x += distance;
    } else if (can_move_y(p) && !can_move_x(p)) {
        if (d == UP)
            p->position.y += distance;
        if (d == DOWN)
            p->position.y -= distance;
    } else if (can_move_y(p) && can_move_x(p)) {
        if (d == LEFT)
            p->position.x -= distance;
        if (d == RIGHT)
            p->position.x += distance;
        if (d == UP)
            p->position.y += distance;
        if (d == DOWN)
            p->position.y -= distance;
    }
}

bool intersect(cpiece p1, cpiece p2)
{
    if (!p1 || !p2) {
        failure("intersect p1 or p2 are NULL");
        return false;
    }

    if (get_x(p1) < get_x(p2) + get_width(p2) &&
            get_x(p1) + get_width(p1) > get_x(p2) &&
            get_y(p1) < get_y(p2) + get_height(p2) &&
            get_height(p1) + get_y(p1) > get_y(p2)) {
        return true;
    }
    return false;
}

int get_x(cpiece p)
{
    if (!p) {
        failure("get_x p is NULL");
        return -1;
    }
    return p->position.x;
}

int get_y(cpiece p)
{
    if (!p) {
        failure("get_y p is NULL");
        return -1;
    }
    return p->position.y;
}

int get_height(cpiece p)
{
    if (!p) {
        failure("get_height p is NULL");
        return -1;
    }
    return p->height;
}

int get_width(cpiece p)
{
    if (!p) {
        failure("get_width p is NULL");
        return -1;
    }
    return p->width;
}

bool is_horizontal(cpiece p)
{
    if (!p) {
        failure("is_horizontal p is NULL");
        return false;
    }
    return (get_height(p) < get_width(p)); //si la hauteur est inferieur a la largeur alors la piece est horizontal
}

bool can_move_x(cpiece p)
{
    if (!p) {
        failure("can_move_x p is NULL");
        return false;
    }
    return p->move_x;
}

bool can_move_y(cpiece p)
{
    if (!p) {
        failure("can_move_y p is NULL");
        return false;
    }
    return p->move_y;
}

piece new_piece(int x, int y, int width, int height, bool move_x, bool move_y)
{
    piece newp = malloc(sizeof (struct piece_s));

    if (!newp) {
        failure("new_piece alloc newp");
        return NULL;
    }

    newp->position.x = x;
    newp->position.y = y;
    newp->width = width;
    newp->height = height;
    newp->move_x = move_x;
    newp->move_y = move_y;
    return newp;
}

