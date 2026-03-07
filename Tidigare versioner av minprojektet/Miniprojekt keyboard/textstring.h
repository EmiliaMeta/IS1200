//Text UI
void print_and_wait(const char *msg, Player *player);

//shop
void shop_intro();
void shop_options();
void shop_purchase(const char *itemName, int count);
void no_coins();
void current_coins(int coins);

//player
void player_info(Player *player);

//creature
void found_creature();
void no_pokemonballs(const char *itemName);
void pokemonballs(const char *itemName, int count);
void option();
void catched();
void escaped();
void exit_text(Player *player);
void invalid(Player *player);

//battle
void battle_intro();

int get_sw(void);

void room_script(int room_id, Player *player);

void choose_start_creature(Player *player);

void tutorial_intro(Player *player);
void tutorial_menu(Player *player);
void tutorial_fight(Player *player);
void tutorial_end(Player *player);

//menu 
void menu();
void acion_menu();

//funktioner
int damage_calc(Creature *attacker, Creature *defender);
void creature_info(const char *label, Creature *c, Player *player);
void purchase_type(Player *player, int price, int *item_count, const char *itemName);
int try_ball(const char *itemName, int *count, int rate );
