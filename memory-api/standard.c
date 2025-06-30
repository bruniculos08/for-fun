#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

// int main(void)
// {
//     char *ptr1 = calloc(1, sizeof(char));
//     int *ptr2 = malloc(sizeof(int));
//     printf("ptr1 = %i\n", ptr1);
//     printf("ptr2 = %i\n", ptr2);
//     printf("ptr2 - ptr1 = %i\n", (((int)ptr2) - ((int)ptr1)));
//     printf("ptr1[1000] = %c\n", ptr1[1000]);
//     ptr1[1000] = 'a';
//     printf("ptr1[1000] = %c\n", ptr1[1000]);
// }

//      f(x) = x²
//      f : R -> R 
//      tipo nome (tipo arg1, tipo arg2, ...)
//      {
//          (corpo)
//      }

// int -> 32 bits -> bit de sinal + 2^(31)
// float -> 32 bits
// double -> 64 bits
// long double -> 128 bits
// char -> 8 bits (2^8)

int main()
{
    // int batata = 2, banana = 5;
    // printf("%i %i\n", banana, batata);
    // scanf("%i %i", &banana, &batata);
    // printf("%i %i\n\n", banana, batata);

    // printf("%i %i\n", banana, batata);

    // int *addr;
    // int num = 20000001;
    // addr = &num;
    // printf("%p\n", addr);
    // printf("%p\n", (addr + 1));
    // printf("%p\n", *addr);

    char *a;
    char b = 'x';
    a = &b;
    printf("%i\n", a);
    printf("%i\n", a + 1);
    printf("%c\n", a);

    // printf("%i\n", sizeof(int *));
    // printf("%i\n", sizeof(int));



}