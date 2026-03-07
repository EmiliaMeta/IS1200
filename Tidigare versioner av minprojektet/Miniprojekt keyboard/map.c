//skapar 9 rum - Emilia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // för strcmp / atoi
#include "map.h"
#include "menu.h"
#include "systems.h"
#include "textstring.h"

// Boss flags (only one battle per gameplay)
static int miniboss_done = 0;
static int gym1_done = 0;
static int gym2_done = 0;
static int gym3_done = 0;
static int finalboss_done = 0;

void init_map(Room map[]) {
    map[0].id = 1; map[0].description = "Professor Maples laboratorium";
    map[1].id = 2; map[1].description = "Professors Maples personal garden";
    map[2].id = 3; map[2].description = "Shop";
    map[3].id = 4; map[3].description = "The rocky cliffs";
    map[4].id = 5; map[4].description = "Room 5: A miniboss appears!";
    map[5].id = 6; map[5].description = "Room 6: Rock training ground (Gym Leader 1)";
    map[6].id = 7; map[6].description = "Room 7: Paper arena (Gym Leader 2)";
    map[7].id = 8; map[7].description = "Room 8: Scissors pass (Gym Leader 3)";
    map[8].id = 9; map[8].description = "Room 9: Slutboss! Random type, 10 HP.";
}

