#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#define swap(type, foo, bar) ({type tmp; tmp=foo; foo=bar; bar=tmp;})

#define align4(x) (((((x)-1)>>2)<<2)+4)

// NOTES:
//      - brk(x) moves the heap address to the address x but if the last address in the
//      heap is y and y > x then brk(x) returns y (I thinks it's how it works, but it's not explained 
//      anywhere, even in the manual).

typedef struct memory_segment *mem_seg_ptr;
// It's possible to use "*" in tge type definition, then mem_seg represents the type "struct memory_segment *".

struct memory_segment 
{
    size_t size;
    mem_seg_ptr next;
    mem_seg_ptr prev;
    // Every structure, unless it fits inside some size less than 4 (it depends on the architecture),
    // will have a size divisible by 4 (mantaining the order of the fields in the memory, i.e., the
    // structure address is the address of it's first field and so on), and it's organized in a way
    // that using an integer (4 bytes) or a char (1 byte) as flag doesn't change the size of the of
    // the structure.
    int free;
    void *ptr;
    char data[1];
};
// I found a better explanation about the memory alignment done by the C compiler in:
// https://levelup.gitconnected.com/how-struct-memory-alignment-works-in-c-3ee897697236
// Following what is tells the compiler aligns the structure by the biggest field size, which is 8 in this case,
// then, if there are a list of field in sequence which the size of each of them added togheter sums up lesse then 8
// they will be put in a contiguous memory slot with 8 bytes and x bytes of padding (x + the sum = 8):
// 
// [   data_1    |    data_2   | ... | data_n  |    padding   |    data_n+1   | ... | data_n+m  ]
// 
// The padding size position is choosen in a way that the aligment in this piece of memory is the biggest size of data
// and then, breaking it in more blocks like:
// 
// [   data_1    |    data_2   | ... |   data_n  ]
// 
// the same rule will be followed to this and others smaller blocks.

// #define MEM_SEG_SIZE sizeof(struct memory_segment)
// Because the field data points to beggining of that and then it is really a part of the meta data 
#define MEM_SEG_SIZE 40

// This will be the pointer to the first block (starts with null, so it's possible to know when the first
// segment is created):
void *base = NULL;

// void *malloc(size_t size)
// {
//     void *ptr = sbrk(0);
//     void *req = sbrk(size);
//     if(req == (void *) -1) return NULL;
//     else
//     {
//         // The situation ptr = req is probably the result of thread concurrency (they should not be equal): 
//         assert(ptr == req);
//         return ptr;
//     }
// }

mem_seg_ptr findSeg(mem_seg_ptr *last, size_t size)
{
    mem_seg_ptr b = base;
    while (b && !(b->free && b->size >= size))
    {
        *last = b;
        b = b->next;
    }
    return b;
}

mem_seg_ptr extendHeap(mem_seg_ptr last, size_t data_size)
{
    mem_seg_ptr m;
    m = sbrk(0);
    if(sbrk(MEM_SEG_SIZE + data_size) == (void *) -1)
        return NULL;
    m->free = 0;
    m->size = data_size;
    m->next = NULL;
    m->ptr = (void *) m->data;
    if(last != NULL)
        last->next = m;
    return m; 
}

void splitSeg(mem_seg_ptr a, size_t size)
{
    if(a->size - size >= sizeof(struct memory_segment))
    {
        mem_seg_ptr b;
        b = (mem_seg_ptr) (a->ptr + size);
        b->size = a->size - size;
        a->size = size;
        b->next = a->next;
        a->next = b;
        b->free = 1;
        b->ptr = (void *) b->data;
    }
}

