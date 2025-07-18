
#include <memory/heap/heap.h>
#include <memory/memory.h>
#include <panic.h>

#include <stdbool.h>

#define FAST_PTR_MOD(n, d) ((ptrdiff_t)(n) & ((d) - 1))

static Result HeapValidateTable(void *ptr, void *end, size_t blockSize, HeapTable *heapTable)
{
    Result result = SUCCESS;

    ptrdiff_t tableSize = end - ptr;
    size_t totalBlocks = tableSize / blockSize;

    if (heapTable->total != totalBlocks)
    {
        result = -EINVARG;

        goto out;
    }
out:
    return result;
}

static bool HeapValidateAlignment(void *ptr, size_t blockSize)
{
    return FAST_PTR_MOD(ptr, blockSize) == 0;
}

static uint32_t HeapAlignValueToUpper(uint32_t val, size_t blockSize)
{
    ptrdiff_t rem = FAST_PTR_MOD(val, blockSize);

    if (rem == 0)
    {
        return val;
    }

    val = (val - rem);
    val += blockSize;

    return val;
}

static int HeapGetEntryType(HeapBlockTableEntry entry)
{
    return entry & 0x0f;
}

static int HeapGetStartBlock(Heap *heap, uint32_t totalBlocks)
{
    HeapTable *heapTable = heap->heapTable;

    int blockCurrent = 0;
    int blockStart = -1;

    for (ptrdiff_t i = 0; i < heapTable->total; i++)
    {
        if (HeapGetEntryType(heap->heapTable->entries[i] != HEAP_BLOCK_TABLE_ENTRY_FREE))
        {
            blockCurrent = 0;
            blockStart = -1;
            
            continue;
        }

        if (blockStart == -1)
        {
            blockStart = i;
        }

        blockCurrent++;

        if (blockCurrent == totalBlocks)
        {
            break;
        }
    }

    if (blockStart == -1)
    {
        return -ENOMEM;
    }

    return blockStart;
}

static void *HeapBlockToAddress(Heap *heap, uint32_t block)
{
    return heap->startAddress + (block * heap->blockSize);
}

static uint32_t HeapAddressToBlock(Heap *heap, void *ptr)
{
    return (ptr - heap->startAddress) / heap->blockSize;
}

void HeapMarkBlocksTaken(Heap *heap, uint32_t startBlock, uint32_t totalBlocks)
{
    int endBlock = (startBlock + totalBlocks) - 1;

    HeapBlockTableEntry entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;

    if (totalBlocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for (ptrdiff_t i = 0; i <= endBlock; i++)
    {
        heap->heapTable->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != endBlock - 1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

Result HeapCreate(Heap *heap, void *start, void *end, HeapTable *heapTable, size_t blockSize)
{
    Result result = 0;

    if (!HeapValidateAlignment(start, blockSize) || !HeapValidateAlignment(end, blockSize))
    {
        return -EINVARG;

        goto out;
    }

    memset(heap, 0, sizeof(Heap));
    heap->startAddress = start;
    heap->heapTable= heapTable;
    heap->blockSize = blockSize;

    result = HeapValidateTable(start, end, blockSize, heapTable);

    if (result < 0)
    {
        goto out;
    }
    size_t tableSize = sizeof(HeapBlockTableEntry) * heapTable->total;
    memset(heapTable->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, tableSize);

out:
    return result;
}

void *HeapMallocBlocks(Heap *heap, uint32_t totalBlocks)
{
    void *address = 0;

    int startBlock = HeapGetStartBlock(heap, totalBlocks);
    if (startBlock < 0)
    {
        goto out;
    }

    address = HeapBlockToAddress(heap, startBlock);
    HeapMarkBlocksTaken(heap, startBlock, totalBlocks);

out:
    return address;
}

void HeapMarkBlocksFree(Heap *heap, uint32_t startBlock)
{
    HeapBlockTableEntry entry = heap->heapTable->entries[startBlock];

    if ((entry & HEAP_BLOCK_IS_FIRST) == 0)
    {
        Panic();
    }

    for (ptrdiff_t i = startBlock; i < heap->heapTable->total; i++)
    {
        HeapBlockTableEntry entry = heap->heapTable->entries[i];
        heap->heapTable->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;

        if (!(entry & HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}

void *HeapMalloc(Heap *heap, size_t size)
{
    size_t alignedSize = HeapAlignValueToUpper(size, heap->blockSize);
    uint32_t totalBlocks = alignedSize / heap->blockSize;

    return HeapMallocBlocks(heap, totalBlocks);
}

void HeapFree(Heap *heap, void *ptr)
{
    int block = HeapAddressToBlock(heap, ptr);
    HeapMarkBlocksFree(heap, block);
}