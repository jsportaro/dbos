#include "kernel.h"

static uint16_t *videoMemory = (uint16_t *)(0xb8000);

static uint16_t TerminalMakeChar(char c, char color)
{
    return color << 8 | c;
}

static void ClearTerminal(void)
{
    for (uint16_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        videoMemory[i] = TerminalMakeChar('A', 4);
    }
}

void KernelMain(void)
{
    ClearTerminal();
}