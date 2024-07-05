#include "debug.h"
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8 terminal_color;
uint16 *terminal_buffer;

void debug_terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16 *)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void debug_terminal_setcolor(uint8 color)
{
    terminal_color = color;
}

void debug_terminal_putentryat(char c, uint8 color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void debug_terminal_putchar(char c)
{
    if (c != '\n')
    {
        debug_terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH)
        {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
        return;
    }
    ++terminal_row;
    terminal_column = 0;
}

void debug_terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        debug_terminal_putchar(data[i]);
}

void debug_terminal_writestring(const char *data)
{
    debug_terminal_write(data, strlen(data));
}

void debug_terminal_print_num(int x)
{
    while (x > 0)
    {
        int mod = x % 10;
        debug_terminal_putchar(((char)mod) + '0');
        x = x / 10;
    }
}

void PANIC(char *str)
{
    debug_terminal_writestring("PANIC: ");
    debug_terminal_writestring(str);
    debug_terminal_writestring("\n");
    asm("cli");
    asm("hlt");
}