void *malloc(size_t size)
{
    size_t aligned_size = align4(size);
    // If it's the first time using malloc:
    if(!base)
    {
        mem_seg_ptr a = extendHeap(NULL, aligned_size);
        base = (void *) a;
        return (a == NULL) ? NULL : (void *) (a->data);
    }
    // Else find a free block:
    mem_seg_ptr a;
    mem_seg_ptr last = base;
    a = findSeg(&last, aligned_size);
    // If a block was found it means the block found has a size bigger than MEM_SEG_SIZE + aligned_size:
    if(a)
    {
        // Because the real size of the struct mem_seg is 32 (MEM_SEG_SIZE + 4) it's necessary the block
        // found to have a minimum size that fits the new struct mem_seg created when spliting:
        // if(a->size > MEM_SEG_SIZE + 4)
        if(a->size > sizeof(struct memory_segment))
            splitSeg(a, aligned_size);
    }
    // Else no free block was found, so it's needed to extend the heap:
    else
        a = extendHeap(last, aligned_size);
    return (a == NULL) ? NULL : (void *) (a->data);
}

void *calloc(size_t num, size_t size)
{
    int *ptr = (int *) malloc(num * size);
    size_t aligned_size = align4(num * size);
    if(ptr)
    {
        for(size_t i = 0; i < aligned_size >> 2; i++)
        {
            *(ptr + i) = 0;
        }
    }
    return ((void *) ptr);
}

void fusion(mem_seg_ptr fst)
{
    mem_seg_ptr snd = fst->next;
    if(snd)
    {
        fst->size += MEM_SEG_SIZE + snd->size;
        fst->next = snd->next;
        if(snd->next)
            snd->next->prev = fst;
    }
}

// void defragmentation()
// {
//     mem_seg_ptr b = base;
//     while (b != NULL)
//     {
//         if(b->next != NULL && b->free && b->next->free)
//             fusion(b, b->next);
//         else
//             b = b->next;
//     }
// }

// The following function suposes that the parameter ptr is a pointer to the data:
void *getMetaData(void *ptr)
{
    char *tmp; 
    tmp = ptr;
    return ((void *) (tmp - MEM_SEG_SIZE));
}

int validateAddr(void *p)
{
    if(base)
    {
        if((base < p) && (p < sbrk(0)))
        {
            // The pointer must be equal to the pointer inside it's respective metada:
            return (p == (((mem_seg_ptr) getMetaData(p))->ptr));
        }
    }
    return 0;
}

void free(void *ptr)
{
    if(validateAddr(ptr))
    {
        mem_seg_ptr meta_data = (mem_seg_ptr) getMetaData(ptr);
        meta_data->free = 1;
        // If the previous mem_seg exists and is free:
        if(meta_data->prev && meta_data->prev->free)
        {
            mem_seg_ptr prev_mem = meta_data->prev; 
            fusion(prev_mem);
            meta_data = prev_mem;
        }
        // If the next mem_seg exists and is free:
        if(meta_data->next && meta_data->next->free)
            fusion(meta_data);
        // Else if the next mem_seg don't exist:
        else if(meta_data->next == NULL)
        {
            if(meta_data->prev)
            {
                meta_data->prev->next = NULL;
                // The top of the stack is exactly in the meta_data address because this is where ends the last unfree mem_seg:
                brk((void *) meta_data);
            }
            // If the previous and the next mem_seg don't exist then the base
            // becomes what it was at the beggining:
            else
                base = NULL;
            
        }
        // Else only fix the pointers of the previous and next block:
        else
        {
            if(meta_data->prev) meta_data->prev->next = meta_data->next;
            if(meta_data->next) meta_data->next->prev = meta_data->prev;
        }
    }
}

void copyMemSeg(mem_seg_ptr source, mem_seg_ptr dest)
{
    if(dest->size >= source->size)
    {
        dest->size = source->size;
        int *data_source, *data_dest;
        data_source = (int *) source->ptr;
        data_dest = (int *) dest->ptr;
        for(size_t i = 0; i * 4 < source->size; i++)
        {
            data_dest[i] = data_source[i];
        }
    }
}

