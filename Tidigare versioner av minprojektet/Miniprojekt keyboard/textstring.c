#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "objects.h"
#include "menu.h" 

//Globala variabler och helpers
static int first_prompt_shown = 0;
CreatureType current_found_type;  // global

//UI 
void menu(){
    printf("\n=== MAIN MENU ===\n");
    printf("1. Shop\n");
    printf("2. Catch a creature\n");
    printf("3. Fight (test fight)\n");
    printf("4. Show player info\n");
    printf("5. Exit menu\n");
    printf("Input: ");
}

void print_and_wait(const char *msg, Player *player) {
    printf("%s\n", msg);
    if (!first_prompt_shown) {
        printf("[Press ENTER to start the game, M for menu]\n");
        first_prompt_shown = 1;
    }
    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'm' || buffer[0] == 'M') {
            game_menu(player);
        }
        // Om bara enter trycktes görs inget -> fortsätt
    }
}

void exit_text(Player *player){
    print_and_wait("Exiting menu...", player);
}

void invalid(Player *player){
    print_and_wait("Invalid input!\n", player);
}


//Input-funktion
int get_sw(void) {
    char buffer[16];

    while (1) {
        printf("Input: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Om spelaren bara trycker enter -> fråga igen utan att "äta upp" nästa input
            if (buffer[0] == '\n') {
                continue;
            }
            // annars returnera siffervärdet
            return atoi(buffer);
        } else {
            clearerr(stdin);
        }
    }
}

//Shopssys
void shop_intro(){
    printf("\n=== The shop ===\n");
}
void shop_options(){
    printf("Would you like to buy an item?\n");
    printf("1. Buy a bronze ball (2 coins)\n");
    printf("2. Buy a Silver ball (4 coins)\n");
    printf("3. Buy a Gold ball (10 coins)\n");
    printf("4. Buy a Revive (6 coins)\n");
    printf("5. Exit the shop\n");
}
void shop_purchase(const char *itemName, int count){
    printf("Purchase successful! You bought a %s!\n", itemName);
    printf("You now have %d %s(s)!\n", count, itemName);
}
void no_coins(int missing){
    printf("Sorry not enough coins, you need %d coins more to purchase :(\n", missing);
}
void current_coins(int coins){
    printf("You have %d coins.\n", coins);
}

//Creature
void found_creature(){
    current_found_type = rand() % 3;  // spara typ
    printf("You have found a %s creature! Hurry, catch it before it escapes!!\n",
           type_to_string(current_found_type));
}


//Catchsys
void no_pokemonballs(const char *itemName){
    printf("Oh no you do not have a %s\n!", itemName);
}

void pokemonballs(const char *itemName, int count){
    printf("- %s (x%d)\n", itemName, count);
}

void option(){
    printf("Choose a ball to catch the creature:\n");
    printf("Input: 1 = Bronze, 2 = Silver, 3 = Gold\n");
}

void catched(){
    printf("Congratulations! You caught the creature!!\n");
}

void escaped(){
    printf("Oh no!!!! The creature escaped.....\n");
}


//Fightsys
void battle_intro(){
    printf("\n=== Battle start! ===\n");
}
void acion_menu(){
    printf("\n=== Action menu ===\n\n");
    printf("1. Attack!!!!\n");
    printf("2. Switch creature\n");
    printf("3. Use item\n");   // flyttat in både Revive och Pokéball här
    printf("4. Try to escape!\n\n");
}


//Manus 
void room_script(int room_id, Player *player) {
    switch(room_id) {
        case 1:
            // Introduktion sker i choose_start_creature
            break;

        case 2:
            print_and_wait("A wild creature has attacked!", player);
            break;

        case 3:
            print_and_wait("You walk into a small shop...", player);
            break;

        case 4:
            print_and_wait("You spot something unusual on the ground...", player);
            print_and_wait("What could it be?", player);
            break;


        case 5:
            print_and_wait("A dark shadow blocks your path", player);
            print_and_wait("It feels powerful... be ready!!", player);
            battle_intro();
            print_and_wait("A miniboss rises in front of you!", player);
            break;

        case 6:
            print_and_wait("The forest rustles... sicssors-creatures jumps in front of you!", player);
            battle_intro();
            break;

        case 7:
            print_and_wait("A storm of paper-creatures appear in front of you", player);
            battle_intro();
            break;

        case 8:
            print_and_wait("You step into a rocky, sharp cliffs surround you on every side.", player);
            battle_intro();
            print_and_wait("Several rock-creatures leap out before you!", player);
            print_and_wait("Prepare yourself for battle!", player);
            break;

        case 9:
            print_and_wait("It is dark and eerie", player);
            battle_intro();
            print_and_wait("The the BIG FINAL BOSS stands in front of you, get redy to fight!", player);
            break;

        default:
            printf("TBD\n");
    }
}

