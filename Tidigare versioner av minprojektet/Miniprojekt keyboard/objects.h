#ifndef OBJECTS_H
#define OBJECTS_H

// ================= CREATURE =================
typedef enum {
    ROCK, PAPER, SCISSORS
} CreatureType;

typedef enum {
    NORMAL, MINIBOSS, BOSS
} CreatureRank;

typedef struct {
    int health;
    int max_health;
    CreatureType type;
    CreatureRank rank;
    int is_alive;   // 1 = levande, 0 = död
    int is_active;
} Creature;


// ================= ITEM =================
typedef struct {
    int bronze_ball;   // 25% catch rate
    int silver_ball;   // 50% catch rate
    int gold_ball;     // 100% catch rate
    int revive;        // återställer en död creature
} Item;

// ================= PLAYER =================
typedef struct {
    int creature;       // antal fångade creatures (max 4)
    int coins;          // valuta
    Item items;         // inventory
    Creature team[4];   // lag
} Player;

// ================= FUNKTIONER =================
Creature create_creature(CreatureType type, CreatureRank rank);
Player create_player(int start_coins);

const char* type_to_string(CreatureType type);
const char* rank_to_string(CreatureRank rank);

void creature_card(Player *player);

void creature(Player *player);
#endif

// global från textstring.c
extern CreatureType current_found_type;


