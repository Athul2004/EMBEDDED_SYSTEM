#include <stdio.h>

struct Book {
    char title[51]; 
    char author[31]; 
    float price;
};

int main() {
    struct Book b[3]; 
    struct Book mostExpensiveBook;



    for (int i = 0; i < 3; i++) {
        printf("Enter details for Book %d:\n", i + 1);
        printf("Title : ");
        scanf("%s", b[i].title); 
        printf("Author : ");
        scanf("%s", b[i].author);
        printf("Price: ");
        scanf("%f", &b[i].price);
        printf("*************************\n");
    }

    mostExpensiveBook = b[0]; 
    for (int i = 1; i < 3; i++) {
        if (b[i].price > mostExpensiveBook.price) {
            mostExpensiveBook = b[i];
        }
    }

    printf("\nDetails of the Most Expensive Book:\n");
    printf("Title: %s\n", mostExpensiveBook.title);
    printf("Author: %s\n", mostExpensiveBook.author);
    printf("Price: %.2f\n", mostExpensiveBook.price);

    return 0;
}