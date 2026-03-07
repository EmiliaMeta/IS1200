/**
 * main.c
 * -------------------------
 * Startar spelet PocketCreature.
 * Initierar system, spelare och karta.
 * För DTEK-V och PC-simuleringsläge.
 */

#include "io.h"
#include "map.h"
#include "menu.h"
#include "systems.h"
#include "objects.h"
#include "textstring.h"
#include "random.h"

#ifdef PC_TEST
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#endif

#ifdef PC_TEST
#include <stdio.h>
#include "objects.h"
#include "textstring.h"

// PC-version av choose_start_creature (ersätter DTEK-inmatning)
void choose_start_creature(Player *p) {
    print_text("\nChoose your starting creature!\n");
    print_text("1 = Rock\n2 = Paper\n3 = Scissors\n");
    int choice = 0;

    while (choice < 1 || choice > 3) {
        printf("Enter choice (1-3): ");
        scanf("%d", &choice);
    }

    CreatureType t;
    if (choice == 1) t = ROCK;
    else if (choice == 2) t = PAPER;
    else t = SCISSORS;

    p->team[0] = create_creature(t, NORMAL);
    p->creature = 1;

    printf("You chose your starter! The adventure begins!\n");
}
#endif


// === Globala deklarationer ===
void shopsys(Player *player, Item *item);
void catchsys(Player *player, Item *item);
int fightsys(Player *player, Player *enemy);

// Global spelare (för meny och interrupts)
Player global_player;

// === Huvudprogram ===
int main(void) {
#ifdef PC_TEST
    printf("\n=== PocketCreature Simulation ===\n");
    printf("Use numbers 1 to 4 for menu choices, and 'a' to simulate button.\n");
    printf("Press ENTER to start the game...\n");
    getchar();
#endif

    // Initiera I/O och eventuella interrupts
    io_init();

#ifndef PC_TEST
    enable_interrupt();
#endif

    // Initiera slumpgeneratorn
#ifdef PC_TEST
    srand(time(NULL));
#else
    srand_custom(12345);
#endif

    // Skapa spelaren med startmynt
    Player player = create_player(10);
    global_player = player; // för menyer och interrupts

    // Initiera kartan
    Room map[NUM_ROOMS];
    init_map(map);

    // Starta spelets huvudloop (hanteras av move_player)
    move_player(&player, map);

#ifdef PC_TEST
    printf("\nGame finished. Press ENTER to exit.\n");
    getchar();
#endif

    return 0;
}
