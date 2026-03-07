
/**
 * textstring.c  
 
 * Dialogue, tutorials, shop/fight messages and helper text 
 * functions that control what is displayed to the player through the JTAG UART
 * on the DTEK-V board.
 */

#include "objects.h"
#include "textstring.h"
#include "menu.h"
#include "io.h"
#include "random.h"    // (ersätter stdlib.h rand())

#ifdef PC_TEST
#include <stdio.h>  // behövs för printf på PC
#endif

// Skriver text till terminal (PC) eller display (DTEK)
void print_text(const char *text) {
#ifdef PC_TEST
    printf("%s", text);
#else
    print(text);
#endif
}

// Skriver ut rumsinfo (kort version, för PC-simulering)
void print_room_info(int room_id) {
#ifdef PC_TEST
    switch (room_id) {
        case 1: printf("\n[Room 1] Professor Maple's Laboratory\n"); break;
        case 2: printf("\n[Room 2] Garden outside the lab\n"); break;
        case 3: printf("\n[Room 3] The shop\n"); break;
        case 4: printf("\n[Room 4] The rocky cliffs\n"); break;
        case 5: printf("\n[Room 5] A miniboss blocks your way\n"); break;
        case 6: printf("\n[Room 6] Gym Leader Rox awaits!\n"); break;
        case 7: printf("\n[Room 7] Gym Leader Paige's Paper Arena\n"); break;
        case 8: printf("\n[Room 8] Slash’s Scissor Pass\n"); break;
        case 9: printf("\n[Room 9] The Final Boss awaits...\n"); break;
        default: printf("\n[Unknown room]\n"); break;
    }
#else
    // På DTEK kan du använda print_and_wait() eller displayfunktionen
    print("Entered a new room\n");
#endif
}

// Skriver ut text långsamt (tecken för tecken) för berättarkänsla
void print_and_wait(const char *msg, Player *player) {
    int i = 0;
    while (msg[i] != '\0') {
        char c[2] = { msg[i], '\0' };
        print(c);
        delay(2);
        i++;
    }
    print("\n");
}

// Shop text

void text_shop_intro(void) {
    print("\n=== THE SHOP ===\n");
}

void text_shop_menu(void) {
    print("Would you like to buy an item?\n");
    print("1. Buy a Bronze ball (2 coins)\n");
    print("2. Buy a Silver ball (4 coins)\n");
    print("3. Buy a Gold ball (10 coins)\n");
    print("4. Buy a Revive (6 coins)\n");
    print("5. Exit the shop\n");
}

void text_no_coins(void) {
    print("Not enough coins!\n");
}

void text_item_purchased(const char *itemName) {
    print("You bought a ");
    print(itemName);
    print("!\n");
}

void text_exit_shop(Player *player) {
    print_and_wait("Exiting shop...", player);
}

// Catch text

void text_found_creature(CreatureType type) {
    print("You found a ");
    print(type_to_string(type));
    print(" creature! Try to catch it before it escapes!\n");
}

void text_catched(void) {
    print("Congratulations! You caught the creature!\n");
}

void text_escaped(void) {
    print("Oh no! The creature escaped!\n");
}

void text_no_balls(void) {
    print("You don't have any Pokéballs left!\n");
}

void text_choose_ball(void) {
    print("Choose a ball to try and catch the creature:\n");
    print("1 = Bronze, 2 = Silver, 3 = Gold\n");
}

// Battle text

void text_battle_intro(void) {
    print("\n=== BATTLE START ===\n");
}

void text_battle_action_menu(void) {
    print("\n=== ACTION MENU ===\n");
    print("Press BTN1 to ATTACK!\n");
    print("1. Go to shop\n");
    print("2. Switch creature\n");
    print("3. Use item\n");
    print("4. Try to escape\n");
}

void text_battle_victory(Player *player) {
    print_and_wait("Enemy defeated!! You earned 1 coin.", player);
}

void text_battle_defeat(Player *player) {
    print_and_wait("All your creatures fainted... Game Over!", player);
}

void text_enemy_attack(int hp) {
    print("Enemy attacks! Your HP: ");
    print_dec(hp);
    print("\n");
}

void text_player_attack(int hp) {
    print("Player attacks! Enemy HP: ");
    print_dec(hp);
    print("\n");
}

// Meny text

void text_main_menu(void) {
    print("\n=== PLAYER MENU ===\n");
    print("1. Show creatures\n");
    print("2. Show items\n");
    print("3. Player info\n");
    print("4. Exit menu\n");
}

