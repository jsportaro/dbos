#include <terminal.h>

#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t *videoMemory = (uint16_t *)(0xb8000);

uint16_t TerminalMakeChar(char c, char color)
{
    return (color << 8) | c;
}

void TerminalPutChar(int x, int y, char c, char color)
{
    videoMemory[(y * VGA_WIDTH) + x] = TerminalMakeChar(c, color);
}

void TerminalClear(void)
{
    for (uint16_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        videoMemory[i] = TerminalMakeChar(' ', 0);
    }
}

static uint16_t terminalRow = 0;
static uint16_t terminalCol = 0;

void TerminalWriteChar(char c, char color)
{
    if (c == '\n')
    {
        terminalCol = 0;
        terminalRow += 1;
        return;
    }

    TerminalPutChar(terminalCol, terminalRow, c, color);
    terminalCol += 1;
    
    if (terminalCol >= VGA_WIDTH)
    {
        terminalCol = 0;
        terminalRow += 1;
    }
}

static size_t strlen(const char *string)
{
    size_t length = 0;

    while (string[length])
    {
        length++;
    }

    return length;
}

void KernelPrint(const char *string)
{
    size_t length = strlen(string);

    for (int i = 0; i < length; i++)
    {
        TerminalWriteChar(string[i], 15);
    }
}
