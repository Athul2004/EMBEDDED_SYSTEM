#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    int *ptr;

    printf("Enter the number of elements: \n");
    scanf("%d", &n);

    ptr = (int *)malloc(n * sizeof(int));
    if (ptr == NULL) {
        printf("Memory allocation is failed\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Enter the element %d: ", i + 1);
        scanf("%d", (ptr + i));
    }

    printf("You entered:\n");
    for (i = 0; i < n; i++) {
        printf("%d\n", *(ptr + i));
    }

    free(ptr);
    return 0;
}
