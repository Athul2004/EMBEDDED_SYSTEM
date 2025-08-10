//CONDITIONAL CALCULATOR USING 'IF ELSE' STATEMENTS


#include <stdio.h>
int a, b;
int alu;


int sum(int, int);
int sub(int, int);
int mul(int, int);
float div(float, float);
void menu();


int main()
{
  printf("\nEnter First number: ");
  scanf("%d", &a);
  printf("Enter Secod number: ");
  scanf("%d", &b);
  menu();

  if (alu == 1)
  {
    printf("Sum: %d\n", sum(a, b));
  }
  else if (alu == 2)
  {
    printf("Difference: %d\n", sub(a, b));
  }
  else if (alu == 3)
  {
    printf("Product: %d\n", mul(a, b));
  }
  else if (alu == 4)
  {
    printf("Division: %f\n", div(a, b));
  }
  else
  {
    printf("ERROR");
  }
  return 0;
}
void menu()
{
  printf("\nEnter the required arithemetic operation\n");
  printf("1. Addition\n2. Subtraction\n3. Multiplication\n4. Division");
  printf("\nPlease choose your operation: ");
  scanf("%d", &alu);
}

int sum(int x, int y)
{

  int sum = 0;
  sum = x + y;
  return sum;
}

int sub(int x, int y)
{

  int sub = 0;
  sub = x - y;
  return sub;
}

int mul(int x, int y)
{
  int mul = 0;
  mul = x * y;
  return mul;
}

float div(float x, float y)
{
  float div = 0;
  div = x / y;
  return div;
}


// End of calculator_condition.c