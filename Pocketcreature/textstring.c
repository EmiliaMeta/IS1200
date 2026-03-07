
/**
 * textstring.c  
 * Dialog, tutorials, shop/fight meddelanden och hjälp text 
 * funktioner som komtrollerar vad som visas för spelaren via JTAG UART på DTEK-V kortet.
 * Contribution by both Aleena Amir and Emilia Lindqvist 
 */

#include "objects.h"
#include "menu.h"
#include "io.h"
#include "random.h"    // (ersätter stdlib.h rand(), då ej kapabel med DTEK-V)

CreatureType current_found_type; // Global variabel som håller koll på vilken typ av creature som hittats

// Skriver ut text långsamt (mening för mening/rad för rad)
void print_and_wait(const char *msg, Player *player) {
    print("\n");
    print(msg);
    print("\n");
    delay(900);
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
    print("4. Exit the shop\n");
}

// Visar att spelaren saknar tillräckligt med pengar
void text_no_coins(void) {
    print("Not enough coins!");
}

void text_item_purchased(const char *itemName) {
    print("You bought a ");
    print(itemName);
    print("!\n");
}

void text_exit_shop(Player *player) {
    print_and_wait("Exiting shop...", player);
}


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

void text_no_balls(const char *itemName) {
    print("\n\nYou don´t have any ");
    print(itemName);
    print("(s)!");
}

void text_choose_ball(void) {
    print("Choose a ball to try and catch the creature:\n");
    print(" ");
    print("\n1 = Bronze, 2 = Silver, 3 = Gold\n");
}

void chosen_ball(const char *itemName) {
    print("\nYay!! You chose ");
    print(itemName);
    print("!\n\n");
}

// Battle text

void text_battle_intro(void) {
    print("\n=== BATTLE START ===\n");
}

void text_battle_action_menu(void) {
    print("\n=== ACTION MENU ===\n");
    print("1. Attack! \n");
    print("2. Switch creature\n");
    print("3. Escape\n\n");
}

void text_battle_victory(Player *player) {
    print_and_wait("Enemy defeated!! You earned 1 coin.", player);
}

void text_battle_defeat(Player *player) {
    print_and_wait("All your creatures fainted... ", player);
}

void text_enemy_attack(int hp) {
    print("--> Enemy attacks! Your HP: ");
    print_dec(hp);
    print("\n");
}

void text_player_attack(int hp) {
    print("--> Player attacks! Enemy HP: ");
    print_dec(hp);
    print("\n");
}

// Meny text

void text_main_menu(void) {
    print("=== PLAYER MENU ===\n");
    print("1. Show creatures\n");
    print("2. Player info\n");
    print("3. Exit menu\n");
}

void text_invalid(Player *player) {
    print_and_wait("Invalid input!", player);
}

void text_exit_menu(Player *player) {
    print_and_wait("Exiting menu...", player);
}

// Rum text

static const char *room_names[] = {
    "Professor Maple's Laboratory",
    "Professor Maple's Garden",
    "The Shop",
    "Mysterious Summit",
    "Miniboss Arena",
    "Gym 1: Rockfall Grounds",
    "Gym 2: Cloudveil Paper Arena",
    "Gym 3: Scissors Pass",
    "Final Boss Chamber"
};

void text_room_intro(int room_id, Player *player) {
    const char *room_name = "Unknown Area";

    if (room_id >= 1 && room_id <= 9)
        room_name = room_names[room_id - 1];

    print("\n\n==================================================\n");
    print("  ENTERING ROOM ");
    print_dec(room_id);
    print(": ");
    print(room_name);
    print("\n==================================================\n");
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
    print(")\n");
}
void current_coins(int coins) {
    print("You have ");
    print_dec(coins);
    print(" coins.\n");
}
// Tutorials

// Tutorial: story introduktion
void tutorial_intro(Player *player){
    print_and_wait("Professor Maple: Your mission is to explore the world and overcome challanges.", player);
    print_and_wait("Professor Maple: Fight alongside your creatures and grow stronger as a trainer.", player);
    print_and_wait("Professor Maple: Find and defeat the various gym leaders to become champion!", player); 
    print_and_wait("Professor Maple: ..Hmm? Whats That? Reward for becoming champion?", player);
    print_and_wait("Professor Maple: Well.. The reward is honor ofcourse! And having fun!", player);
    print_and_wait("Professor Maple: Enough Questions!", player);
}
// Tutorial: catch
void tutorial_catch(Player *player) {
    print_and_wait("Professor Maple: Let me show you how to play!", player);
    print_and_wait("First, let's try to catch a creature.", player);
    player->items.gold_ball++;
    print_and_wait("Here, take a gold ball so you can try it!", player);
    print_and_wait("\n--- TUTORIAL: Catch ---\n", player);
}

