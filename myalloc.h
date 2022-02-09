// Andrew Hepworth

#include <stdio.h>
#include <unistd.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - ((x) - 1) & (ALIGNMENT - 1))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

int myalloc(int size);