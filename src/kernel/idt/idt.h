#ifndef IDT_H
#define IDT_H

#include "../utils/util.h"

#define DIVISION_BY_ZERO 0
#define DEBUG_EXCEPTION 1
#define NON_MASKABLE_INTERRUPT 2
#define BREAKPOINT_EXCEPTION 3
#define INTO_DETECTED_OVERFLOW 4
#define OUT_OF_BOUNDS_EXCEPTION 5
#define INVALID_OPCODE_EXCEPTION 6
#define NO_COPROCESSOR_EXCEPTION 7
#define DOUBLE_FAULT 8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define BAD_TSS 10
#define SEGMENT_NOT_PRESENT 11
#define STACK_FAULT 12
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT 14
#define UNKNOWN_INTERRUPT_EXCEPTION 15
#define COPROCESSOR_FAULT 16
#define ALIGNMENT_CHECK_EXCEPTION 17
#define MACHINE_CHECK_EXCEPTION 18

void init_idt(void);

typedef struct registers
{
    uint32 ds;                                     // Data segment selector
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32 int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32 eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers_t;

// This intentionally accepts a *COPY* of the registers.
// It's slower, but it prevents service routines from messing with them.
// Maybe messing with them is useful, and we'll change this later.
typedef void (*isr_handler_t)(registers_t);
void register_interrupt_handler(uint8 interrupt, isr_handler_t handler);

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#endif