// Tutorial: meny
void tutorial_menu(Player *player) {
    print_and_wait("Good job! Now check your backpack.", player);
    print_and_wait("You can find your creatures, items and coins there.", player);
    print_and_wait("\n--- TUTORIAL: Menu ---\n", player);
}

// Tutorial: strid
void tutorial_fight(Player *player) {
    print_and_wait("Now let's learn how to fight!", player);
    print_and_wait("Battles are turn-based, and you always attack first.", player);
    print_and_wait("Remember: Rock beats Scissors, Scissors beats Paper, Paper beats Rock.", player);
    print_and_wait("\n--- TUTORIAL: Fight ---\n", player);
}

// Tutorial: avslut
void tutorial_end(Player *player) {
    print_and_wait("If all your creatures faint, you can always return here.", player);
    print_and_wait("Here! Take some creature balls to start your adventure.", player);
    player->items.bronze_ball++;
    player->items.silver_ball++;
    print_and_wait("Good luck on your adventure!", player);
}

// Gym Leaders
void dialogue_rox(Player *player){
    print_and_wait("Rox: Welcome challenger! BAHAHAHAH", player);
    print_and_wait("A booming voice shouts from atop the cliffs.", player);
    print_and_wait("You see a buff woman looking down at you, gaze oozing with determination.", player); 
    print_and_wait("Rox: Strength, endurance, and an unbreakable spirit. That’s what true power means!", player); 
    print_and_wait("Rox: Only the trainers with a ROCK solid will can pass my gym. Fight me!", player);
    print_and_wait("Rox jumps down with a loud thud, shaking the ground beneath you.", player); 
    print_and_wait("Gym Leader Rox challenges you!", player);
}
void dialogue_rox_defeat(Player *player){
    print_and_wait("You defeated Gym Leader Rox!", player);
    print_and_wait("Rox: I guess even the hardest rock can be worn down… by determination.", player);
    print_and_wait("Rox: You’ve earned the Rock Badge… and my respect. Here!", player);
    print_and_wait("Rox: But so what?! You beat one gym leader. Don’t let it go to your head, punk. ", player); 
    print_and_wait("Rox: There's more gym leaders ahead you know.", player);
    print_and_wait("Rox: I'll only lose to the strongest, so don't you dare lose to them either.", player); 
    print_and_wait("Rox: Good luck, Soot!", player);
}
void dialogue_paige(Player *player){
    print_and_wait("Paige: Oh my.. to have come this far.. Welcome challenger..", player);
    print_and_wait("You hear a soft whisper coming somewhere from the thick mist surrounding you.", player);
    print_and_wait("Squinting, you can make out a dignified slender figure from the mist.", player); 
    print_and_wait("Paige: They say the pen is mightier than the sword… But in my arena, PAPER beats them all.", player); 
    print_and_wait("Paige: Only the trainers with wisdom beyond their years can dream of victory here..", player);
    print_and_wait("The mist dissipates and you see Paige standing directly in front of you smirking.", player); 
    print_and_wait("Gym Leader Paige challenges you!", player);
}
void dialogue_paige_defeat(Player *player){
    print_and_wait("You defeated Gym Leader Paige!", player);
    print_and_wait("Paige: I’ve… been torn to shreds…", player);
    print_and_wait("Paige: Take this Paper Badge. just… don’t spill coffee on it.", player);
    print_and_wait("Paige hands you a badge declaring that you have cleared her arena.", player); 
    print_and_wait("Paige: Don't relax yet, there is still one gym ahead..", player); 
    print_and_wait("Paige: Good luck, Soot.. ", player);
}
void dialogue_slash(Player *player){
    print_and_wait("You step onto a narrow bridge swaying in the mountain wind.", player);
    print_and_wait("Slash: So you’ve cut your way through the others, huh? Welcome challenger.", player);
    print_and_wait("A stoic figure waits across the bridge.. Calm, poised, eyes sharp as SCISSORS.", player); 
    print_and_wait("Slash: Lets make this quick, I have swords to sharpen.", player); 
    print_and_wait("Slash steps onto the swaying bridge calmly approaching you.", player); 
    print_and_wait("Gym Leader Slash challenges you!", player);
}
void dialogue_slash_defeat(Player *player){
    print_and_wait("You defeated Gym Leader Slash!", player);
    print_and_wait("Slash: My edge… it’s gone dull!", player);
    print_and_wait("Slash: Well done trainer. You have beat me and all the other gyms.", player);
    print_and_wait("Slash: Take my sharp badge. Mind the edges though! Haha!", player); 
    print_and_wait("Slash: With this you have become the pocketcreature champion!", player); 
    print_and_wait("..Slash suddenly becomes quiet, his face darkening..", player); 
    print_and_wait("Slash: ....", player);
    print_and_wait("Slash: ..Rumours say there's a strong creature ahead attacking trainers, causing chaos.", player); 
    print_and_wait("Slash: Perhaps since you are so strong, you can defeat this creature and restore peace.", player); 
    print_and_wait("Slash: But make sure you are thoroughly prepared going forward. You might not survive.", player);
    print_and_wait("Slash: Please be careful. I wish you luck, Soot.", player);
}
// Bosses
void dialogue_miniboss(Player *player){
    print_and_wait("Strong foe: Mwahaha! Your journey ends here puny trainer.", player);
    print_and_wait("Whats this!? A creature that can talk? You are flabbergasted.", player);
    print_and_wait("Strong foe: Far too long have we creatures been subservient to lowly trainers..", player); 
    print_and_wait("Strong foe: But no more! I will liberate my brethren!", player);
    print_and_wait("Strong foe: One by one we will defeat trainers and crush their morale.", player); 
    print_and_wait("Strong foe: The trainers will have no choice but to give up their dreams of becoming champion.", player); 
    print_and_wait("Strong foe: And soon we will all be free! And rule over mankind!", player);
    print_and_wait("Strong foe: And you.. Mwahaha... Shall be my first victim!!!", player);

}

