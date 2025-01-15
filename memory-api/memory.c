#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

// NOTES:
//      - brk(x) moves the heap address to the address x but if the last address in the
//      heap is y and y > x then brk(x) returns y (I thinks it's how it works, but it's not explained 
//      anywhere, even in the manual).

typedef struct memory_segment *mem_seg_ptr;
// It's possible to use "*" in tge type definition, then mem_seg represents the type "struct Mem_seg *".

struct memory_segment 
{
    size_t size;
    char next;
    // Every structure, unless it fits inside some size less than 4 (it depends on the architecture),
    // will have a size divisible by 4 (mantaining the order of the fields in the memory, i.e., the
    // structure address is the address of it's first field and so on), and it's organized in a way
    // that using an integer (4 bytes) or a char (1 byte) as flag doesn't change the size of the of
    // the structure.
    int free;
};

void *malloc(size_t size)
{
    void *ptr = sbrk(0);
    void *req = sbrk(size);
    if(req == (void *) -1) return NULL;
    else
    {
        // The situation ptr = req is probably the result of thread concurrency (they should not be equal): 
        assert(ptr == req);
        return ptr;
    }
}

void free(void *ptr);

int main(void)
{
    printf("%i\n", sizeof(void *));
    printf("%i\n", sizeof(mem_seg_ptr));
    struct memory_segment a;
    printf("%i\n", sizeof(a));
    printf("&a.size = %li\n", &a.size);
    printf("&a.next = %li\n", &a.next);
    printf("&a.free = %li\n", &a.free);

    // To use the struct memory_segment it's is possible to save each stryct before de data
    // that it refers to (but it's is needed to know the addresses of each struct field).
    mem_seg_ptr x;
    x = sbrk(0);
    sbrk(sizeof(struct memory_segment) + sizeof(int));
    x->size = sizeof(struct memory_segment) + sizeof(int);
    int *data_addr = (int *) (x + sizeof(struct memory_segment));
    *data_addr = 200;
    printf("stored number = %i\n", *data_addr); 
    return 0;
}

// int main(void)
// {
//     void *ptr;
//     printf("ptr = %i \n", sbrk(0));
//     int a = brk(NULL);
//     printf("a = %i \n", a);
//     printf("ptr = %i \n", sbrk(0));
//     // sbrk(1);
//     // a = brk(NULL);
//     // ptr = sbrk(0);
//     // printf("a = %i \n", a);
//     // printf("ptr = %i \n", ptr);
//     // a = brk(ptr + 1);
//     // ptr = sbrk(0);
//     // printf("a = %i \n", a);
//     // printf("ptr = %i \n", ptr);
//     // a = brk(ptr - 1);
//     a = brk((void *) 2);
//     ptr = sbrk(-5);
//     printf("errno = %i \n", errno);
//     printf("a = %i \n", a);
//     printf("errno = %i \n", errno);
//     printf("ptr = %i \n", ptr);
//     // printf("*(ptr + 4) = %i \n", *((int *) ptr - 1));
//     // printf("*(ptr - 1) = %c \n", *((char *) ptr - 1));
//     // printf("*ptr = %i \n", *((int *) ptr + 1));
// }