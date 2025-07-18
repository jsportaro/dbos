#ifndef __OS_PANIC_H__
#define __OS_PANIC_H__

#include <terminal.h>

#include <stdbool.h>

static inline void Panic(void)
{
    KernelPrint("PANIC!\n");
    while (true)
    {

    }
}

#endif