void enter_room(Player *player, int room_id) {
    // Nytt: manus från textstring
    room_script(room_id, player);

    switch(room_id) {
        case 1: {
            // statisk flagga så tutorial bara körs en gång
            static int tutorial_done = 0;

            // Om spelaren inte har valt start-creature ännu
            if (player->creature == 0) {
                choose_start_creature(player);  // kör bara Maple här
            } 
            else {
                // Professor Maple återupplivar bara om ALLA creatures är döda
                int all_dead = 1;
                for (int i = 0; i < player->creature; i++) {
                    if (player->team[i].is_alive) {
                        all_dead = 0;
                        break;
                    }
                }
                if (all_dead) {
                    for (int i = 0; i < player->creature; i++) {
                        player->team[i].is_alive = 1;
                        player->team[i].health = player->team[i].max_health;
                    }
                    print_and_wait("Professor Maple: Your entire team has fallen... I revived all your creatures!", player);
                    print_and_wait("The professor gives you a ride back. Sweet!", player);
                } else {
                    print_and_wait("Professor Maple: You seem to be ready, your creatures can still fight!", player);
                }
            }

            // Kör tutorial bara en gång
            if (!tutorial_done) {
                // Catch tutorial
                tutorial_intro(player);
                print_and_wait("\n--- TUTORIAL: Catch ---\n", player);
                catchsys(player, &player->items);

                // Menu tutorial
                tutorial_menu(player);
                print_and_wait("\n--- TUTORIAL: Menu ---\n", player);
                game_menu(player);

                // Fight tutorial
                tutorial_fight(player);
                print_and_wait("\n--- TUTORIAL: Fight ---\n", player);
                Player enemy = create_player(0);
                CreatureType t = rand() % 3;
                enemy.team[0] = create_creature(t, NORMAL);
                enemy.creature = 1;
                fightsys(player, &enemy);

                tutorial_end(player);
                tutorial_done = 1; // markera att tutorial är gjord
            }
            break;
        }

        case 2:
            // TODO: loopa fightsys med SCISSORS-creatures
            encountersys(player, NORMAL); 
            break;

        case 3:
            shopsys(player, &player->items);
            break;

        case 4: {
            static int event_done = 0;  // flagga för att inte loopa

            if (event_done) {
                print_and_wait("This place seems quiet now... nothing more to find here.", player);
                break;
            }

            int event = rand() % 3; // 0=loot, 1=healing, 2=coins
            switch(event) {
                case 0: { // Loot chest
                    int loot = rand() % 4; // 0–3
                    switch (loot) {
                        case 0: player->items.bronze_ball++; print_and_wait("You found a Bronze ball!", player); break;
                        case 1: player->items.silver_ball++; print_and_wait("You found a Silver ball!", player); break;
                        case 2: player->items.gold_ball++;   print_and_wait("You found a Gold ball!", player); break;
                        case 3: player->items.revive++;      print_and_wait("You found a Revive!", player); break;
                    }
                    break;
                }

                case 1: { // Healing spring
                    int revived = 0;
                    for (int i = 0; i < player->creature; i++) {
                        if (!player->team[i].is_alive || player->team[i].health < player->team[i].max_health) {
                            player->team[i].is_alive = 1;
                            player->team[i].health = player->team[i].max_health;
                            revived = 1;
                        }
                    }
                    if (revived) {
                        print_and_wait("You discover a glowing spring! Your creatures are fully healed!", player);
                    } else {
                        print_and_wait("You discover a glowing spring... but your creatures are already at full health.", player);
                    }
                    break;
                }

                case 2: { // Coin stash
                    int coins_found = (rand() % 5) + 1; // 1–5 coins
                    player->coins += coins_found;
                    char buf[64];
                    sprintf(buf, "You find a pile of coins! You pick up %d coins.", coins_found);
                    print_and_wait(buf, player);
                    break;
                }
            }
            event_done = 1; // Körs bara en gång
            break;
        }

        case 5: {
            if (!miniboss_done) {
                print_and_wait("A miniboss blocked your path!", player);
                Player enemy = create_player(0);
                CreatureType t = rand() % 3;
                enemy.team[0] = create_creature(t, MINIBOSS);
                enemy.creature = 1;
                int result = fightsys(player, &enemy);

                if (result == 1) {
                    miniboss_done = 1;
                    print_and_wait("You defeated the miniboss! The path is now clear.", player);
                } else if (result == 0) {
                    print_and_wait("You lost against the miniboss... Try again!", player);
                } else {
                    print_and_wait("You escaped... the miniboss still blocks your path.", player);
                }
            } else {
                print_and_wait("The miniboss has already been defeated. You may proceed freely.", player);
            }
            break;
        }

        case 6: {
            print_and_wait("You arrive at a rocky training ground...", player);
            int choice;
            do {
                print_and_wait("What do you want to do?\n1. Wander around (wild Rock creatures)\n2. Try to proceed", player);
                choice = get_sw();
                if (choice == 1) {
                    encountersys_type(player, NORMAL, ROCK); // wild Rock only
                } else if (choice == 2) {
                    if (!gym1_done) {
                        print_and_wait("Gym Leader Rox: My rocks will crush you!", player);
                        Player leader = create_player(0);
                        leader.team[0] = create_creature(ROCK, MINIBOSS);
                        leader.creature = 1;
                        int result = fightsys(player, &leader);

                        if (result == 1) {
                            gym1_done = 1;
                            print_and_wait("You defeated Gym Leader Rox! You may now proceed freely.", player);
                        } else if (result == 0) {
                            print_and_wait("You lost against Gym Leader Rox... Try again!", player);
                        } else {
                            print_and_wait("You escaped... the Gym Leader still blocks your path.", player);
                        }
                    } else {
                        print_and_wait("Gym Leader Rox has already been defeated. You may proceed freely.", player);
                    }
                }
            } while(choice != 2 || !gym1_done);
            break;
        }

        case 7: {
            print_and_wait("You step into a papery arena, sheets fluttering in the wind...", player);
            int choice;
            do {
                print_and_wait("What do you want to do?\n1. Wander around (wild Paper creatures)\n2. Try to proceed", player);
                choice = get_sw();
                if (choice == 1) {
                    encountersys_type(player, NORMAL, PAPER); // wild Paper only
                } else if (choice == 2) {
                    if (!gym2_done) {
                        print_and_wait("Gym Leader Paige: My Paper will fold you flat!", player);
                        Player leader = create_player(0);
                        for (int i = 0; i < 2; i++) {
                            leader.team[i] = create_creature(PAPER, MINIBOSS);
                            leader.creature++;
                        }
                        int result = fightsys(player, &leader);

                        if (result == 1) {
                            gym2_done = 1;
                            print_and_wait("You defeated Gym Leader Paige! You may now proceed freely.", player);
                        } else if (result == 0) {
                            print_and_wait("You lost against Gym Leader Paige... Try again!", player);
                        } else {
                            print_and_wait("You escaped... the Gym Leader still blocks your path.", player);
                        }
                    } else {
                        print_and_wait("Gym Leader Paige has already been defeated. You may proceed freely.", player);
                    }
                }
            } while(choice != 2 || !gym2_done);
            break;
        }

        case 8: {
            print_and_wait("You enter a sharp pass filled with the sound of blades clashing...", player);
            int choice;
            do {
                print_and_wait("What do you want to do?\n1. Wander around (wild Scissors creatures)\n2. Try to proceed", player);
                choice = get_sw();
                if (choice == 1) {
                    encountersys_type(player, NORMAL, SCISSORS); // wild Scissors only
                } else if (choice == 2) {
                    if (!gym3_done) {
                        print_and_wait("Gym Leader Slash: My Scissors will cut your hopes!", player);
                        Player leader = create_player(0);
                        for (int i = 0; i < 3; i++) {
                            leader.team[i] = create_creature(SCISSORS, MINIBOSS);
                            leader.creature++;
                        }
                        int result = fightsys(player, &leader);

                        if (result == 1) {
                            gym3_done = 1;
                            print_and_wait("You defeated Gym Leader Slash! You may now proceed freely.", player);
                        } else if (result == 0) {
                            print_and_wait("You lost against Gym Leader Slash... Try again!", player);
                        } else {
                            print_and_wait("You escaped... the Gym Leader still blocks your path.", player);
                        }
                    } else {
                        print_and_wait("Gym Leader Slash has already been defeated. You may proceed freely.", player);
                    }
                }
            } while(choice != 2 || !gym3_done);
            break;
        }

        case 9: {
            if (!finalboss_done) {
                print_and_wait("The Final boss is waiting...", player);
                Player boss = create_player(0);
                CreatureType t = rand() % 3;
                boss.team[0] = create_creature(t, BOSS);
                boss.creature = 1;
                int result = fightsys(player, &boss);

                if (result == 1) {
                    finalboss_done = 1;
                    print_and_wait("You defeated the Final Boss!", player);
                    print_and_wait("=== CONGRATULATIONS, YOU WIN THE GAME! ===", player);
                    exit(0);
                } else if (result == 0) {
                    print_and_wait("You died! You are thrown back to room 8...", player);
                    enter_room(player, 8);
                } else {
                    print_and_wait("You escaped... but the Final Boss still awaits!", player);
                }
            } else {
                print_and_wait("The Final Boss is already defeated. Victory is yours!", player);
                exit(0);
            }
            break;
        }

        default:
            print_and_wait("Unknown room!", player);
    }
}

