#include<stdio.h>

struct Employee{
    char *name;
    int emp_id;
    float salary;
};

int main()
{
    int a = 46;
    int arr[5] = {1, 42, 32, 43, 51};
    int *arr_ptr = arr;
    int *p = &a;
    struct Employee emp1;
    emp1.name = "Neeraj";
    //printf("Nmae of the employee = %s\n", emp1.name);
    printf("Address of a = %p\n", p);
    printf("Value of a = %d\n", *p);
    printf("Address of a from &a = %p\n", &a);
    printf("Address of arr = %p\n", arr);
    printf("Value of the first element of array = %d\n", *arr_ptr);
    printf("Value of second element of array = %d\n", *(arr_ptr + 1));
    printf("Value of *arr_ptr++ = %d\n", *arr_ptr++);
    printf("Value of *++arr_ptr = %d\n", *++arr_ptr);

    return 0;
}