// Tutorial-manus för rum 1 (Professor Maple)
// Tutorial-del 1: intro + catch
void tutorial_intro(Player *player) {
    print_and_wait("Professor Maple: Now, let me show you how to play!", player);
    print_and_wait("Professor Maple: First, we will try to catch a creature", player);
    player->items.gold_ball++;
    print_and_wait("Professor Maple: Here, take a gold ball so you can try to catch one!", player);
    print_and_wait("Professor Maple: Use one of your balls to make an attempt.", player);
}

// Tutorial-del 2: meny
void tutorial_menu(Player *player) {
    print_and_wait("Professor Maple: Great! now check your backpack for your new friend!", player);
    print_and_wait("Professor Maple: In your backpack you can find your creatures, items & your coins.", player);
    print_and_wait("Professor Maple: Now, open your backpack so we can see your belongings!", player);
}

// Tutorial-del 3: fight
void tutorial_fight(Player *player) {
    print_and_wait("Professor Maple: And Finally, lets try a battle!!!", player);
    print_and_wait("Professor Maple: Battles are turn-based, your creature will always strike first!", player);
    print_and_wait("Professor Maple: But remember! Rock beats scissor, scissor beats paper & paper beats rock.", player);
    
}

// Tutorial end
void tutorial_end(Player *player) {
    print_and_wait("Professor Maple: If all the creatures dies, you can always return here.", player);
    print_and_wait("Professor Maple: You can always check your backpack by entering m", player);
    print_and_wait("Professor Maple: Good luck on your adventure, Soot!", player);
}

// Välja start-creature
void choose_start_creature(Player *player) {
    print_and_wait("Professor Maple: It is time to choose your first creature!", player);
    print_and_wait("Which one would you like?", player);
    printf("1. Rock - symbol of strength\n");
    printf("2. Paper - symbol of wisdom\n");
    printf("3. Scissors - symbol of speed\n");

    int choice = get_sw();
    CreatureType t;

    switch(choice) {
        case 1: t = ROCK; break;
        case 2: t = PAPER; break;
        case 3: t = SCISSORS; break;
        default:
            printf("Invalid input , You got Rock by default.\n");
            t = ROCK;
    }

    // Skapa och lägg till creature i spelarens team
    Creature c = create_creature(t, NORMAL);
    player->team[0] = c;
    player->creature = 1;

    print_and_wait("Congratulations! You have chosen your first creature!", player);
    print_and_wait("Take good care of it - together, your adventure begins now!", player);
}

//Player info
void player_info(Player *player){
    print_and_wait("\n--- PLAYER INFO ---", player);
    printf("Coins: %d\n", player->coins);
    printf("Items: Bronze(%d), Silver(%d), Gold(%d), Revive(%d)\n",
    player->items.bronze_ball,
    player->items.silver_ball,
    player->items.gold_ball,
    player->items.revive);
    printf("Creatures: %d\n", player->creature);
    for (int i = 0; i < player->creature; i++){
        Creature c = player->team[i];
        printf("-> Creature %d:\n   HP: %d/%d\n   Type: %s\n   Rank: %s\n   Alive: %s\n\n",
        i+1,
        c.health, c.max_health,
        type_to_string(c.type),
        rank_to_string(c.rank),
        c.is_alive ? "Yes" : "No");
        }
}

//Nyttiga funktioner
int damage_calc(Creature *attacker, Creature *defender){
    if ((attacker->type == ROCK && defender->type == SCISSORS) 
      ||(attacker->type == SCISSORS && defender->type == PAPER) 
      || (attacker->type == PAPER && defender->type == ROCK)) {
        return 2;
    }
    return 1;
}
void creature_info(const char *label, Creature *c, Player *player){
    char buf[128];
    sprintf(buf, "%s\nType: %s\nRank: %s\nHP: %d/%d!\n", label, 
    type_to_string(c->type),
    rank_to_string(c->rank),
    c->health,
    c->max_health);
    print_and_wait(buf, player);
}

void purchase_type(Player *player, int price, int *item_count, const char *itemName){
    if (player->coins >= price){
        player->coins -= price;
        (*item_count)++;
        shop_purchase(itemName, *item_count);
    } else {
        int missing = price -player->coins;
        no_coins(missing);
    }
}

int try_ball(const char *itemName, int *count, int rate){
    if(*count > 0){
        (*count)--;
        pokemonballs(itemName, *count);
        return rate;
    } else {
        no_pokemonballs(itemName);
        return -1;
    }
}