// Metod for att rora sig mellan rum
// Metod for att rora sig mellan rum
void move_player(Player *player, Room map[]) {
    int room_id = 1;        // starta i rum 1
    int max_reached = 1;    // hogsta rummet spelaren natt
    char buffer[16];
    int choice;

    // Nytt: starta spelet i rum 1 direkt
    enter_room(player, room_id);
    max_reached = 1; // markera att rum 1 ar avklarat

    while(1) {
        printf("\nYou are in: %s\n", map[room_id-1].description);

        // Skriv ut tillåtna val baserat på rummet
        if (room_id == 1) {
            printf("What do you want to do? (1 = forward, 0 = end, m = menu): ");
        } else if (room_id == NUM_ROOMS) {
            printf("What do you want to do? (2 = backward, 0 = end, m = menu): ");
        } else {
            printf("What do you want to do? (1 = forward, 2 = backward, 0 = end, m = menu): ");
        }

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue; // hoppa om input misslyckas
        }

        // ENTER = gör inget, fråga igen
        if (buffer[0] == '\n') {
            continue;
        }

        // Nytt: oppna meny
        if (buffer[0] == 'm' || buffer[0] == 'M') {
            game_menu(player);
            continue;
        }

        // Avsluta spel
        if (buffer[0] == '0') {
            print_and_wait("The Game has ended...", player);
            break;
        }

        // Konvertera input till int
        choice = atoi(buffer);

        // Rörelse framåt
        if (choice == 1) {
            if (room_id < NUM_ROOMS) {
                room_id++;
                if (room_id > max_reached) {
                    max_reached = room_id;
                }
                enter_room(player, room_id);
            } else {
                print_and_wait("You cannot go forward from here.", player);
            }
        }
        // Rörelse bakåt
        else if (choice == 2) {
            if (room_id > 1) {
                room_id--;
                enter_room(player, room_id);
            } else {
                print_and_wait("You cannot go backward from here.", player);
            }
        }
        else {
            print_and_wait("Invalid input! Use 1 = forward, 2 = backward.", player);
        }
    }
}
