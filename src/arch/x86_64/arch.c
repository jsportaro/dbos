#include <terminal.h>
#include <arch/arch.h>
#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/pic/pic.h>

void ArchPrint(void)
{
    KernelPrint("x86_64\n");
}

void ArchInit(void)
{
    RemapPIC();
    IDTInit();
}