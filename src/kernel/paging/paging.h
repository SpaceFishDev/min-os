#ifndef PAGING_H

#define PAGING_H

#include "../utils/util.h"

#define INITIAL_HEAP_PAGE_COUNT 16

uint32 e_kmalloc_a(uint32 sz);                // page aligned.
uint32 e_kmalloc_p(uint32 sz, uint32 *phys);  // returns a physical address.
uint32 e_kmalloc_ap(uint32 sz, uint32 *phys); // page aligned and returns a physical address.
uint32 e_kmalloc(uint32 sz);                  // vanilla (normal).

uint32 disable_early_kmalloc();
typedef struct
{
    uint32 present : 1;  // Page present in memory
    uint32 rw : 1;       // Read-only if clear, readwrite if set
    uint32 user : 1;     // Supervisor level only if clear
    uint32 accessed : 1; // Has the page been accessed since last refresh?
    uint32 dirty : 1;    // Has the page been written to since last refresh?
    uint32 unused : 7;   // Amalgamation of unused and reserved bits
    uint32 frame : 20;   // Frame address (shifted right 12 bits)
} page;
typedef struct
{
    page pages[1024];
} page_table;

typedef struct
{
    page_table *tables[1024];

    uint32 tablesPhysical[1024];

    uint32 physicalAddr;
} page_directory;

void initialize_memory(uint32 total_frames, uint32 ident_addr, uint32 ident_len);
void switch_page_directory(page_directory *new);
page *get_page(uint32 address, int make, page_directory *dir);

page *get_kernel_page(uint32 address, int make);
page *map_kernel_page(uint32 address, int make);
void unmap_kernel_page(uint32 address);

#endif