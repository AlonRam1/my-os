#include <kernel/string/string.h>


int streq(const char* a, const char* b)
{
    while(*a || *b)
    {
        if(*a != *b)
            return 0;

        a++;
        b++;
    }

    return 1;
}


void strcopy(char* dst, const char* src)
{
    while(*src)
    {
        *dst++ = *src++;
    }

    *dst = 0;
}
