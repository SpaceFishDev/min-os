#ifndef UTIL_H
#define UTIL_H
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long size_t;
typedef char int8;
typedef short int16;
typedef int int32;

typedef char bool;
#define true 1
#define false 0

size_t strlen(const char *str);
unsigned char inb(unsigned short port);
void outb(unsigned short port, uint8 data);
void memset(void *, char ch, size_t);
__attribute__((__always_inline__, __artificial__, __unused__)) __inline__ static void x86_cli(void)
{
    __asm__ __volatile__("cli" ::: "memory", "cc");
}
__attribute__((__always_inline__, __artificial__, __unused__)) __inline__ static void x86_sti(void)
{
    __asm__ __volatile__("sti" ::: "memory", "cc");
}
#endif