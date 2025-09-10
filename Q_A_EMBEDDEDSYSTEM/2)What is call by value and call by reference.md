# Call by Value vs Call by Reference in C

## 1. Call by Value
- When you pass a variable to a function **by value**, a *copy* of that variable is made.  
- The function works with the copy, not the original.  
- So, changes inside the function **do not affect** the original variable.

### Example
```c
#include <stdio.h>

void change(int x) {
    x = 20;   // only the copy changes
}

int main() {
    int a = 10;
    change(a);
    printf("a = %d\n", a); // still 10
    return 0;
}
```
## 2. Call by Reference
 - In C, there’s no direct call by reference like in C++.
 - But you can achieve it using pointers.
- Instead of passing the value, you pass the address of the variable.
- The function then works with the original variable.
```c
#include <stdio.h>

void change(int *x) {
    *x = 20;   // changes the original variable
}

int main() {
    int a = 10;
    change(&a);
    printf("a = %d\n", a); // now 20
    return 0;
}
```

## ✅ Key Difference
- **Call by Value:** Function gets a copy → original is safe.
- **Call by Reference:** Function gets the address → original can be changed.


![alt text](image-1.png)
