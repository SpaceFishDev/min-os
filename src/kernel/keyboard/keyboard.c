#include "../utils/util.h"
#include "keyboard.h"
#include "../idt/idt.h"
#include "../utils/debug/debug.h"

const uint8 lower_ascii_codes[256] = {
    0x00, ESC, '1', '2',    /* 0x00 */
    '3', '4', '5', '6',     /* 0x04 */
    '7', '8', '9', '0',     /* 0x08 */
    '-', '=', BS, '\t',     /* 0x0C */
    'q', 'w', 'e', 'r',     /* 0x10 */
    't', 'y', 'u', 'i',     /* 0x14 */
    'o', 'p', '[', ']',     /* 0x18 */
    '\n', 0x00, 'a', 's',   /* 0x1C */
    'd', 'f', 'g', 'h',     /* 0x20 */
    'j', 'k', 'l', ';',     /* 0x24 */
    '\'', '`', 0x00, '\\',  /* 0x28 */
    'z', 'x', 'c', 'v',     /* 0x2C */
    'b', 'n', 'm', ',',     /* 0x30 */
    '.', '/', 0x00, '*',    /* 0x34 */
    0x00, ' ', 0x00, 0x00,  /* 0x38 */
    0x00, 0x00, 0x00, 0x00, /* 0x3C */
    0x00, 0x00, 0x00, 0x00, /* 0x40 */
    0x00, 0x00, 0x00, '7',  /* 0x44 */
    '8', '9', '-', '4',     /* 0x48 */
    '5', '6', '+', '1',     /* 0x4C */
    '2', '3', '0', '.',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00, /* 0x54 */
    0x00, 0x00, 0x00, 0x00  /* 0x58 */
};

const uint8 upper_ascii_codes[256] = {
    0x00, ESC, '!', '@',    /* 0x00 */
    '#', '$', '%', '^',     /* 0x04 */
    '&', '*', '(', ')',     /* 0x08 */
    '_', '+', BS, '\t',     /* 0x0C */
    'Q', 'W', 'E', 'R',     /* 0x10 */
    'T', 'Y', 'U', 'I',     /* 0x14 */
    'O', 'P', '{', '}',     /* 0x18 */
    '\n', 0x00, 'A', 'S',   /* 0x1C */
    'D', 'F', 'G', 'H',     /* 0x20 */
    'J', 'K', 'L', ':',     /* 0x24 */
    '"', '~', 0x00, '|',    /* 0x28 */
    'Z', 'X', 'C', 'V',     /* 0x2C */
    'B', 'N', 'M', '<',     /* 0x30 */
    '>', '?', 0x00, '*',    /* 0x34 */
    0x00, ' ', 0x00, 0x00,  /* 0x38 */
    0x00, 0x00, 0x00, 0x00, /* 0x3C */
    0x00, 0x00, 0x00, 0x00, /* 0x40 */
    0x00, 0x00, 0x00, '7',  /* 0x44 */
    '8', '9', '-', '4',     /* 0x48 */
    '5', '6', '+', '1',     /* 0x4C */
    '2', '3', '0', '.',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00, /* 0x54 */
    0x00, 0x00, 0x00, 0x00  /* 0x58 */
};

bool left_shift = false;
bool right_shift = false;
bool cntrl = false;

uint8 keyboard[256];
uint8 keyboard_buffer[256];
int buf_idx = 0;

void poll_kbd()
{
    uint8 byte = inb(0x60);
    if (byte & 0x80)
    {
        uint8 pressed_byte = byte & 0x7F;
        if (pressed_byte == 0x2A)
        {
            left_shift = false;
        }
        else if (pressed_byte == 0x36)
        {
            right_shift = false;
        }
        else if (pressed_byte == 0x1D)
        {
            cntrl = false;
        }
        keyboard[pressed_byte] = 0;
        return;
    }
    if (byte == 0x2A)
    {
        left_shift = true;
        return;
    }
    else if (byte == 0x36)
    {
        right_shift = true;
        return;
    }
    else if (byte == 0x1D)
    {
        cntrl = 1;
        return;
    }
    keyboard[byte]++;
    if (buf_idx >= 256)
    {
        buf_idx = 0;
    }
    keyboard_buffer[buf_idx] = byte;
    ++buf_idx;
    return;
}

void keyboard_handler(registers_t regs)
{
    poll_kbd();
}

int init_keyboard()
{
    outb(0x64, 0xFF);
    uint8 status = inb(0x64);
    status = inb(0x64);
    outb(0x64, 0xAA);
    uint8 result = inb(0x60);
    if (result != 0x55)
    {
        return -1;
    }
    outb(0x64, 0x20);
    result = inb(0x60);
    register_interrupt_handler(IRQ1, keyboard_handler);
    return 0;
}
bool left_shift_pressed()
{
    return left_shift;
}
bool right_shift_pressed()
{
    return right_shift;
}
bool cntrl_pressed()
{
    return cntrl;
}

uint8 *scancodes_pressed()
{
    return keyboard;
}

char poll_keyboard()
{
    if (!buf_idx)
    {
        return 0;
    }
    char c = keyboard_buffer[buf_idx - 1];
    --buf_idx;
    if (left_shift || right_shift)
    {
        if (upper_ascii_codes[c])
        {
            return upper_ascii_codes[c];
        }
    }
    return lower_ascii_codes[c];
}