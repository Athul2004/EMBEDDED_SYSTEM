#include <stdio.h>
unsigned char modifyRegistervalue(unsigned char reg) 
{
    reg |= (1 << 2);   // Set 3rd
    reg &= ~(1 << 5);  // Clear 6th
    reg ^= (1 << 0);   // Toggle 1st 
    return reg;
}
void printBinary(unsigned char n) {
    for (int i = 7; i >= 0; i--) 
    {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}
int main()
{
    unsigned char reg = 0b00101101; 
    printf("Original register: ");
    printBinary(reg);
    reg = modifyRegistervalue(reg); 
    printf("Modified register: ");
    printBinary(reg);
    return 0;
}
