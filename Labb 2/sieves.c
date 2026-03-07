// #include "sieves.h"


   //från wikipedia
    /*algorithm Sieve of Eratosthenes is
        input: an integer n > 1.
        output: all prime numbers from 2 through n.

    let A be an array of Boolean values, indexed by integers 2 to n,
    initially all set to true.
    
    for i = 2, 3, 4, ..., not exceeding √n do
        if A[i] is true
            for j = i*i, i*i+i, i*i+2i, i*i+3i, ..., not exceeding n do
                set A[j] := false

    return all i such that A[i] is true.
    */

#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6 //Bestämmer hur många kolumner per rad i output

void print_number(int n) {
    static int varv = 0; //static så att räknaren kommer ihåg hur många varv som körts

    printf("%10d ", n); //%10d ger 10 stegs mellanrum mellan varje siffra i ouput

    varv++;
    if (varv % COLUMNS == 0) {
        printf("\n"); //ny rad efter 6 kolumner
    }
}

void print_sieves(int n){
    char A[n+1]; //för minneskonsumtion då char är 1 byte och endast 1 eller 0 lagras
    for (int i = 0; i <= n; i++){
        A[i] = 1; //sann
    }
    A[0] = 0; // falsk
    A[1] = 0; //falsk
    for (int i = 2; i * i <= n; i++){  
        if (A[i]){
            for (int j = i * i; j <= n; j += i){ 
                A[j] = 0; //falsk
            }
        }
    }
    for (int i = 2; i <= n; i++){
        if (A[i]){
            print_number(i);
        }
    }

// Surprise assignment 
    int x = 0; //x = Föregående primtal
    int steg = 0; //Räknar hur många gånger y - x = 8
    for (int y = 2; y < n; y++){ //y = primtalet efter x
        if (A[y] == 1){
            if (y - x == 8){
                steg ++;
            }
            x = y;
        }
    }
    printf("\n Sa har manga ganger finns en differens 8 mellan 2 pa varandra foljande primtal: %d", steg);
}



int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}
