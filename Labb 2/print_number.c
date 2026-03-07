#include <stdio.h>
#include <stdlib.h>
// #include "sieves.h"

#define COLUMNS 6 //Bestämmer hur många kolumner per rad i output

void print_number(int n) {
    static int varv = 0; //static så att räknaren kommer ihåg hur många varv som körts

    printf("%10d ", n); //%10d ger 10 stegs mellanrum mellan varje siffra i ouput

    varv++;
    if (varv % COLUMNS == 0) {
        printf("\n"); //ny rad efter 6 kolumner
    }
}

int is_prime(int n){
      if (n<2){
        return 0;
    }
    for (int i = 2; i * i <= n; i++){
        if (n % i == 0){
            return 0;
        }
    } 
    return n;
}

void print_primes(int n){
    
    for (int i = 2; i <= n; i++){
        if (is_prime(i)){
            print_number(i);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}