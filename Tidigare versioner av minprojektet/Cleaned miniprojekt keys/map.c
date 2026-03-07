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
#include <stdio.h>


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
    map[3] = (Room){4, "Rocky Cliffs"};
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
                print_and_wait("Let's learn how to play!", player);
                catchsys(player, &player->items);
                game_menu(player);
                Player enemy = create_player(0);
                enemy.team[0] = create_creature(rand_custom() % 3, NORMAL);
                enemy.creature = 1;
                fightsys(player, &enemy);
                print_and_wait("Good job! You're ready for your journey!", player);
                tutorial_done = 1;
            }
        } break;

        case 2:
            // Garden – slumpmässigt encounter
            encountersys(player, NORMAL);
            break;

        case 3:
            // Shop
            shopsys(player, &player->items);
            break;

        case 4: { // Rocky cliffs – slumpmässig händelse
            static int event_done = 0;
            if (event_done) {
                print_and_wait("You already explored this area.", player);
                break;
            }

            int event = rand_custom() % 3;
            if (event == 0) {
                int item_type = rand_custom() % 4;
                switch (item_type) {
                    case 0: player->items.bronze_ball++; print_and_wait("You found a Bronze ball!", player); break;
                    case 1: player->items.silver_ball++; print_and_wait("You found a Silver ball!", player); break;
                    case 2: player->items.gold_ball++; print_and_wait("You found a Gold ball!", player); break;
                    case 3: player->items.revive++; print_and_wait("You found a Revive!", player); break;
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
                print_and_wait("A miniboss blocks your path!", player);
                Player enemy = create_player(0);
                enemy.team[0] = create_creature(rand_custom() % 3, MINIBOSS);
                enemy.creature = 1;
                if (fightsys(player, &enemy) == 1) {
                    miniboss_done = 1;
                    print_and_wait("You defeated the miniboss!", player);
                }
            } else {
                print_and_wait("The miniboss has already been defeated.", player);
            }
            break;

        case 6: // Gym 1 – Rock
            print_and_wait("You enter Gym 1: The Rock Grounds.", player);
            encountersys_type(player, NORMAL, ROCK);
            if (!gym1_done) {
                print_and_wait("Gym Leader Rox challenges you!", player);
                Player leader = create_player(0);
                leader.team[0] = create_creature(ROCK, MINIBOSS);
                leader.creature = 1;
                if (fightsys(player, &leader) == 1) {
                    gym1_done = 1;
                    print_and_wait("You defeated Gym Leader Rox!", player);
                }
            }
            break;

        case 7: // Gym 2 – Paper
            print_and_wait("You enter Gym 2: The Paper Arena.", player);
            encountersys_type(player, NORMAL, PAPER);
            if (!gym2_done) {
                print_and_wait("Gym Leader Paige challenges you!", player);
                Player leader = create_player(0);
                leader.team[0] = create_creature(PAPER, MINIBOSS);
                leader.creature = 1;
                if (fightsys(player, &leader) == 1) {
                    gym2_done = 1;
                    print_and_wait("You defeated Gym Leader Paige!", player);
                }
            }
            break;

        case 8: // Gym 3 – Scissors
            print_and_wait("You enter Gym 3: Scissors Pass.", player);
            encountersys_type(player, NORMAL, SCISSORS);
            if (!gym3_done) {
                print_and_wait("Gym Leader Slash challenges you!", player);
                Player leader = create_player(0);
                leader.team[0] = create_creature(SCISSORS, MINIBOSS);
                leader.creature = 1;
                if (fightsys(player, &leader) == 1) {
                    gym3_done = 1;
                    print_and_wait("You defeated Gym Leader Slash!", player);
                }
            }
            break;

        case 9: // Final Boss
            if (!finalboss_done) {
                print_and_wait("The Final Boss awaits!", player);
                Player boss = create_player(0);
                boss.team[0] = create_creature(rand_custom() % 3, BOSS);
                boss.creature = 1;
                if (fightsys(player, &boss) == 1) {
                    finalboss_done = 1;
                    print_and_wait("Congratulations! You defeated the Final Boss!", player);
                    print_and_wait("=== YOU WIN THE GAME ===", player);
                } else {
                    print_and_wait("You were defeated... Try again!", player);
                    enter_room(player, 8);
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
#include "map.h"
#include "io.h"
#include "menu.h"
#include "systems.h"
#include "textstring.h"
#include "objects.h"

// =============================
// move_player()
// -----------------------------
// Huvudloop för spelet — styr rörelse mellan rum,
// anropar enter_room() och kontrollerar slutvillkor.
// =============================
void move_player(Player *player, Room map[]) {

    int room_id = 1;        // Startar i rum 1
    int max_reached = 1;    // Högsta rummet spelaren nått

#ifdef PC_TEST
    print_text("\n=== PocketCreature Simulation ===\n");
    print_text("Use switch inputs (1–4) or 'a' to simulate BTN1.\n");
    print_text("Press BTN1 to start the game...\n");
    while (!get_btn());  // Vänta på knapptryck
#endif

    // Startar spelet i första rummet
    enter_room(player, room_id);
    max_reached = 1;

    while (1) {
        print_room_info(room_id); // visar rumsnamn via textstring.c

        // Visa val beroende på vilket rum man är i
        if (room_id == 1) {
            print_text("1: Move forward\n0: Exit game\nBTN1: Open menu\n");
        } else if (room_id == NUM_ROOMS) {
            print_text("2: Move backward\n0: Exit game\nBTN1: Open menu\n");
        } else {
            print_text("1: Move forward\n2: Move backward\n0: Exit game\nBTN1: Open menu\n");
        }

        int action = io_switch();  // Läs PC-simulerat val (1–4)
        if (get_btn()) {           // BTN öppnar meny
            game_menu(player);
            continue;
        }

        // --- Avsluta spel ---
        if (action == 0) {
            print_and_wait("The game has ended.\n", player);
            break;
        }

        // --- Gå framåt ---
        if (action == 1) {
            if (room_id < NUM_ROOMS) {
                room_id++;
                if (room_id > max_reached)
                    max_reached = room_id;
                enter_room(player, room_id);
            } else {
                print_and_wait("You cannot go further.\n", player);
            }
        }
        // --- Gå bakåt ---
        else if (action == 2) {
            if (room_id > 1) {
                room_id--;
                enter_room(player, room_id);
            } else {
                print_and_wait("You cannot go back.\n", player);
            }
        }
        else {
            print_text("Invalid input. Use 1–2 or BTN1.\n");
        }

        // --- Kontrollera HP ---
        if (player->team[0].health <= 0) {
            print_and_wait("Your team has fallen... Game Over!\n", player);
            break;
        }

        // --- Kontrollera seger ---
        if (room_id == NUM_ROOMS) {
            print_and_wait("You reached the final room! Congratulations!\n", player);
            break;
        }

#ifdef PC_TEST
        print_text("Press BTN1 to continue...\n");
        while (!get_btn());
#endif
    }

#ifdef PC_TEST
    print_text("\n--- Simulation ended. ---\n");
#endif
}
