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
    unsigned char reg;
    
    printf("Enter 8 bit value: ");
    scanf("%hhu", &reg);
    printf("You entered value: ");
    printBinary(reg);
    reg = modifyRegistervalue(reg); 
    printf("Modified register value: ");
    printBinary(reg);
    return 0;
}