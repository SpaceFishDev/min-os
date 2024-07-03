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
void outb(unsigned char value, unsigned short port);
void memset(char *, char ch, size_t);

#endif