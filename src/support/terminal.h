#ifndef __OS_TERMINAL_H__
#define __OS_TERMINAL_H__

#include <stdint.h>

uint16_t TerminalMakeChar(char c, char color);
void TerminalPutChar(int x, int y, char c, char color);
void TerminalClear(void);
void TerminalWriteChar(char c, char color);
void KernelPrint(const char *string);

#endif
