#include "pit.h"
#include "../idt/idt.h"

static void timer_test(registers_t registers)
{
    debug_terminal_writestring("timer test\n");
}

void init_timer(uint32 hz, void *callback)
{
    debug_terminal_writestring("Initializing PIT\n");
    register_interrupt_handler(IRQ0, &timer_test);
    uint32 divisor;
    if (hz)
    {
        divisor = PIT_NATURAL_FREQ / hz;
    }
    else
    {
        divisor = 0;
    }
    outb(PIT_COMMAND, 0x36);
    uint8 low = (uint8)(divisor & 0xFF);
    uint8 high = (uint8)((divisor >> 8) & 0xFF);
    outb(PIT_DATA0, low);
    outb(PIT_DATA0, high);
}