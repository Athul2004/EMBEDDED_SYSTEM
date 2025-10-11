/*
Write a C program to print a pyramid of stars for a given number n. 

If n = 5, the output should be:
              *
             ***
            *****
           *******
          *********

*/




#include <stdio.h>

int main()
{
    int n, i, j;

    printf("Enter the number of rows: ");
    scanf("%d", &n);

    for (i = 1; i<=n; i++)        // loop for rows
    {
        for (j = 1; j <= n-i; j++)  // print spaces
            printf(" ");

        for (j = 1; j <= 2*i - 1; j++)  // print stars
            printf("*");

        printf("\n");  // move to next line
    }

    return 0;
}
