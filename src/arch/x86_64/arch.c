#include <terminal.h>
#include <arch/arch.h>
#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/paging/paging.h>
#include <arch/x86_64/pic/pic.h>
#include <arch/x86_64/paging/paging.h>
#include <memory/heap/kheap.h>
#include <panic.h>

void ArchPrint(void)
{
    KernelPrint("x86_64\n");
}

static PagingChunk *kernelChunk = {0};

void ArchInit(void)
{
    RemapPIC();
    IDTInit();

    //  That last flag might not be right
    kernelChunk = PagingNew(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    PagingSwitch(PagingChunkGetDirectory(kernelChunk));

    char *ptr = KZAlloc(4096);
    int result = PagingSet(PagingChunkGetDirectory(kernelChunk), (void *)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITABLE);

    if (result < 0)
    {
        Panic();
    }

    PagingEnable();

    char *ptr2 = (char *)0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';

    KernelPrint(ptr2);
    KernelPrint(ptr);

    KernelPrint("\nDone\n");
    //  I stuck this in io.asm
    EnableInterrupts();
}