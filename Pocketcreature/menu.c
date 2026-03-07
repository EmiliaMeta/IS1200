/**
 * menu.c

 * Spelaren kan:
 *  - Visa sina creatures
 *  - Visa sina items
 *  - Se spelarinformation
 *  - Avsluta menyn
 *  - Contribution by both Aleena Amir and Emilia Lindqvist
 */

#include "menu.h"
#include "textstring.h"
#include "systems.h"
#include "io.h"

// Huvudfunktion för spelarens meny
void game_menu(Player *player) {
    int choice = 0;

    while (choice !=3) {
        // Skriver ut menytext
        text_main_menu();
        choice = io_switch();

        switch (choice) {
            case 1:
                // Visa spelarens creatures
                creature_card(player);
                break;

            case 2:
                // Visa spelarens items
                text_player_info(player);
                break;
            case 3:
                // Avsluta menyn
                text_exit_menu(player);
                break;

            default:
                text_invalid(player);
                break;
        }
    }
}
