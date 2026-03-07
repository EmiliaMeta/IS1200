#include "random.h"

// Ger "slump" värden utan stdlib

static unsigned int current_seed = 1;

void srand_custom(unsigned int seed) {
    current_seed = seed;
}

unsigned int rand_custom(void) {
    current_seed = current_seed * 1103515245 + 12345;
    return (current_seed >> 16) & 0x7FFF;
}
