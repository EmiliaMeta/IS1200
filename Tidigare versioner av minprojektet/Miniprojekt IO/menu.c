#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "systems.h"
#include "textstring.h"

// Huvudmeny för spel
void game_menu(Player *player) {
    int choice = 0;
    char buffer[16];

    do {
        printf("\n=== PLAYER MENU ===\n");
        printf("1. Show creatures\n");
        printf("2. Show items\n");
        printf("3. Player info\n");
        printf("4. Exit menu\n");
        printf("Input: ");

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            choice = atoi(buffer);
        } else {
            choice = 0;
        }

        switch(choice) {
            case 1:
                creature_card(player);   // skriver ut creatures
                break;
            case 2:
                printf("\n--- ITEMS ---\n");
                printf("Bronze balls: %d\n", player->items.bronze_ball);
                printf("Silver balls: %d\n", player->items.silver_ball);
                printf("Gold balls: %d\n", player->items.gold_ball);
                printf("Revives: %d\n", player->items.revive);
                break;
            case 3:
                printf("\n--- PLAYER INFO ---\n");
                printf("Name: Soot\n");  // du kan ersätta med variabel om du vill
                printf("Coins: %d\n", player->coins);
                break;
            case 4:
                exit_text(player);
                break;
            default:
                invalid(player);
        }
    } while(choice != 4);
}



void debug_menu(Player *player) {
    int choice = 0;
    char buffer[16];

    do {
       menu();
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            choice = atoi(buffer);
        } else {
            choice = 0;
        }

        switch(choice) {
            case 1:
                shopsys(player, &player->items);
                break;
            case 2:
                catchsys(player, &player->items);
                break;
            case 3: {
                creature(player);
                break;
            }
            case 4:
                player_info(player);
                break;
            case 5:
                exit_text(player);
                break;
            default:
                invalid(player);
        }
    } while(choice != 5);
}
