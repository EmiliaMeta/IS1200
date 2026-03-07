/**
 * systems.c

 * Innehåller alla huvudsakliga spelsystem:
 *  - Shop (köp av items)
 *  - Catch (fångst)
 *  - Fight (strider)
 *  - Encounters (slumpmöten)
 * Samt hjälpfunktioner som damage_calc() och try_ball().
 */

#include "systems.h"
#include "textstring.h"
#include "io.h"
#include "random.h"
#include "map.h"
#include "objects.h"  // för CreatureType

CreatureType current_found_type;  // global variabel som håller typ på senaste fångade varelse



// Shop system

// Huvudfunktion för shoppen
void shopsys(Player *player, Item *item) {
    int choice = 0;

    while (choice != 5) {
        text_shop_intro();
        // current_coins(player->coins); // kvar från textstring eller systems
        text_shop_menu();

        choice = io_switch();

        switch (choice) {
            case 1: buy_item(player, &item->bronze_ball, 2, "Bronze ball"); break;
            case 2: buy_item(player, &item->silver_ball, 4, "Silver ball"); break;
            case 3: buy_item(player, &item->gold_ball, 10, "Gold ball"); break;
            case 4: buy_item(player, &item->revive, 6, "Revive"); break;
            case 5: text_exit_shop(player); break;
            default: text_invalid(player); break;
        }
    }
}

// Hjälpfunktion för köp av items
void buy_item(Player *player, int *count, int price, const char *itemName) {
    if (player->coins >= price) {
        player->coins -= price;
        (*count)++;
        text_item_purchased(itemName);
    } else {
        text_no_coins();
    }
}

// Catch system

// Hanterar försök att fånga en creature
void catchsys(Player *player, Item *item) {
    text_found_creature(current_found_type);

    // Kontroll: max 4 creatures
    if (player->creature >= 4) {
        print_and_wait("You already have 4 creatures!", player);
        return;
    }

    // Kontroll: har spelaren några bollar?
    if (item->bronze_ball == 0 && item->silver_ball == 0 && item->gold_ball == 0) {
        text_no_balls();
        return;
    }

    text_choose_ball();

    int attempts = 0;
    int catchRate = 0;

    while (attempts < 3) {
        int choice = io_switch();

        switch (choice) {
            case 1:
                catchRate = try_ball("Bronze ball", &item->bronze_ball, 40);
                break;
            case 2:
                catchRate = try_ball("Silver ball", &item->silver_ball, 65);
                break;
            case 3:
                catchRate = try_ball("Gold ball", &item->gold_ball, 100);
                break;
            default:
                text_invalid(player);
                break;
        }

        if (catchRate > 0) {
            int chance = rand_custom() % 100;
            if (chance < catchRate) {
                player->team[player->creature++] = create_creature(current_found_type, NORMAL);
                text_catched();
                return;
            } else {
                text_escaped();
            }
        }

        attempts++;
    }
}

// Fight system

// Strid mellan spelare och fiende
int fightsys(Player *player, Player *enemy) {
    text_battle_intro();
    led_countdown_fight();

    // Kontroll: har spelaren någon levande creature?
    int alive = 0;
    for (int i = 0; i < player->creature; i++) {
        if (player->team[i].is_alive) alive = 1;
    }

    if (!alive) {
        text_battle_defeat(player);
        enter_room(player, 1);
        return 0;
    }

    Creature *p = NULL;
    Creature *e = &enemy->team[0];

    // Välj creature
    creature_card(player);
    int choice = io_switch();
    if (choice < 1 || choice > player->creature)
        choice = 1;
    p = &player->team[choice - 1];

    hp_display(p, e);

    // Stridsloop
    while (p->is_alive && e->is_alive) {
        text_battle_action_menu();
        int action = io_switch();

        if (get_btn()) {
            int dmg = damage_calc(p, e);
            e->health -= dmg;
            text_player_attack(e->health);
            hp_display(p, e);
        } else {
            switch (action) {
                case 1:
                    shopsys(player, &player->items);
                    break;
                case 2:
                    creature_card(player);
                    choice = io_switch();
                    if (choice >= 1 && choice <= player->creature)
                        p = &player->team[choice - 1];
                    break;
                case 3:
                    catchsys(player, &player->items);
                    break;
                case 4:
                    if (rand_custom() % 100 < 75) {
                        print_and_wait("You escaped the battle!", player);
                        return -1;
                    } else {
                        print_and_wait("You failed to escape!", player);
                    }
                    break;
                default:
                    text_invalid(player);
                    break;
            }
        }

        // Kontrollera om fienden är besegrad
        if (e->health <= 0) {
            e->is_alive = 0;
            text_battle_victory(player);
            player->coins++;
            return 1;
        }

        // Fiendens tur
        int dmg = damage_calc(e, p);
        p->health -= dmg;
        text_enemy_attack(p->health);
        hp_display(p, e);

        if (p->health <= 0) {
            p->is_alive = 0;
            text_battle_defeat(player);
            enter_room(player, 1);
            return 0;
        }
    }

    return 0;
}

// Encounters

// Slumpmässigt möte
void encountersys(Player *player, CreatureRank rank) {
    Player enemy = create_player(0);
    CreatureType t = rand_custom() % 3;
    enemy.team[0] = create_creature(t, rank);
    enemy.creature = 1;

    text_battle_intro();

    int choice;
    do {
        print_and_wait("1. Catch\n2. Fight\n", player);
        choice = io_switch();

        if (choice == 1) {
            catchsys(player, &player->items);
            return;
        } else if (choice == 2) {
            int result = fightsys(player, &enemy);
            if (result == 0) enter_room(player, 1);
            return;
        } else {
            text_invalid(player);
        }
    } while (1);
}

// Möte med specifik typ
void encountersys_type(Player *player, CreatureRank rank, CreatureType type) {
    Player enemy = create_player(0);
    enemy.team[0] = create_creature(type, rank);
    enemy.creature = 1;

    text_battle_intro();

    int choice;
    do {
        print_and_wait("1. Catch\n2. Fight\n", player);
        choice = io_switch();

        if (choice == 1) {
            catchsys(player, &player->items);
            return;
        } else if (choice == 2) {
            int result = fightsys(player, &enemy);
            if (result == 0) enter_room(player, 1);
            return;
        } else {
            text_invalid(player);
        }
    } while (1);
}

// Hjälppredikat

// Beräknar skada baserat på typfördelar
int damage_calc(Creature *attacker, Creature *defender) {
    if ((attacker->type == ROCK && defender->type == SCISSORS) ||
        (attacker->type == SCISSORS && defender->type == PAPER) ||
        (attacker->type == PAPER && defender->type == ROCK))
        return 2;
    return 1;
}

// Skriver ut creature-info (används i fights)
void creature_info(const char *label, Creature *c, Player *player) {
    print(label);
    print("\nType: ");
    print(type_to_string(c->type));
    print("\nRank: ");
    print(rank_to_string(c->rank));
    print("\nHP: ");
    print_dec(c->health);
    print("/");
    print_dec(c->max_health);
    print("\n");
}

// Köplogik för items
void purchase_type(Player *player, int price, int *item_count, const char *itemName) {
    if (player->coins >= price) {
        player->coins -= price;
        (*item_count)++;
        text_item_purchased(itemName);
    } else {
        text_no_coins();
    }
}

// Hanterar kast av bollar och returnerar fångstchans
int try_ball(const char *itemName, int *count, int rate) {
    if (*count > 0) {
        (*count)--;
        text_item_purchased(itemName); // använder text för feedback
        return rate;
    } else {
        text_no_balls();
        return -1;
    }
}
