#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include "objects.h"

/**
 * textstring.h
 
 * Samlar alla textutskrifter, menyer och berättelser för PocketCreature.
 */

// Skriver ut en text långsamt (tecken för tecken)
void print_and_wait(const char *msg, Player *player);


// Skriver ut rubrik och meny för shoppen
void text_shop_intro(void);
void text_shop_menu(void);

// Skriver ut meddelanden relaterade till köp
void text_no_coins();
void text_item_purchased(const char *itemName);
void text_exit_shop(Player *player);
void chosen_ball(const char *itemName);

// Skriver ut meddelanden relaterade till att fånga creatures
void text_found_creature(CreatureType type);
void text_catched(void);
void text_escaped(void);
void text_no_balls(const char *itemName);
void text_choose_ball(void);


// Skriver ut inledningen av en strid
void text_battle_intro(void);

// Skriver ut stridsmenyn (val)
void text_battle_action_menu(void);

// Skriver ut meddelanden vid vinst och förlust
void text_battle_victory(Player *player);
void text_battle_defeat(Player *player);

// Skriver ut attacker från spelare och fiende
void text_enemy_attack(int hp);
void text_player_attack(int hp);


// Skriver ut spelarens huvudmeny
void text_main_menu(void);

// Skriver ut ogiltig input eller avslut
void text_invalid(Player *player);
void text_exit_menu(Player *player);


// Skriver ut berättelse/introduktion för varje rum
void text_room_intro(int room_id, Player *player);


// Skriver ut spelarens sammanställda information (creatures, coins, items)
void text_player_info(Player *player);

// Tutorial dialoger
void tutorial_intro(Player *player);
void tutorial_menu(Player *player);
void tutorial_fight(Player *player);
void tutorial_end(Player *player);

// Gym dialoger
void dialogue_rox(Player *player);
void dialogue_rox_defeat(Player *player);
void dialogue_paige(Player *player);
void dialogue_paige_defeat(Player *player);
void dialogue_slash(Player *player);
void dialogue_slash_defeat(Player *player);

// Boss dialoger
void dialogue_miniboss_defeat(Player *player);
void dialogue_miniboss(Player *player);
void dialogue_boss_defeat(Player *player);
void dialogue_boss(Player *player);
#endif 

