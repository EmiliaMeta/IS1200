/**
 * Initializes the DTEK-V hardware, player, and game systems,
 * then starts the map traversal and gameplay loop.
 * contribution by both Aleena Amir and Emilia Lindqvist
 */

#include "objects.h"
#include "menu.h"
#include "map.h"
#include "io.h"
#include "random.h"

int main(void) {
    // Initierar slumpgeneratorn med ett statiskt seed
    srand_custom(12345);
    
    // Skapar spelare med startmynt
    Player player = create_player(10);

    // Initierar kartan
    Room map[NUM_ROOMS];
    init_map(map);

    // Startar spelrörelse (main gameplay loop)
    move_player(&player, map);

    // Returnerar aldrig hit under normalt spel
    return 0;
}
