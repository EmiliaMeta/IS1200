#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sieves.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        clock_t t0 = clock();                  // starta tidtagning

        print_primes(atoi(argv[1]));           // kör din funktion

        clock_t t1 = clock();                  // stoppa tidtagning
        double seconds = (double)(t1 - t0) / CLOCKS_PER_SEC;
        printf("\nTime: %.3f s\n", seconds);   // skriv tiden
    } else {
        printf("Please state an integer number.\n");
    }
    return 0;
}
