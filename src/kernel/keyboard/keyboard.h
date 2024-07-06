#ifndef KEYBOARD_H
#define KEYBOARD_H

#define ESC (0x1B)
#define BS (0x08)

int init_keyboard();
char poll_keyboard();

#endif