#include <stdio.h>

int add(int c, int d); //function declaration

int main()
{
    int ret_sum=0;
    ret_sum=add(10,23); //function call
    printf("Sum of 2 numbers=%d" ,ret_sum);
}

//function definition
int add(int c, int d){
    int sum=0;
    sum=c+d;
    return sum;
}