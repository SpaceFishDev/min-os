#include "util.h"

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}

unsigned char inb(unsigned short port)
{
    unsigned char ret;
    asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(port) : "memory");
    return ret;
}

void outb(unsigned char value, unsigned short port)
{
    asm volatile("out %%al, %%dx" : : "a"(value), "d"(port) : "memory");
}

void memset(void *dat, char ch, size_t sz)
{
    for (size_t i = 0; i < sz; ++i)
    {
        ((char *)dat)[i] = ch;
    }
}