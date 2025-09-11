#include<stdio.h>
int main(){

    int even[10];
    int i=0;
    int j=2;

    for (i=0; i<=9; i++){
        even[i]= j;
        printf("Value of even[%d] = %d\n", i, j);
        j+=2;
    }
}