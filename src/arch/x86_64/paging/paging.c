#include <arch/x86_64/paging/paging.h>
#include <memory/heap/kheap.h>

static uint32_t *currentDirectory = NULL;

void PagingLoadDirectory(uint32_t *directory);

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