void dialogue_miniboss_defeat(Player *player){
    print_and_wait("Strong foe: Guh.. How are you so strong?!", player);
    print_and_wait("Strong foe: ..Because you have a strong bond with your creatures?", player); 
    print_and_wait("Strong foe: Impossible! ..And yet your eyes tell me you speak truth..", player);
    print_and_wait("Strong foe: Perhaps creatures and trainers can work together..", player); 
    print_and_wait("Strong foe: Trainer, beware. I have more companions lying in wait for unsuspecting trainers.", player); 
    print_and_wait("Strong foe: You will undoubtedly face us again.", player);
    print_and_wait("Strong foe: But if the bonds with your creatures are real...", player);
    print_and_wait("Strong foe: ..I believe you can... restore our faith in trainers....", player);
    print_and_wait("The miniboss faints.", player);
}

void dialogue_boss(Player *player){
    print_and_wait("Boss: So we finally meet..", player);
    print_and_wait("Boss: Rumours have spread of a special trainer who treasures his creatures.", player); 
    print_and_wait("Boss: Hah! I will believe it when I see it.", player);
    print_and_wait("Boss: Now face me!", player); 
}

void dialogue_boss_defeat(Player *player){
    print_and_wait("Boss: Unbelievable..", player);
    print_and_wait("Boss: You sure proved me wrong.", player); 
    print_and_wait("Boss: I can see that you treasure your creatures.", player);
    print_and_wait("Boss: Not all trainers are good.. But not all are bad either.", player); 
    print_and_wait("Boss: Fine. I will no longer commit evil deeds. You trainers can relax.", player); 
    print_and_wait("And with that.. The boss retreats, defeated and somehow satisfied.", player);
}

// Låter spelaren välja sin första creature
void choose_start_creature(Player *player) {
    print_and_wait("Professor Maple: It's time to choose your first creature!\n", player);
    print("1. Rock: symbol of strength\n");
    print("2. Paper: symbol of wisdom\n");
    print("3. Scissors: symbol of speed\n");

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
    print_and_wait("Take good care of it. Your journey begins now!", player);
}