void text_invalid(Player *player) {
    print_and_wait("Invalid input!", player);
}

void text_exit_menu(Player *player) {
    print_and_wait("Exiting menu...", player);
}

// Room text

void text_room_intro(int room_id, Player *player) {
    switch (room_id) {
        case 1:
            print_and_wait("Professor Maple's Laboratory.", player);
            break;
        case 2:
            print_and_wait("You enter a calm garden...", player);
            break;
        case 3:
            print_and_wait("You walk into a small shop.", player);
            break;
        case 4:
            print_and_wait("You arrive at the rocky cliffs.", player);
            break;
        case 5:
            print_and_wait("A miniboss stands before you!", player);
            break;
        case 6:
            print_and_wait("Gym 1: Rock training ground.", player);
            break;
        case 7:
            print_and_wait("Gym 2: Paper arena.", player);
            break;
        case 8:
            print_and_wait("Gym 3: Scissors pass.", player);
            break;
        case 9:
            print_and_wait("The Final Boss Chamber awaits...", player);
            break;
        default:
            print_and_wait("Unknown room.", player);
    }
}

// Player info

void text_player_info(Player *player) {
    print("\n--- PLAYER INFO ---\n");
    print("Coins: ");
    print_dec(player->coins);
    print("\nItems: Bronze(");
    print_dec(player->items.bronze_ball);
    print("), Silver(");
    print_dec(player->items.silver_ball);
    print("), Gold(");
    print_dec(player->items.gold_ball);
    print("), Revive(");
    print_dec(player->items.revive);
    print(")\n");
}


