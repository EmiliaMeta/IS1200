//emilia - player, creature, items

#include "objects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "systems.h"
#include "textstring.h"


// ====== Skapa en creature ======
Creature create_creature(CreatureType type, CreatureRank rank) {
    Creature c;
    c.type = type;
    c.rank = rank;
    c.is_alive = 1;

    switch(rank) {
        case NORMAL:   c.max_health = 3; break;
        case MINIBOSS: c.max_health = 5; break;
        case BOSS:     c.max_health = 10; break;
    }

    c.health = c.max_health;
    return c;
}

// ====== Skapa en spelare ======
Player create_player(int start_coins) {
    Player p;
    p.creature = 0;
    p.coins = start_coins;

    p.items.bronze_ball = 0;
    p.items.silver_ball = 0;
    p.items.gold_ball   = 0;
    p.items.revive      = 0;

    for (int i = 0; i < 4; i++) {
        p.team[i].is_alive = 0;
    }

    return p;
}

// ====== creature ======
void creature_card(Player *player){
    printf("\n=== Your creatures ===\n\n");
    printf("Chose a creature:\n");
    for(int i=0;i<player->creature;i++){
        Creature c = player->team[i];
        printf("-> Creature %d:\n   HP: %d/%d\n   Type: %s\n   Rank: %s\n   Alive: %s\n\n",
        i+1,
        c.health, c.max_health,
        type_to_string(c.type),
        rank_to_string(c.rank),
        c.is_alive ? "Yes" : "No");
    }
}

// ====== Slumpar creature ======
void creature(Player *player){
    Player enemy = create_player(0);
    CreatureType t = rand() % 3;        
    enemy.team[0] = create_creature(t, NORMAL);
    enemy.creature = 1;
    fightsys(player, &enemy);
}

// ====== Skriv ut typ som text ======
const char* type_to_string(CreatureType type) {
    switch(type) {
        case ROCK: return "Rock";
        case PAPER: return "Paper";
        case SCISSORS: return "Scissors";
        default: return "Unknown";
    }
}

// ====== Skriv ut rank som text ======
const char* rank_to_string(CreatureRank rank) {
    switch(rank) {
        case NORMAL: return "Normal";
        case MINIBOSS: return "Miniboss";
        case BOSS: return "Boss";
        default: return "Unknown";
    }
}

//PLAYER
/*
Attribut:
    Creatures, max 4
    Coins
    Items
*/

//CREATURES
/*
Attribut:
    Health
        Vanlig creature 3HP
        Miniboss 5HP
        Slutboss 10HP
    Typ(Sten, påse eller sax)
    Vanlig, miniboss, boss.
*/

//Items
/*
Attributes:
    Typ(pokeboll, revive)
        Pokeboll:
            Bronsboll har catchrate 25%
            Silverboll har  catchrate 50%
            Guldboll har catchrate 100%
        Revive:
            Används på död creature
            Återställer 100% Health
*/

