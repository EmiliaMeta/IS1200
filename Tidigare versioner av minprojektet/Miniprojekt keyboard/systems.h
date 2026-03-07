#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "objects.h"

// Deklarationer för alla system

void shopsys(Player *player, Item *item);
void buy_item(Player *player, int *count, int price, const char *itemName);


void catchsys(Player *player, Item *item);

int fightsys(Player *player, Player *enemy);

void encountersys(Player *player, CreatureRank rank);
void encountersys_type(Player *player, CreatureRank rank, CreatureType type);

#endif
