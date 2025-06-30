#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

void *malloc(size_t size)
{
    void *ptr = sbrk(0);
    void *req = sbrk(size);
    if(req == (void *) -1) return NULL;
    else
    {
        // the situation ptr = req is probably the result of thread concurrency 
        // (they should not be equal): 
        assert(ptr == req);
        return ptr;
    }
}

int main(void)
{
    char *ptr = malloc(sizeof(char) * sizeof("sword"));
    memcpy(ptr, "sword", sizeof("sword"));
    printf("%s\n", ptr);
}