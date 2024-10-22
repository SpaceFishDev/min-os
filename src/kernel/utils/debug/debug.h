#ifndef DEBUG_H
#define DEBUG_H
#include "../util.h"

enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};
static inline uint8 vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}
static inline uint16 vga_entry(unsigned char uc, uint8 color)
{
    return (uint16)uc | (uint16)color << 8;
}
void debug_terminal_initialize();
void debug_terminal_setcolor(uint8 color);
void debug_terminal_putentryat(char c, uint8 color, size_t x, size_t y);
void debug_terminal_putchar(char c);
void debug_terminal_write(const char *data, size_t size);
void debug_terminal_writestring(const char *data);
void debug_terminal_print_num(int x);
void PANIC(char *str);
#endif