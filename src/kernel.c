#include <kernel.h>
#include <idt/idt.h>
#include <support/terminal.h>

#include <stdint.h>
#include <stddef.h>

void KernelMain(void)
{
    TerminalClear();
    KernelPrint("Database OS\n");

    IDTInit();

    KernelPrint("System ready...\n");
}