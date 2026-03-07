#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "objects.h"

/**
 * systems.h
 
 * Deklarerar alla huvudsakliga spelsystem:
 *  - Shop (köp av items)
 *  - Catch (fånga creatures)
 *  - Fight (strider)
 *  - Encounters (slumpmöten)
 * Samt hjälpfunktioner för strids- och itemlogik.
 */

// SHop system

// Huvudfunktion för shoppen (köp av items)
void shopsys(Player *player, Item *item);

// Hjälpfunktion för att köpa ett föremål
void buy_item(Player *player, int *count, int price, const char *itemName);

// Catch system

// Försök att fånga en creature (upp till 3 försök)
void catchsys(Player *player, Item *item);

// Fights system

// Startar en strid mellan spelare och fiende
// Returnerar: 1 = vinst, 0 = förlust, -1 = flykt
int fightsys(Player *player, Player *enemy);

// Encounter system

// Slumpmöte med fiende
void encountersys(Player *player, CreatureRank rank);

// Möte med specifik typ av fiende
void encountersys_type(Player *player, CreatureRank rank, CreatureType type);

// Hjälppredikat

// Beräknar skada baserat på typer
int damage_calc(Creature *attacker, Creature *defender);

// Skriver ut info om en creature (typ, HP, rank)
void creature_info(const char *label, Creature *c, Player *player);

// Hanterar köp av föremål (logik)
void purchase_type(Player *player, int price, int *item_count, const char *itemName);

// Försöker kasta en boll och returnerar fångstchans
int try_ball(const char *itemName, int *count, int rate);

#endif // SYSTEMS_H
