#include "idt.h"
#include "../utils/debug/debug.h"
#include "../pic/pic.h"
struct idt_entry_struct
{
    uint16 base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16 sel;     // Kernel segment selector.
    uint8 always0;  // This must always be zero.
    uint8 flags;    // More flags. See documentation.
    uint16 base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
    uint16 limit;
    uint32 base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQ Handlers
extern void
irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void load_idt(idt_ptr_t *);
static void idt_set_gate(uint8, uint32, uint16, uint8);

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

void init_idt()
{
    debug_terminal_writestring("Initializing IDT\n");
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    idt_set_gate(0, (uint32)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32)isr31, 0x08, 0x8E);

    // IRQ entries
    idt_set_gate(32, (uint32)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32)irq15, 0x08, 0x8E);

    debug_terminal_writestring("Loading IDT.\n");
    load_idt(&idt_ptr);
    asm("sti");
}

static void idt_set_gate(uint8 num, uint32 base, uint16 sel, uint8 flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags /* | 0x60 */;
}

extern void halt();

isr_handler_t interrupt_handlers[256];

void register_interrupt_handler(uint8 interrupt, isr_handler_t handler)
{
    debug_terminal_writestring("Registering interrupt!\n");
    interrupt_handlers[interrupt] = handler;
}

void isr_handler(registers_t regs)
{
    if (regs.int_no == GENERAL_PROTECTION_FAULT)
    {
        debug_terminal_writestring("ISR: ");
        debug_terminal_print_num(regs.int_no);
        debug_terminal_writestring("\n");
        debug_terminal_writestring("ERR: ");
        debug_terminal_print_num(regs.err_code);
        debug_terminal_writestring("\n");
        asm("cli");
        while (1)
            ;
    }
    if (interrupt_handlers[regs.int_no])
    {
        interrupt_handlers[regs.int_no](regs);
    }
    else
    {
        debug_terminal_writestring("UNHANDLED INTERRUPT: ");
        debug_terminal_print_num(regs.int_no);
        debug_terminal_writestring("\n");
    }
    PIC_sendEOI(regs.int_no);
}

void irq_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no])
    {
        interrupt_handlers[regs.int_no](regs);
    }
    PIC_sendEOI(regs.int_no);
}