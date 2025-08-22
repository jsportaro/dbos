#ifndef __OS_STRING_H__
#define __OS_STRING_H__

#include <stdbool.h>
#include <stddef.h>

size_t strlen(const char *string);
size_t strnlen(const char *string, int max);
bool IsDigit(char c);
int ToNumericDigit(char c);

#endif
