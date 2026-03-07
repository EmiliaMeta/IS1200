#ifndef OBJECTS_H
#define OBJECTS_H

#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 * Innehåller alla grundläggande spelobjekt och strukturer:
 *  - Creature
 *  - Item 
 *  - Player 
 *  - Hjälpfunktioner för att skapa och visa dem
 * - added by Emilia Lindqvist
 */


// Alla typer av creatures
typedef enum {
    ROCK,
    PAPER,
    SCISSORS
} CreatureType;

// Alla ranknivåer
typedef enum {
    NORMAL,
    MINIBOSS,
    BOSS
} CreatureRank;

// Struktur för en creature
typedef struct {
    int health;
    int max_health;
    CreatureType type;
    CreatureRank rank;
    int is_alive;    // 1 = levande, 0 = död
    int is_active;   // 1 = vald i strid
} Creature;


// Item

// Struktur för spelarens föremål
typedef struct {
    int bronze_ball;   // 25% catch rate
    int silver_ball;   // 50% catch rate
    int gold_ball;     // 100% catch rate
} Item;


// Player

// Spelarens data
typedef struct {
    int creature;       // antal fångade creatures (max 4)
    int coins;          // antal mynt
    Item items;         // spelarens inventory
    Creature team[4];   // spelarens team (upp till 4 creatures)
} Player;


// Funktioner

// Skapar en ny creature
Creature create_creature(CreatureType type, CreatureRank rank);

// Skapar en ny spelare
Player create_player(int start_coins);

// Visar spelarens alla creatures
void creature_card(Player *player);

// Konverterar typ och rank till sträng
const char* type_to_string(CreatureType type);
const char* rank_to_string(CreatureRank rank);

// Global variabel från textstring.c (används vid fångst)
extern CreatureType current_found_type;

// DTEK-V systemfunktioner (JTAG, delay)
extern void print(const char*);
extern void print_dec(unsigned int);

#endif
