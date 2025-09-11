#include <stdio.h>

int func() {
    // Stored in the stack
    int local_var;
    local_var++;
    return local_var;
}

int main() {
    int res;
    res = func();
    printf("%d\n", res);

    res = func();
    printf("%d\n", res);

    res = func();
    printf("%d\n", res);

    return 0;
}
