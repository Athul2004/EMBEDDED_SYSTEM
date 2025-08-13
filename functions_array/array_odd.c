
#include<stdio.h>
int sum_array(int [] , int );
int print_odd_numbers(int [],int [], int);
int main()
{
    int numbers[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}; // Array to hold numbers
    int odd_array[20]; // Array to hold odd numbers
    int odd_count = 0; 
    printf("********************************************************************************\n");
    printf("ARRAY CONTENTS => ");
    for(int i =0 ;i< 20;i++)
    {
        printf("%d ",numbers[i]);
    }
    printf("\n******************************************************************************\n");
    int result = sum_array(numbers, 20); // Calculate the sum of the array
    printf("Sum of the array is => %d\n", result);
    printf("\n********************************************************************************\n");
    odd_count = print_odd_numbers(numbers,odd_array,20); // Print odd numbers from the array
    printf("Odd Numbers in the array => ");
    for(int i =0 ;i< odd_count;i++)
    {
        printf("%d ",odd_array[i]); // Print each odd number stored in odd_array
    }
    printf("\n*****************************************************************************\n");
    return 0;
}
//Function to calculate the sum of an array
int sum_array(int arr[], int size) 
{
    int sum = 0;
    for (int i = 0; i < size; i++) 
    {
        sum += arr[i];
    }
    return sum;
}
//Function to print odd numbers from the array and store them in odd_array
int print_odd_numbers(int arr[],int odd[], int size) 
{
    int odd_count = 0;
    for (int i = 0; i < size; i++) 
    {
        if (arr[i] % 2 != 0) 
        {
            odd[odd_count] = arr[i]; // Store odd number in odd_array from the original array.
            odd_count++;
        }
    }
    printf("Total odd numbers: %d\n", odd_count);
    printf("\n*******************************************************************************\n");
    return odd_count;
}