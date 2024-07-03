#include "utils/util.h"
#include "utils/debug/debug.h"
#include "gdt/gdt.h"
#include "idt/idt.h"

static void i0(registers_t reg)
{
    debug_terminal_writestring("BALLS");
}

void kernel_main(void)
{
    init_gdt();
    init_idt();
    debug_terminal_initialize();
    debug_terminal_writestring("Hello, World!");

    while (1)
        ;
}
