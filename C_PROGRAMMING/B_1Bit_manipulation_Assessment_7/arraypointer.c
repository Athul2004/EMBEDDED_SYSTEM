
#include <stdio.h>

int main()
{   
    float arr[5] = {1, 22, 43, 5, 6};

    void *arr_ptr = arr;  // base address of array

    int a = 10;
    float b = 3.14;
    int *p = &a;          // int pointer
    void *ptr = &b;       // void pointer

    printf("value of b = %f\n", *(float *)ptr);

    a = 20;
    printf("value of a = %d\n", a);

    *p = 30;
    printf("modified value of a using p = %d\n", a);

    *(float *)ptr = 2.55;
    printf("modified value of b using ptr = %f\n", b);

    // printing array values using arr_ptr
    printf("\nArray elements:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("arr[%d] = %f\n", i, *((float *)arr_ptr + i));
    }

    return 0;
}

