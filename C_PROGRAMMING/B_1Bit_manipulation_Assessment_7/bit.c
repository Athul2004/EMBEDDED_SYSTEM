#include <stdio.h>

// Function to print binary representation (32 bits)
void printBin(int n) {
    for (int i = 31; i >= 0; i--) {
        int mask = 1 << i;
        if (n & mask)
            printf("1");
        else
            printf("0");
    }
    printf("\n********************************\n");
}

// Function to set a bit at given position
int setBit(int n, int pos) {
    int mask = 1 << pos;
    n = n | mask;   // turn ON the bit at pos
    printf("Value after setting bit at %d position:\n", pos);
    printBin(n);
    return n;
}

// Function to clear a bit at given position
int clearBit(int n, int pos) {
    int mask = ~(1 << pos);
    n = n & mask;   // turn OFF the bit at pos
    printf("Value after clearing bit at %d position:\n", pos);
    printBin(n);
    return n;
}

int main() {
    int a;
    int pos, clear;

    printf("Enter the number: ");
    scanf("%d", &a);

    printf("Binary representation of input:\n");
    printBin(a);

    printf("Enter position to set bit: ");
    scanf("%d", &pos);
    a = setBit(a, pos);   // update a with new value

    printf("Enter position to clear bit: ");
    scanf("%d", &clear);  // ✅ fixed with &
    a = clearBit(a, clear);  // update a with new value

    return 0;
}



