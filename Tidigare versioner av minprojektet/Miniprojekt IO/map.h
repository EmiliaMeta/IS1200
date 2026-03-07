#ifndef MAP_H
#define MAP_H

#include "objects.h"

// Antal rum
#define NUM_ROOMS 9

// Struktur för ett rum
typedef struct {
    int id;
    const char *description;
} Room;

// Funktioner
void init_map(Room map[]);
void enter_room(Player *player, int room_id);
void move_player(Player *player, Room map[]);
void enter_room(Player *player, int room_id);


#endif
