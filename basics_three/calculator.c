//Normal calculator using functions

#include <stdio.h>

int sum(int a, int b) {
    int sum = a+b;
    return sum;
}
int sub(int a, int b) {
    int sub = a-b;
    return sub;
}
int mul(int a, int b) {
    int mul = a*b;
    return mul;
}
float div(int a, int b) {
    float div = (float)a/(float)b;
    return div;
}
int main() {
    int a, b, sumr, subr, mulr;
    float divr;
    printf("Enter the first number : \n");
    scanf("%d", &a);
    printf("Enter the second number : \n");
    scanf("%d", &b);
    sumr= sum(a,b);
    subr= sub(a,b);
    mulr= mul(a,b);
    divr= div(a,b);
    printf("sum is = %d\n", sumr);
    printf("sub is = %d\n", subr);
    printf("mul is = %d\n", mulr);
    printf("div is = %f\n", divr);
    return 0;
}

// End of calculator.c