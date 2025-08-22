#include <arch/arch.h>
#include <drivers/ata/ata.h>
#include <fs/pathparser.h>
#include <kernel.h>
#include <memory/heap/kheap.h>
#include <terminal.h>

#include <stdint.h>
#include <stddef.h>

void KernelMain(void)
{
    TerminalClear();
    KernelPrint("Database OS ");

    KHeapInit();
    DiskSearchAndInit();

    ArchPrint();
    ArchInit();

    ParsePath("0:/bin/shell.bin", NULL);

    KernelPrint("System ready...\n");
}