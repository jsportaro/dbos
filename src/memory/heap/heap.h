#ifndef __OS_HEAP_H__
#define __OS_HEAP_H__

#include <status.h>

#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE  0x00

#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000

typedef unsigned char HeapBlockTableEntry;

typedef struct HeapTable
{
    HeapBlockTableEntry *entries;
    size_t total;
} HeapTable;

typedef struct Heap
{
    HeapTable *heapTable;
    void *startAddress;
    size_t blockSize;
} Heap;

Result HeapCreate(Heap *heap, void *start, void *end, HeapTable *table, size_t blockSize);
void *HeapMalloc(Heap *heap, size_t size);
void HeapFree(Heap *heap, void *ptr);

#endif
