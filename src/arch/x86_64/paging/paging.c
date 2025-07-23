#include <arch/x86_64/paging/paging.h>
#include <memory/heap/kheap.h>
#include <memory/memory.h>
#include <panic.h>
#include <status.h>

static uint32_t *currentDirectory = NULL;

void PagingLoadDirectory(uint32_t *directory);

// kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
PagingChunk *PagingNew(uint8_t flags)
{
    uint32_t *directory = KZAlloc(PAGING_ENTRIES_PER_TABLE * sizeof(uint32_t));
    uint32_t offset = 0;

    for (ptrdiff_t i = 0; i < PAGING_ENTRIES_PER_TABLE; i++)
    {
        uint32_t *entries = KZAlloc(PAGING_ENTRIES_PER_TABLE * sizeof(uint32_t));
        for (ptrdiff_t j = 0; j < PAGING_ENTRIES_PER_TABLE; j++)
        {
            entries[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
        }

        offset += (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);

        directory[i] = (uint32_t)entries | flags | PAGING_IS_WRITABLE;
    }

    PagingChunk *chunk = KZAlloc(sizeof(PagingChunk));
    chunk->entries = directory;

    return chunk;
}

void PagingSwitch(uint32_t *directory)
{
    PagingLoadDirectory(directory);
    currentDirectory = directory;
}

uint32_t *PagingChunkGetDirectory(PagingChunk *pagingChunk)
{
    return pagingChunk->entries;
}

bool PagingIsAligned(void * address)
{
    return FAST_PTR_MOD(address, PAGING_PAGE_SIZE) == 0;
}

int PagingGetIndexes(void *virtualAddress, uint32_t *directoryIndex, uint32_t *tableIndex)
{
    int result = SUCCESS;

    if (!PagingIsAligned(virtualAddress))
    {
        result = -EINVARG;

        goto out;
    }

    *directoryIndex = ((uint32_t)virtualAddress / (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *tableIndex = FAST_PTR_MOD(((uint32_t)virtualAddress), (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)) / PAGING_PAGE_SIZE;

    int check = ((uint32_t)virtualAddress % (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

    if (*tableIndex != check)
    {
        Panic();
    }

out:
    return result;
}

int PagingSet(uint32_t *directory, void *virtualAddress, uint32_t val)
{
    Result result = SUCCESS;
    
    if (!PagingIsAligned(virtualAddress))
    {
        result = -EINVARG;

        goto out;
    }

    uint32_t directoryIndex = 0;
    uint32_t tableIndex = 0;

    result = PagingGetIndexes(virtualAddress, &directoryIndex, &tableIndex);

    if (result != SUCCESS)
    {
        goto out;
    }

    uint32_t entry = directory[directoryIndex];
    uint32_t *table = (uint32_t *)(entry & 0xfffff000);
    table[tableIndex] = val;

out:
    return result;
}