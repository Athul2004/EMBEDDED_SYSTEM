#include<stdio.h>
struct Employee
{
    char name[32];
    int id;
    float salary;
};
void increment (struct Employee e,float percentage)
{
    e.salary=e.salary+((e.salary*percentage/100));
    printf("new salary in function %.2f",e.salary);
}
int main()
{
    struct Employee e={"Athul s", 101,75000.000};

    increment(e, 10.0);
    printf("Salary in main function %.2f",e.salary);

    return 0;
}

