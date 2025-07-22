#include <panic.h>
#include <memory/heap/heap.h>
#include <memory/heap/kheap.h>
#include <memory/memory.h>
#include <terminal.h>

Heap KernelHeap = {0};
HeapTable KernelHeapTable = {0};

void KHeapInit(void)
{
    int totalTableEntries = OS_HEAP_SIZE_BYTES / OS_HEAP_BLOCK_SIZE;
    KernelHeapTable.entries = (HeapBlockTableEntry *)(OS_HEAP_TABLE_ADDRESS);
    KernelHeapTable.total = totalTableEntries;

    void *end = (void *)(OS_HEAP_ADDRESS + OS_HEAP_SIZE_BYTES);
    int result = HeapCreate(&KernelHeap, (void *)OS_HEAP_ADDRESS, end, &KernelHeapTable, OS_HEAP_BLOCK_SIZE);

    if (result != 0)
    {
        KernelPrint("Failed to create heap\n");
        Panic();
    }
}

void *KMalloc(size_t size)
{
    return HeapMalloc(&KernelHeap, size);
}

void *KZAlloc(size_t size)
{
    void *ptr = KMalloc(size);

    if (ptr == NULL)
    {
        return NULL;
    }

    memset(ptr, 0x00, size);

    return ptr;
}

void KFree(void *ptr)
{
    HeapFree(&KernelHeap, ptr);
}