#include <arch/x86_64/pic/pic.h>
#include <arch/x86_64/support/io.h>

#define PIC1            0x20		/* IO base address for master PIC */
#define PIC2            0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)

void RemapPIC(void)
{
    outb(PIC1, 0b00010001);
    outb(PIC1_DATA, 0x20);
    outb(PIC1_DATA, 0x01);
}

void IntAck(void)
{
    outb(PIC1, 0x20);
}