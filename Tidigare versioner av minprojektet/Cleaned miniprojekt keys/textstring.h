#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include "objects.h"

/**
 * textstring.h
 * -------------------------
 * Samlar alla textutskrifter, menyer och berättelser för spelet PocketCreature.
 * 
 * Här deklareras alla textfunktioner som används av systems, menu och map.
 *
 * Kodspråk: engelska
 * Kommentarer: svenska
 */

// Skriver ut en text långsamt (tecken för tecken)
void print_and_wait(const char *msg, Player *player);

/* ========================= SHOP TEXT ========================= */

// Skriver ut rubrik och meny för shoppen
void text_shop_intro(void);
void text_shop_menu(void);

// Skriver ut meddelanden relaterade till köp
void text_no_coins(void);
void text_item_purchased(const char *itemName);
void text_exit_shop(Player *player);

/* ========================= CATCH TEXT ========================= */

// Skriver ut meddelanden relaterade till att fånga creatures
void text_found_creature(CreatureType type);
void text_catched(void);
void text_escaped(void);
void text_no_balls(void);
void text_choose_ball(void);

/* ========================= BATTLE TEXT ========================= */

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

/* ========================= MENU TEXT ========================= */

// Skriver ut spelarens huvudmeny
void text_main_menu(void);

// Skriver ut ogiltig input eller avslut
void text_invalid(Player *player);
void text_exit_menu(Player *player);

/* ========================= ROOM TEXT ========================= */

// Skriver ut berättelse/introduktion för varje rum
void text_room_intro(int room_id, Player *player);

/* ========================= PLAYER INFO ========================= */

// Skriver ut spelarens sammanställda information (creatures, coins, items)
void text_player_info(Player *player);
void print_text(const char *text);
void print_room_info(int room_id);

#endif 
#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include "objects.h"  // för Player, Creature etc.

void print_and_wait(const char *text, Player *player);
void print_text(const char *text);
void print_dec(int val);

// Nytt:
void print_room_info(int room_id);

#endif

