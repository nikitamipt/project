#include <stdio.h>



int step(int a, int b, int c) {
    int a1 = a;
    for (int i = 1; i < b; i++) {
        printf("a1 = %d    \n", a1);
        a1 = (a1*a) % c;
    }
    return a1;
}

float an(float a1, float a2) {
    return (a2+1)/a1;
}


int main() {
    float a2 = 2019;
    float a1 = 2;
    for (int i = 0; i < 2016; i++) {
        float a0 = a2;
        a2 = an(a1, a2);
        a1 = a0;
        printf("%f\n", a2);
    }
    printf("%f", a2);


}


