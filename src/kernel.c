#include <kernel.h>
#include <terminal.h>
#include <arch/arch.h>
#include <memory/heap/kheap.h>

#include <stdint.h>
#include <stddef.h>

void KernelMain(void)
{
    TerminalClear();
    KernelPrint("Database OS ");

    KHeapInit();
    
    ArchPrint();
    ArchInit();

    KernelPrint("System ready...\n");
}