#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/config.h>
#include <arch/x86_64/pic/pic.h>
#include <memory/memory.h>
#include <terminal.h>

struct IDTDescriptor IDTDescriptors[OS_TOTAL_INTERRUPTS];
struct IDTRDescriptor IDTRDescriptor;

extern void IDTEnableInterrupts(void);
extern void IDTDisableInterrupts(void);
extern void IDTLoad(struct IDTRDescriptor *ptr);
extern void Int21h(void);
extern void NoInterrupt(void);

void NoInterruptHandler(void)
{
    IntAck();
}

void Int21hHandler(void)
{
    KernelPrint("Keyboard pressed\n");
    IntAck();
}

static void IDTSet(int interruptNumber, void *address)
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

    for (int i = 0; i < OS_TOTAL_INTERRUPTS; i++)
    {
        IDTSet(i, NoInterrupt);
    }

    IDTSet(0x21, Int21h);

    IDTLoad(&IDTRDescriptor);
    IDTEnableInterrupts();
}