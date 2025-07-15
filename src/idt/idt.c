#include <idt.h>
#include <config.h>
#include <memory.h>
#include <../support/terminal.h>

struct IDTDescriptor IDTDescriptors[OS_TOTAL_INTERRUPTS];
struct IDTRDescriptor IDTRDescriptor;

extern void IDTLoad(struct IDTRDescriptor *ptr);

void IDTZero(void)
{
    KernelPrint("Divide by zero error\n");
}

void IDTSet(int interruptNumber, void *address)
{
    struct IDTDescriptor *descriptor = &IDTDescriptors[interruptNumber];

    descriptor->offset_lower = (uint32_t)address & 0x0000ffff;
    descriptor->selector = KERNEL_CODE_SELECTOR;
    descriptor->zero = 0x00;
    descriptor->typeAttribute = 0xEE;
    descriptor->offset_upper = (uint32_t)address >> 16;
}

void IDTInit(void)
{
    memset(IDTDescriptors, 0, sizeof(IDTDescriptors));

    IDTRDescriptor.limit = sizeof(IDTDescriptors) - 1;
    IDTRDescriptor.base = (uint32_t)IDTDescriptors;

    IDTSet(0, IDTZero);

    IDTLoad(&IDTRDescriptor);
}