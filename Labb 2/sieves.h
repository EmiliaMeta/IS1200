#ifndef SIEVES_H
#define SIEVES_H

#include <stddef.h>

#define COLUMNS 6

/* Utskrift av ett (1) tal, kolumnformaterat */
void print_number(int n);

/* Trial-division-varianten (Assignment 2) */
int  is_prime(int n);
void print_primes(int n);

/* Sieve-varianter (Assignment 3) – om du bygger dem */
void print_sieves(int n);          // stack
void print_sieves_heap(int n);     // heap

#endif
