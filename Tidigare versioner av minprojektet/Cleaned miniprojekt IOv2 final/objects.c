/**
 * Innehåller:
 *  - Creature creation
 *  - Player creation
 *  - Helpers for displaying and identifying creatures
 */

#include "objects.h"
#include "menu.h"
#include "systems.h"
#include "textstring.h"

// Skapar en creature baserat på typ och rank
Creature create_creature(CreatureType type, CreatureRank rank) {
    Creature c;
    c.type = type;
    c.rank = rank;
    c.is_alive = 1;
    c.is_active = 0;

    // Sätter HP baserat på rank
    switch (rank) {
        case NORMAL:   c.max_health = 3; break;
        case MINIBOSS: c.max_health = 5; break;
        case BOSS:     c.max_health = 10; break;
        default:       c.max_health = 3; break;
    }

    c.health = c.max_health;
    return c;
}

// Skapar en ny spelare med startmynt
Player create_player(int start_coins) {
    Player p;
    p.creature = 0;
    p.coins = start_coins;

    // Nollställer inventory
    p.items.bronze_ball = 0;
    p.items.silver_ball = 0;
    p.items.gold_ball   = 0;

    // Gör alla team-platser inaktiva
    for (int i = 0; i < 4; i++) {
        p.team[i].is_alive = 0;
        p.team[i].is_active = 0;
    }

    return p;
}

// Helpers

// Skriver ut spelarens alla creatures
void creature_card(Player *player) {
    print("\n=== YOUR CREATURES ===\n");
    for (int i = 0; i < player->creature; i++) {
        Creature *c = &player->team[i];
        print("-> Creature ");
        print_dec(i + 1);
        print("\n   HP: ");
        print_dec(c->health);
        print("/");
        print_dec(c->max_health);
        print("\n   Type: ");
        print(type_to_string(c->type));
        print("\n   Rank: ");
        print(rank_to_string(c->rank));
        print("\n   Alive: ");
        print(c->is_alive ? "Yes\n" : "No\n");
    }
}

// Random creature

// Skapar en slumpmässig creature att möta
void creature(Player *player) {
    Player enemy = create_player(0);
    CreatureType t = rand_custom() % 3;
    enemy.team[0] = create_creature(t, NORMAL);
    enemy.creature = 1;
    fightsys(player, &enemy, 0);
}

// Type konverteringar

// Returnerar creature-typen som text
const char* type_to_string(CreatureType type) {
    switch (type) {
        case ROCK:     return "Rock";
        case PAPER:    return "Paper";
        case SCISSORS: return "Scissors";
        default:       return "Unknown";
    }
}

// Returnerar rank som text
const char* rank_to_string(CreatureRank rank) {
    switch (rank) {
        case NORMAL:   return "Normal";
        case MINIBOSS: return "Miniboss";
        case BOSS:     return "Boss";
        default:       return "Unknown";
    }
}
