#ifndef __OS_PAGING_H__
#define __OS_PAGING_H__

#include <stddef.h>
#include <stdint.h>

#include <stdbool.h>

#define PAGING_CACHE_DISABLED  0b00010000
#define PAGING_WRITE_THROUGH   0B00001000
#define PAGING_ACCESS_FROM_ALL 0b00000100
#define PAGING_IS_WRITABLE     0b00000010
#define PAGING_IS_PRESENT      0b00000001

#define PAGING_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE         4096

typedef struct PagingChunk
{
    uint32_t *entries;
} PagingChunk;

PagingChunk *PagingNew(uint8_t flags);
void PagingSwitch(uint32_t *directory);
uint32_t *PagingChunkGetDirectory(PagingChunk *pagingChunk);
void PagingEnable(void);

int PagingGetIndexes(void *virtualAddress, uint32_t *directoryIndex, uint32_t *tableIndex);
int PagingSet(uint32_t *directory, void *virtualAddress, uint32_t val);

#endif
