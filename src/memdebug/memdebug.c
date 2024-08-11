#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int line;
    char *file;
    void *ptr;
    bool free;
} allocation_t;

allocation_t *allocations;
int sz_allocations = 0;
int alloc_idx = 0;
void grow_allocations()
{
    allocations = realloc(allocations, sz_allocations * 2 * sizeof(allocation_t));
    sz_allocations *= 2;
}

void init_memdebug()
{
    sz_allocations = 1;
    allocations = malloc(sz_allocations * sizeof(allocation_t));
}

void cleanup_memdebug()
{
    sz_allocations = 0;
    alloc_idx = 0;
    free(allocations);
}

void memdebug_free(void *ptr)
{
    free(ptr);
    for (int i = 0; i < alloc_idx; ++i)
    {
        if (allocations[i].ptr = ptr)
        {
            allocations[i].free = true;
        }
    }
}

void *memdebug_malloc(int sz, char *file, int line)
{
    void *ptr = malloc(sz);
    allocation_t alloc = (allocation_t){.file = file, .free = false, .line = line, .ptr = ptr};
    if (alloc_idx >= sz_allocations)
    {
        grow_allocations();
    }
    allocations[alloc_idx] = alloc;
    ++alloc_idx;
    return ptr;
}

void memdebug_view()
{
    for (int i = 0; i < alloc_idx; ++i)
    {
        if (allocations[i].free == false)
        {
            printf("NON-FREE MALLOC: [FILE: %s LINE: %d]\n", allocations[i].file, allocations[i].line);
        }
    }
}