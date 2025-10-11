/*Write a function in C that takes an array of integers and its size, and returns the second largest element. 
Constraints: 
● Do not sort the array. 
● Assume the array has at least two distinct elements.
*/

#include <stdio.h>
int second_largest(int [], int );
int main()
{
    int size,sec_largest;
    printf("Enter the number of elements: ");
    scanf("%d", &size);
    int arr[size];
    printf("Enter the elements:\n");
    for(int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }
    sec_largest = second_largest(arr, size);
    printf("The 2nd largest element is: %d\n", sec_largest);
    return 0;
}

int second_largest(int arr[], int size)
{
    int largest,second_largest;   
    if(arr[1]>arr[0])            
    {
        largest=arr[1];          
        second_largest=arr[0];   
    }
    else
    {
         largest=arr[0];         
         second_largest=arr[1];  
    }
    for(int i=2;i<size;i++)      
    {
    if(arr[i]>largest)        
        {
            second_largest=largest;  
            largest=arr[i];         
        }
        else if(arr[i]>second_largest && arr[i]<largest)
        {
            second_largest=arr[i];   
        }
    }
    return second_largest;
}