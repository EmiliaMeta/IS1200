#include <stdio.h>
#include <math.h>

int get_tal(void){
    int a;
    printf("Ange ett heltal: ");
    scanf("%d", &a);

    printf("Du skrev: %d\n", a);
    return a;
}
int is_prime(int a){
    if (a<2){
        return 0;
    }
    for (int i = 2; i * i <= a; i++){
        if (a % i == 0){
            return 0;
        }
    }
    return 1;        
}

int main(void) {
    int tal = get_tal();
    if (is_prime(tal)){
        printf("%d ar ett primtal!\n", tal);
    }
    else {
        printf("%d ar inte ett primtal!\n", tal);
    }
    return 0;
}