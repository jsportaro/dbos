#ifndef __OS_MEMORY_H__
#define __OS_MEMORY_H__

#include <stddef.h>

#define FAST_PTR_MOD(n, d) ((ptrdiff_t)(n) & ((d) - 1))

void *memset(void *ptr, int c, size_t size);

#endif
