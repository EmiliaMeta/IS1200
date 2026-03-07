/**
 * map.c
 
 * Funktionen enter_room() kontrollerar vad som händer i varje rum:
 *  - Shop, Gym, Bossar, Minibossar
 *  - Händelser, encounters och tutorials
 */

#include "map.h"
#include "menu.h"
#include "systems.h"
#include "textstring.h"
#include "io.h"
#include "random.h"
int last_room_id = 1; 

// Interna flaggor för bossar och händelser
static int miniboss_done = 0;
static int gym1_done = 0;
static int gym2_done = 0;
static int gym3_done = 0;
static int finalboss_done = 0;
static int tutorial_done = 0;

// Initierar alla rum i spelet
void init_map(Room map[]) {
    map[0] = (Room){1, "Professor Maple's Laboratory"};
    map[1] = (Room){2, "Professor Maple's Garden"};
    map[2] = (Room){3, "Shop"};
    map[3] = (Room){4, "Mysterious summit"};
    map[4] = (Room){5, "Miniboss Arena"};
    map[5] = (Room){6, "Gym 1: Rock Ground"};
    map[6] = (Room){7, "Gym 2: Paper Arena"};
    map[7] = (Room){8, "Gym 3: Scissors Pass"};
    map[8] = (Room){9, "Final Boss Chamber"};
}

// Room config

