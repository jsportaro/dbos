#include <memory.h>

void *memset(void *ptr, int c, size_t size)
{
    char *cPtr = ptr;

    for (int i = 0; i < size; i++)
    {
        cPtr[i] = c;
    }

    return ptr;
}