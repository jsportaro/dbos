#ifndef __OS_IDT_H__
#define __OS_IDT_H__

#include <stdint.h>

struct IDTDescriptor 
{
    uint16_t offset_lower;
    uint16_t selector;
    uint8_t zero;
    uint8_t typeAttribute;
    uint16_t offset_upper;
} __attribute__((packed));

struct IDTRDescriptor
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void IDTInit(void);

#endif