void enter_room(Player *player, int room_id) {
    set_leds(1 << (room_id - 1));        // Tänd LED för aktuellt rum
    text_room_intro(room_id, player);    // Berättande text

    switch (room_id) {
        case 1: { // Laboratory (tutorial area)
            if (player->creature == 0) {
                print_and_wait("Professor Maple: Let's start your adventure!", player);
                choose_start_creature(player);
            } else {
                int all_dead = 1;
                for (int i = 0; i < player->creature; i++) {
                    if (player->team[i].is_alive)
                        all_dead = 0;
                }
                if (all_dead) {
                    for (int i = 0; i < player->creature; i++) {
                        player->team[i].is_alive = 1;
                        player->team[i].health = player->team[i].max_health;
                    }
                    print_and_wait("Professor Maple: I've revived your creatures!", player);
                } else {
                    print_and_wait("Professor Maple: Your team looks healthy!", player);
                }
            }

            if (!tutorial_done) {
                tutorial_intro(player);
                tutorial_catch(player);
                current_found_type = rand_custom() % 3;
                catchsys(player, &player->items);
                tutorial_menu(player);
                game_menu(player);
                Player enemy = create_player(0);
                enemy.team[0] = create_creature(rand_custom() % 3, NORMAL);
                enemy.creature = 1;
                tutorial_fight(player);
                fightsys(player, &enemy, 0);
                print_and_wait("Good job! You're ready for your journey!", player);
                
                tutorial_end(player);
                tutorial_done = 1;
            }
        } break;

        case 2:
            // Garden, slumpmässigt encounter
            current_found_type = rand_custom() % 3;
            encountersys(player, NORMAL);
            break;

        case 3:
            // Shop
            shopsys(player, &player->items);
            break;

        case 4: { // Mysterious summmit, slumpmässig händelse
            static int event_done = 0;
            if (event_done) {
                print_and_wait("You already explored this area.", player);
                break;
            }

            int event = rand_custom() % 3;
            if (event == 0) {
                int item_type = rand_custom() % 3;
                switch (item_type) {
                    case 0: player->items.bronze_ball++; print_and_wait("You found a Bronze ball!", player); break;
                    case 1: player->items.silver_ball++; print_and_wait("You found a Silver ball!", player); break;
                    case 2: player->items.gold_ball++; print_and_wait("You found a Gold ball!", player); break;
                }
            } else if (event == 1) {
                for (int i = 0; i < player->creature; i++) {
                    player->team[i].is_alive = 1;
                    player->team[i].health = player->team[i].max_health;
                }
                print_and_wait("You found a healing spring! Your team feels refreshed!", player);
            } else {
                int coins = (rand_custom() % 5) + 1;
                player->coins += coins;
                print("You found ");
                print_dec(coins);
                print(" coins!\n");
            } 

            event_done = 1;
        } break;

        case 5: // Miniboss
            if (!miniboss_done) {
                dialogue_miniboss(player);
                Player enemy = create_player(0);
                enemy.team[0] = create_creature(rand_custom() % 3, MINIBOSS);
                enemy.creature = 1;
                if (fightsys(player, &enemy,0) == 1) {
                    miniboss_done = 1;
                    print_and_wait("You defeated the miniboss!", player);
                    dialogue_miniboss_defeat(player);
                }
            } else {
                print_and_wait("The miniboss has already been defeated.", player);
            }
            break;

        case 6: // Gym 1 – Rock
            print_and_wait("You enter Gym 1: The Rock Grounds.", player);
            current_found_type = ROCK;
            encountersys_type(player, NORMAL, ROCK);
            if (!gym1_done) {
                dialogue_rox(player);
                Player leader = create_player(0);
                leader.team[0] = create_creature(ROCK, MINIBOSS);
                leader.creature = 1;
                if (fightsys(player, &leader, 0) == 1) {
                    gym1_done = 1;
                    dialogue_rox_defeat(player);
                }
            }
            break;

        case 7: // Gym 2 – Paper
            print_and_wait("You enter Gym 2: The Paper Arena.", player);
            current_found_type = PAPER;
            encountersys_type(player, NORMAL, PAPER);
            if (!gym2_done) {
                dialogue_paige(player);
                Player leader = create_player(0);
                leader.team[0] = create_creature(PAPER, MINIBOSS);
                leader.creature = 1;
                if (fightsys(player, &leader, 0) == 1) {
                    gym2_done = 1;
                    dialogue_paige_defeat(player);
                }
            }
            break;

        case 8: // Gym 3 – Scissors
            print_and_wait("You enter Gym 3: Scissors Pass.", player);
            current_found_type = SCISSORS;
            encountersys_type(player, NORMAL, SCISSORS);
            if (!gym3_done) {
                dialogue_slash(player);
                Player leader = create_player(0);
                leader.team[0] = create_creature(SCISSORS, MINIBOSS);
                leader.creature = 1;
                if (fightsys(player, &leader, 0) == 1) {
                    gym3_done = 1;
                    dialogue_slash_defeat(player);
                }
            }
            break;

        case 9: // Final Boss
            if (!finalboss_done) {
                dialogue_boss(player);
                print_and_wait("The Final Boss awaits!", player);
                Player boss = create_player(0);
                boss.team[0] = create_creature(rand_custom() % 3, BOSS);
                boss.creature = 1;
                if (fightsys(player, &boss, 1) == 1) {
                    finalboss_done = 1;
                    print_and_wait("Congratulations! You defeated the Final Boss!", player);
                    dialogue_boss_defeat(player);
                    print_and_wait("=== YOU WIN THE GAME ===", player);
                    while (1);
                } else {
                    print_and_wait("You were defeated...", player);
                    print_and_wait("GAME OVER!!!", player);
                    while (1);
                }
            } else {
                print_and_wait("You already beat the Final Boss!", player);
            }
            break;

        default:
            print_and_wait("Unknown room.", player);
            break;
    }
}

// Player movement

void move_player(Player *player, Room map[]) {
    int room_id = 1;
    int max_reached = 1;
    enter_room(player, room_id);

    while (1) {
        print("\nYou are in: ");
        print(map[room_id - 1].description);
        print("\n");

        print("Choose action: ");
        if (room_id == 1)
            print("(1 = forward, 0 = exit, 3 = menu)\n");
        else if (room_id == NUM_ROOMS)
            print("(2 = back, 0 = exit, 3 = menu)\n");
        else
            print("(1 = forward, 2 = back, 0 = exit, 3 = menu)\n");

        int val = io_switch();

        if (val == 0) {
            print_and_wait("You ended the game.", player);
            break;
        } else if (val == 1 && room_id < NUM_ROOMS) {
            last_room_id = room_id;
            room_id++;
            if (room_id > max_reached) max_reached = room_id;
            enter_room(player, room_id);
        } else if (val == 2 && room_id > 1) {
            last_room_id = room_id;
            room_id--;
            enter_room(player, room_id);
        } else if (val == 3) { // SW9 interrupt för meny
            game_menu(player);
        } else {
            text_invalid(player);
        }
    }
}
