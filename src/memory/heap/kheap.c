#include <panic.h>
#include <memory/heap/heap.h>
#include <memory/heap/kheap.h>
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

void KFree(void *ptr)
{
    HeapFree(&KernelHeap, ptr);
}