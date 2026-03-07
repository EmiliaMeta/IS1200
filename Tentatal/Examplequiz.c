#include <stdio.h>
/*
1:
*Implement a function with the signature int foo(int x) 
*that returns a value that is 10 times as large as x. 
*For instance, a function call foo(5) should return value 50.

2:
*Implement a function with the signature int bar(int x) 
*that returns x * x. 
*For instance, a function call foo(5) should return value 25.
*/

// ## 1
int foo(int x) {
  int multi = 0;
  for(int i = 0; i < 10; i++){multi += x;}
  return multi;
}

// ## 2
int bar(int x) {
  int squared = 0;
  for(int i=0; i < x; i++){squared += x;}
  return squared;
}

int main(void){
    printf("foo(5) = %d\n", foo(5));
    printf("bar(5) = %d\n", bar(5));
}