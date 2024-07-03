#include "utils/util.h"
#include "utils/debug/debug.h"
#include "gdt/gdt.h"

void kernel_main(void)
{
    init_gdt();
    debug_terminal_initialize();
    debug_terminal_writestring("Hello, World!");

    while (1)
        ;
}
