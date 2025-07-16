#include <arch/arch.h>
#include <kernel.h>
#include <terminal.h>

#include <stdint.h>
#include <stddef.h>

void KernelMain(void)
{
    TerminalClear();
    KernelPrint("Database OS ");

    ArchPrint();
    ArchInit();

    KernelPrint("System ready...\n");
}