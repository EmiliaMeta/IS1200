#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "systems.h"
#include "textstring.h"
#include "io.h"


//============== Shopsys ==============
void shopsys(Player *player, Item *item){
    int choice = 0;

    while(choice != 5){
        // Always print menu at the start of each loop
        shop_intro();
        current_coins(player->coins);
        
        // Print shop menu here
        printf("Would you like to buy an item?\n");
        printf("1. Buy a Bronze ball (2 coins)\n");
        printf("2. Buy a Silver ball (4 coins)\n");
        printf("3. Buy a Gold ball (10 coins)\n");
        printf("4. Buy a Revive (6 coins)\n");
        printf("5. Exit the shop\n");

        io_get_switch_input();
        //choice = get_sw();
        switch(choice){
            case 1:
                buy_item(player, &item->bronze_ball, 2, "Bronze ball");
                break;
            case 2:
                buy_item(player, &item->silver_ball, 4, "Silver ball");
                break;
            case 3:
                buy_item(player, &item->gold_ball, 10, "Gold ball");
                break;
            case 4:
                buy_item(player, &item->revive, 6, "Revive");
                break;
            case 5:
                exit_text(player);
                break;
            default:
                invalid(player);
        }
    }
}

//============= Catchsys ===============
void catchsys(Player *player, Item *item){
    found_creature();

    if(player->creature >= 4){
        printf("Wait! You already have 4 creatures\n");
        return;
    }
    if(item->bronze_ball == 0 && item->silver_ball == 0 && item->gold_ball == 0){
        printf("Oh no you do not have any pokeballs!!\n");
        return;
    }

    option();
    if(item->bronze_ball > 0){
        pokemonballs("Bronze ball", item->bronze_ball);
    }
    if(item->silver_ball > 0){
        pokemonballs("Silver ball", item->silver_ball);
    }
    if(item->gold_ball > 0){
        pokemonballs("Gold ball", item->gold_ball);
    }

    int attempts = 0;
    int catchRate = 0;
    while(attempts < 3){
        //int choice = get_sw();
        int choice = io_get_switch_input();

        if (choice == -1){
            attempts++;
            continue;
        }
        switch (choice){
            case 1:
                catchRate = try_ball("Bronze ball", &item->bronze_ball, 40);
                break;
            case 2:
                catchRate = try_ball("Silver ball", &item->silver_ball, 65);
                break;
            case 3:
                if(item->gold_ball > 0){
                    item->gold_ball--;
                    player->team[player->creature] = create_creature(current_found_type, NORMAL);
                    player->creature++;
                    attempts = 3;
                    catched();
                    return;
                } else {
                    no_pokemonballs("Gold ball");
                    attempts++;
                }
                break;
            default:
                invalid(player);
                catchRate = -1;
                break;
        }
        if(catchRate > 0) break;
        attempts++;
    }

    if(catchRate > 0){
        int randomize = rand() % 100;
        if(randomize < catchRate){
            player->team[player->creature] = create_creature(current_found_type, NORMAL);
            player->creature++;
            catched();
        } else {
            escaped();
        }
    }
}

