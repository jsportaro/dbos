#include <terminal.h>
#include <arch/arch.h>
#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/paging/paging.h>
#include <arch/x86_64/pic/pic.h>
#include <arch/x86_64/paging/paging.h>

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
    PagingEnable();
    
    //  I stuck this in io.asm
    EnableInterrupts();
}