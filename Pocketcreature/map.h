#ifndef MAP_H
#define MAP_H

#include "objects.h"

/**
 * map.h
 * Deklarerar spelvärldens rum och spelprogression.
 * Written by Emilia Lindqvist
 */

#define NUM_ROOMS 9

// Struktur för ett rum
typedef struct {
    int id;
    const char *description;
} Room;

// Initierar kartan med alla rum
void init_map(Room map[]);

// Hanterar händelser när spelaren går in i ett rum
void enter_room(Player *player, int room_id);

// Hanterar spelarens förflyttning mellan rum
void move_player(Player *player, Room map[]);

#endif
