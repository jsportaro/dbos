#ifndef __OS_KHEAP_H__
#define __OS_KHEAP_H__

#include <stdint.h>
#include <stddef.h>

//  100 MB of heap 
//  Anyone running this need at least that much RAM
//  TODO: determine how much RAM is installed and adjust
//        these two accordingly
#define OS_HEAP_SIZE_BYTES 1048576
#define OS_HEAP_BLOCK_SIZE 4096

//  Why this magic number?  Check out
//  https://wiki.osdev.org/Memory_Map_(x86) in the Extended 
//  Memory section - we already put our OS at 0x00100000
//  This is the next free (and potentially large) section of  memory
#define OS_HEAP_ADDRESS 0x01000000 

//  Again, looking at OS Dev we see at 0x00007E00 we have 
//  480 KB of space to put things.  We'll put the table 
//  in that area since we only need (OS_HEAP_SIZE_BYTES / OS_HEAP_SIZE_BYTES)
//  bytes for the table.  Heck, we could put 480000 entries in that space.  
//  Plenty big for the default of 256
#define OS_HEAP_TABLE_ADDRESS 0x00007E00

void KHeapInit(void);
void *KMalloc(size_t size);
void *KZAlloc(size_t size);
void KFree(void *ptr);

#endif