//============== Fightsys ==============
// return: 1 = win, 0 = lose, -1 = escape
int fightsys(Player *player, Player *creature){
    char buf[128];
    battle_intro();

    if (player->creature == 0 || creature->creature == 0) {
        print_and_wait("You do not have a creature to fight with!", player);
        return 0;
    }

    Creature *p = NULL;
    // === Välj start-creature ===
    int alive_found = 0;
    for (int i = 0; i < player->creature; i++) {
        if (player->team[i].is_alive) {
            alive_found = 1;
            break;
        }
    }

    if (!alive_found) {
        print_and_wait("All of your creatures have fainted... You are sent back to Professor Maple's lab.", player);
        enter_room(player, 1);  // skicka tillbaka till labbet
        return 0;               // defeat
    }

    do {
        creature_card(player);
        //int start_choice = get_sw();
        int start_choice = io_get_switch_input();

        if(start_choice < 1 || start_choice > player->creature) {
            invalid(player);
            continue;
        }

        if (!player->team[start_choice-1].is_alive) {
            print_and_wait("That creature has fainted! Choose another one.", player);
            continue;
        }

        p = &player->team[start_choice-1];
        p->is_active = 1;
        break;

    } while(1);

    Creature *e = &creature->team[0];

    creature_info("Your creature: ", p, player);
    creature_info("Enemy creature: ", e, player);

    while (p->is_alive && e->is_alive) {
        int dmg = 0;
        acion_menu();
        int choice = io_get_switch_input();
        //int choice = get_sw();

        switch (choice){
            case 1: { // Attack
                dmg = damage_calc(p,e);
                e->health -= dmg;
                sprintf(buf, "Player (%s) attacks! Enemy (%s) HP: %d",
                        type_to_string(p->type), type_to_string(e->type), e->health);
                print_and_wait(buf, player);
            } break;

            case 2: { // Switch creature
                int new_choice;
                do {
                    creature_card(player);
                    new_choice = io_get_switch_input(); //new_choice = get_sw();
                } while(new_choice < 1 || new_choice > player->creature ||
                        !player->team[new_choice-1].is_alive);

                Creature *new_p = &player->team[new_choice-1];
                if (new_p == p) {
                    print_and_wait("You are already using this creature!", player);
                } else {
                    p->is_active = 0;
                    new_p->is_active = 1;
                    p = new_p;
                    sprintf(buf, "Switched to:\nType: %s\nRank: %s\nHP: %d/%d!\n\n",
                            type_to_string(p->type),
                            rank_to_string(p->rank), p->health, p->max_health);
                    print_and_wait(buf, player);
                }
            } break;

            case 3: { // Use Item
                printf("\n=== ITEMS ===\n");
                printf("1. Use Revive (%d left)\n", player->items.revive);
                printf("2. Throw Pokeball (Bronze:%d, Silver:%d, Gold:%d)\n",
                       player->items.bronze_ball, player->items.silver_ball, player->items.gold_ball);

                int item_choice = io_get_switch_input(); //int item_choice = get_sw();
                if (item_choice == 1) {
                    if (player->items.revive > 0) {
                        int fainted_count = 0;
                        for (int i = 0; i < player->creature; i++) {
                            if (!player->team[i].is_alive) {
                                printf("%d. %s (Fainted)\n", i+1, type_to_string(player->team[i].type));
                                fainted_count++;
                            }
                        }
                        if (fainted_count == 0) {
                            print_and_wait("All your creatures are alive, no need to use a revive.", player);
                        } else {
                            int revive_choice = io_get_switch_input(); //int revive_choice = get_sw();
                            if (revive_choice >= 1 && revive_choice <= player->creature &&
                                !player->team[revive_choice-1].is_alive) {
                                player->team[revive_choice-1].is_alive = 1;
                                player->team[revive_choice-1].health = player->team[revive_choice-1].max_health;
                                player->items.revive--;
                                print_and_wait("You used a Revive! The creature is back in the fight!", player);
                            } else {
                                print_and_wait("Invalid choice or the creature is not fainted.", player);
                            }
                        }
                    } else {
                        print_and_wait("You don't have any revives left.", player);
                    }
                } else if (item_choice == 2) {
                    catchsys(player, &player->items);
                }
            } break;

            case 4: { // Try to escape
                int chance = rand() % 100;
                if (chance < 75) {
                    print_and_wait("You managed to escape!", player);
                    return -1;  // escape result
                } else {
                    print_and_wait("The enemy prevented you from escaping! You must now fight!", player);
                }
            } break;

            default:
                invalid(player);
                break;
        }

        // === Kontrollera om fienden är besegrad ===
        if (e->health <= 0) {
            e->is_alive = 0;
            print_and_wait("Enemy defeated!!", player);
            player->coins += 1;
            return 1; // victory
        }

        // === Fiendens tur ===
        dmg = damage_calc(e,p);
        p->health -= dmg;
        sprintf(buf, "Enemy attacked: Your %s HP: %d",
                type_to_string(p->type), p->health);
        print_and_wait(buf, player);

        if (p->health <= 0) {
            p->is_alive = 0;
            print_and_wait("Your creature died!", player);

            // kolla om det finns levande kvar
            int alive_found = 0;
            for (int i = 0; i < player->creature; i++) {
                if (player->team[i].is_alive) alive_found = 1;
            }
            if (!alive_found) {
                print_and_wait("All of your creatures have fainted... Game Over!", player);
                enter_room(player, 1);  // skicka tillbaka till labbet
                return 0; // defeat
            } else {
                print_and_wait("Choose another creature to continue the battle!", player);
                int new_choice;
                do {
                    creature_card(player);
                    new_choice = io_get_switch_input(); //new_choice = get_sw();
                } while(new_choice < 1 || new_choice > player->creature ||
                        !player->team[new_choice-1].is_alive);

                p = &player->team[new_choice-1];
                p->is_active = 1;
                sprintf(buf, "You send out a new creature!\nType: %s, HP: %d/%d",
                        type_to_string(p->type), p->health, p->max_health);
                print_and_wait(buf, player);
            }
        }
    }
    return 0; // fallback defeat
}


//============= Encountersys ==============
void encountersys(Player *player, CreatureRank rank) {
    print_and_wait("\n--- ENCOUNTER STARTED ---", player);

    // Skapa en fiende med random typ och vald rank
    Player enemy = create_player(0);
    CreatureType t = rand() % 3;
    enemy.team[0] = create_creature(t, rank);
    enemy.creature = 1;

    int choice;
    do {
        print_and_wait("What do you want to do?\n1. Catch\n2. Fight", player);
        int choice = io_get_switch_input();
        //choice = get_sw();

        switch(choice){
            case 1:
                catchsys(player, &player->items);
                return;
            case 2: {
                int result = fightsys(player, &enemy);
                if (result == 0) { // player lost
                    print_and_wait("You lost the battle... You are sent back to Professor Maple's lab.", player);
                    enter_room(player, 1);  // back to lab for revival
                    return;
                }
                return;
            }

            default:
                invalid(player);
        }
    } while(1);
}

//============= Encountersys med specifik typ ==============
void encountersys_type(Player *player, CreatureRank rank, CreatureType type) {
    print_and_wait("\n--- ENCOUNTER STARTED ---", player);

    // Skapa en fiende med specifik typ och vald rank
    Player enemy = create_player(0);
    enemy.team[0] = create_creature(type, rank);
    enemy.creature = 1;

    int choice;
    do {
        print_and_wait("What do you want to do?\n1. Catch\n2. Fight", player);
        choice = io_get_switch_input();
        //choice = get_sw();

        switch(choice){
            case 1:
                catchsys(player, &player->items);
                return;
            case 2: {
                int result = fightsys(player, &enemy);
                if (result == 0) { // player lost
                    print_and_wait("You lost the battle... You are sent back to Professor Maple's lab.", player);
                    enter_room(player, 1);
                    return;
                }
                return;
            }

            default:
                invalid(player);
        }
    } while(1);
}

//============= Buy item helper ==============
void buy_item(Player *player, int *count, int price, const char *itemName) {
    if (player->coins >= price) {
        player->coins -= price;
        (*count)++;
        char buf[64];
        sprintf(buf, "Purchase successful! You bought a %s!", itemName);
        print_and_wait(buf, player);
    } else {
        print_and_wait("Not enough coins!", player);
    }
}
