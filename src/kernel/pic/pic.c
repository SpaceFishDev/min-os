#include "pic.h"
#include "../utils/util.h"
#include "../utils/debug/debug.h"

#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21
#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA 0xA1

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

unsigned int io_wait()
{
    int j = 0;
    for (unsigned int i = 0; i < 0xFFFFFFFF; i++)
    {
        j = i;
    }
    return j;
}
#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20 /* End-of-interrupt command code */
void PIC_sendEOI(uint8 irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);

    outb(PIC1_COMMAND, PIC_EOI);
}

void remap_pic(void)
{

    outb(PIC1_COMMAND, 0x11); // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, 0x11);
    io_wait();
    outb(0x21, 0x20); // ICW2: Master PIC vector offset
    io_wait();
    outb(0xA1, 0x28); // ICW2: Slave PIC vector offset
    io_wait();
    outb(0x21, 4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(0xA1, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(0x21, 0xfd);
    io_wait();
    outb(0xA1, 0xfd);
    io_wait();
}