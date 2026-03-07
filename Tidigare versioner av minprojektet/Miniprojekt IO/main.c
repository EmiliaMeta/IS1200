#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "objects.h"
#include "menu.h"
#include "map.h"
#include "io.h"

Player global_player; // används av interrupts


// Deklarationer
void shopsys(Player *player, Item *item);
void catchsys(Player *player, Item *item);
void fightsys(Player *player, Player *enemy);

int main(void) {
    io_init();
    srand(time(NULL));   // init slumpgenerator för rand()

    // Skapa en spelare
    Player player = create_player(10);

    //SPELMENY
    Room map[NUM_ROOMS];
    init_map(map);
    move_player(&player, map);
   // starta huvudmenyn (loop tills spelaren väljer exit)

    return 0;
}
