#include "gdt.h"
struct gdt_entry_struct
{
    uint16 limit_low;  // The lower 16 bits of the limit.
    uint16 base_low;   // The lower 16 bits of the base.
    uint8 base_middle; // The next 8 bits of the base.
    uint8 access;      // Access flags, determine what ring this segment can be used in.
    uint8 granularity; // low 4 bits are high 4 bits of limit
    uint8 base_high;   // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;
struct gdt_ptr_struct
{
    uint16 limit; // The upper 16 bits of all selector limits.
    uint32 base;  // The address of the first gdt_entry_t struct.
}
__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

extern void load_gdt(gdt_ptr_t *gdt_ptr);

static void gdt_set_gate(int32, uint32, uint32, uint8, uint8);
gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32)&gdt_entries;
    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data
    load_gdt(&gdt_ptr);
}
static void gdt_set_gate(int32 entry, uint32 base, uint32 limit, uint8 access, uint8 gran)
{
    gdt_entries[entry].base_low = (base & 0xFFFF);
    gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
    gdt_entries[entry].base_high = (base >> 24) & 0xFF;

    gdt_entries[entry].limit_low = (limit & 0xFFFF);
    gdt_entries[entry].granularity = (limit >> 16) & 0x0F;

    gdt_entries[entry].granularity |= gran & 0xF0;
    gdt_entries[entry].access = access;
}