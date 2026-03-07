#ifndef RANDOM_H
#define RANDOM_H

// Pseudo-slumptalsgenerator (utan stdlib)

// Initierar seed i generatorn
void srand_custom(unsigned int seed);

// Returnerar nästa slumpvärde
unsigned int rand_custom(void);

#endif
