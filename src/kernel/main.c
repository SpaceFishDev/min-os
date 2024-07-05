#include "utils/util.h"
#include "utils/debug/debug.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit/pit.h"
#include "pic/pic.h"

void kernel_main(void)
{
    debug_terminal_initialize();
    init_gdt();
    remap_pic();
    init_idt();
    init_timer(1, 0);
    while (1)
        ;
}
