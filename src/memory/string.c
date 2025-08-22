#include <memory/string.h>

size_t strlen(const char *string)
{
    size_t l = 0;

    while (*string++)
        l++;

    return l;
}

size_t strnlen(const char *string, int max)
{
    size_t l = 0;

    for (l = 0; l < max; l++)
    {
        if (string[l] == 0)
            break;
    }

    return l;
}

int ToNumericDigit(char c)
{
    return c - 48;
}

bool IsDigit(char c)
{
    return c >= 48 && c <= 57;
}