/*
// Global variabel som håller koll på vilken typ av creature som hittats.
CreatureType current_found_type;

// Skriver ut text tecken för tecken med liten fördröjning för dramatisk effekt.
void print_and_wait(const char *msg, Player *player) {
    int i = 0;
    while (msg[i] != '\0') {
        char c[2] = { msg[i], '\0' };
        print(c);
        delay(2);
        i++;
    }
    print("\n");
}

// Skriver ut text när spelaren lämnar menyn.
void exit_text(Player *player) {
    print_and_wait("Exiting menu...", player);
}

// Visar meddelande vid ogiltig inmatning.
void invalid(Player *player) {
    print_and_wait("Invalid input!\n", player);
}

//Shop

// Introduktion till shoppen.
void shop_intro() {
    print("\n=== THE SHOP ===\n");
}

// Skriver ut hur mycket pengar spelaren har.
void current_coins(int coins) {
    print("You have ");
    print_dec(coins);
    print(" coins.\n");
}

// Visar att köpet lyckades.
void shop_purchase(const char *itemName, int count) {
    print("You bought a ");
    print(itemName);
    print("!\nNow you have ");
    print_dec(count);
    print(" left.\n");
}

// Visar att spelaren saknar tillräckligt med pengar.
void no_coins(int missing) {
    print("Not enough coins! You need ");
    print_dec(missing);
    print(" more.\n");
}

//Catchsystem

// Skrivs ut när ett creature hittas.
void found_creature() {
    // Använd vår egen pseudo-random funktion istället för stdlib
    current_found_type = rand_custom() % 3;
    print("You have found a ");
    print(type_to_string(current_found_type));
    print(" creature! Try to catch it before it escapes!\n");
}

// Skrivs ut om spelaren saknar en viss pokébollstyp.
void no_pokemonballs(const char *itemName) {
    print("Oh no! You don't have any ");
    print(itemName);
    print(" left!\n");
}

// Skriver ut vilka bollar spelaren har kvar.
void pokemonballs(const char *itemName, int count) {
    print("- ");
    print(itemName);
    print(" (x");
    print_dec(count);
    print(")\n");
}

// Skriver ut valmöjligheter för att kasta bollar.
void option() {
    print("Choose a ball to try and catch the creature:\n");
    print("1 = Bronze, 2 = Silver, 3 = Gold\n");
}

// Skrivs ut vid lyckad fångst.
void catched() {
    print("Congratulations! You caught the creature!\n");
}

// Skrivs ut om varelsen flyr.
void escaped() {
    print("Oh no! The creature escaped!\n");
}

//Battle

// Skrivs ut vid start av strid.
void battle_intro() {
    print("\n=== BATTLE START! ===\n");
}

// Visar de val spelaren kan göra under strid.
void acion_menu() {
    print("\n=== ACTION MENU ===\n");
    print("Press BTN1 to ATTACK!\n");
    print("1. Go to shop\n");
    print("2. Switch creature\n");
    print("3. Use item\n");
    print("4. Try to escape\n");
}

//Manus

// Skriver ut text beroende på vilket rum spelaren befinner sig i.
void room_script(int room_id, Player *player) {
    switch (room_id) {
        case 1:
            break;
        case 2:
            print_and_wait("A wild creature appears!", player);
            break;
        case 3:
            print_and_wait("You walk into a small shop...", player);
            break;
        case 4:
            print_and_wait("You spot something shiny on the ground...", player);
            print_and_wait("What could it be?", player);
            break;
        case 5:
            print_and_wait("A dark shadow blocks your path...", player);
            print_and_wait("It feels powerful... be ready!", player);
            break;
        case 6:
            print_and_wait("The forest rustles... Scissors creatures appear!", player);
            break;
        case 7:
            print_and_wait("A storm of Paper creatures approaches!", player);
            break;
        case 8:
            print_and_wait("You step into a rocky path... Stones begin to move!", player);
            print_and_wait("Rock creatures appear before you!", player);
            break;
        case 9:
            print_and_wait("It's dark and eerie...", player);
            print_and_wait("The FINAL BOSS stands before you!", player);
            break;
        default:
            print("Unknown room.\n");
    }
}

// Tutorials

// Tutorial: introduktion fånga en creature.
void tutorial_intro(Player *player) {
    print_and_wait("Professor Maple: Let me show you how to play!", player);
    print_and_wait("First, let's try to catch a creature.", player);
    player->items.gold_ball++;
    print_and_wait("Here, take a gold ball so you can try it!", player);
}

// Tutorial: meny.
void tutorial_menu(Player *player) {
    print_and_wait("Good job! Now check your backpack.", player);
    print_and_wait("You can find your creatures, items and coins there.", player);
}

// Tutorial: strid.
void tutorial_fight(Player *player) {
    print_and_wait("Now let's learn how to fight!", player);
    print_and_wait("Battles are turn-based, and you always attack first.", player);
    print_and_wait("Remember: Rock beats Scissors, Scissors beats Paper, Paper beats Rock.", player);
}

// Tutorial: avslut.
void tutorial_end(Player *player) {
    print_and_wait("If all your creatures faint, you can always return here.", player);
    print_and_wait("You can open your backpack anytime by pressing 'm'.", player);
    print_and_wait("Good luck on your adventure!", player);
}

// Låter spelaren välja sin första creature.
void choose_start_creature(Player *player) {
    print_and_wait("Professor Maple: It's time to choose your first creature!", player);
    print("1. Rock - symbol of strength\n");
    print("2. Paper - symbol of wisdom\n");
    print("3. Scissors - symbol of speed\n");

    int choice = io_switch();
    CreatureType t;

    switch (choice) {
        case 1: t = ROCK; break;
        case 2: t = PAPER; break;
        case 3: t = SCISSORS; break;
        default:
            print("Invalid choice! You got Rock by default.\n");
            t = ROCK;
    }

    Creature c = create_creature(t, NORMAL);
    player->team[0] = c;
    player->creature = 1;

    print_and_wait("Congratulations! You have chosen your first creature!", player);
    print_and_wait("Take good care of it — your journey begins now!", player);
}

// Hjälppredikat

// Beräknar hur mycket skada en attack gör beroende på typer.
int damage_calc(Creature *attacker, Creature *defender) {
    if ((attacker->type == ROCK && defender->type == SCISSORS) ||
        (attacker->type == SCISSORS && defender->type == PAPER) ||
        (attacker->type == PAPER && defender->type == ROCK)) {
        return 2;
    }
    return 1;
}

// Skriver ut information om en creature (typ, HP, rank).
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

// Försöker köpa ett föremål om spelaren har råd.
void purchase_type(Player *player, int price, int *item_count, const char *itemName) {
    if (player->coins >= price) {
        player->coins -= price;
        (*item_count)++;
        shop_purchase(itemName, *item_count);
    } else {
        int missing = price - player->coins;
        no_coins(missing);
    }
}

// Försöker använda en boll och returnerar fångstchansen.
int try_ball(const char *itemName, int *count, int rate) {
    if (*count > 0) {
        (*count)--;
        pokemonballs(itemName, *count);
        return rate;
    } else {
        no_pokemonballs(itemName);
        return -1;
    }
}
*/