void *realloc(void *ptr, size_t size)
{
    size_t aligned_size = align4(size);
    if(ptr && validateAddr(ptr))
    {
        mem_seg_ptr meta_data = (mem_seg_ptr) getMetaData(ptr);
        if(meta_data->size >= aligned_size)
        {
            // The function splitSeg() verifies if it's possible to split:
            splitSeg(meta_data, aligned_size);
            return meta_data->ptr;
        }
        else
        {
            // If the next memory segment has enough size and is free:
            if(meta_data->next && meta_data->next->free && (meta_data->size + MEM_SEG_SIZE + meta_data->next->size) >= aligned_size)
            {
                fusion(meta_data);
                if(meta_data->size >= aligned_size)
                    splitSeg(meta_data, aligned_size);
                return meta_data->ptr;
            }
            // Else it's necessary to do a malloc:
            else
            {
                void *ptr_copy = malloc(size);
                copyMemSeg(meta_data, (mem_seg_ptr) getMetaData(ptr_copy));
                free(ptr);
                return ptr_copy;
            }
        }
    }
    if(!ptr)
        return malloc(size);
    return NULL;
}

int printf(const char *str, ...)
{
    va_list pa;
    int flag;

    va_start(pa, str);

    int size = strlen(str);
    int count = 100;
    char *buffer = malloc(sizeof(char) * size);
    for(size_t i = 0; i < size; i++)
    {
        if(str[i] == '%')
        {
            if(str[i+1] == 'c')
            {
                buffer[i] = (char) va_arg(pa, int);
                i++;
            }
            continue;
        }
        buffer[i] = str[i];
    }
    write(1, buffer, sizeof(char) * size);

    // char buffer[5] = "test";
    // write(1, buffer, sizeof(char) * 5);

    // int buffer[5] = {100,10,40,30,20};
    // write(1, buffer, sizeof(int) * 5);
    
    va_end(pa);   
}

int main(void)
{
    // int a = 0, b = 1;
    // swap(int, a, b);
    // printf("a = %i, b = %i\n", a, b);
    
    printf("test %c\n", 'x');
    return 0;
    // printf("%i\n", sizeof(void *));
    // printf("%i\n", sizeof(size_t));
    // printf("%i\n", sizeof(int));
    // printf("%i\n", sizeof(mem_seg_ptr));
    // struct memory_segment a;
    // printf("%i\n", sizeof(a));
    // printf("sizeof(struct memory_segment) = %i\n", sizeof(struct memory_segment));
    // printf("&a = %li\n", &a);
    // printf("&a + MEM_SEG_SIZE = %li\n", ((char *) &a) + MEM_SEG_SIZE);
    // printf("&a.size = %li\n", &a.size);
    // printf("&a.next = %li\n", &a.next);
    // printf("&a.prev = %li\n", &a.prev);
    // printf("&a.free = %li\n", &a.free);
    // printf("&a.ptr = %li\n", &a.ptr);
    // printf("a.data = %li\n", a.data);
    // printf("&a.data = %li\n", &a.data);
    // printf("(((char *) &a) - a.data) = %li\n", (((char *) &a) - a.data));
    // printf("%i\n", 2 << 1);
    // return 0;

    int *ptr1 = calloc(2, sizeof(int));
    ptr1[0] = 1001;
    ptr1[1] = 1000;
    int *tmp = ptr1;
    ptr1 = realloc(ptr1, sizeof(int) * 5);
    printf("tmp = %i\n", tmp);
    printf("ptr1 = %i\n", ptr1);
    printf("ptr1[0] = %i\n", ptr1[0]);
    printf("ptr1[1] = %i\n", ptr1[1]);
    // return 0;
    size_t *ptr2 = malloc(sizeof(size_t));
    free(ptr1);
    ptr1 = calloc(1, sizeof(int));

    printf("ptr1 = %i\n", ptr1);
    printf("ptr2 = %i\n", ptr2);
    printf("tmp = %i\n", tmp);
    printf("ptr2 - ptr1 = %i\n", ((int)ptr2) - ((int)ptr1));
    printf("ptr1[0] = %i\n", ptr1[0]);
    // Note that the following line prints the first field (size_t size) of the meta data related to ptr2:
    printf("ptr1[1] = %i\n", ptr1[1]);
    printf("&ptr1[1] = %i\n", &ptr1[1]);
    void *ptr = getMetaData((void *) ptr1);
    printf("ptr = %i\n", ptr);
    printf("ptr->data = %i\n", ((mem_seg_ptr) ptr)->data);
    

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