#include "utils/util.h"
#include "utils/debug/debug.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "pit/pit.h"
#include "pic/pic.h"
#include "paging/paging.h"
#include "heap/heap.h"
#include "keyboard/keyboard.h"
#include "shell/shell.h"
#include "video/video.h"

#define TIMER_HZ 60

typedef struct
{
    unsigned long tab_size;
    unsigned long str_size;
    unsigned long address;
    unsigned long reserved;
} aout_symbol_table_t;

typedef struct
{
    unsigned long num;
    unsigned long size;
    unsigned long address;
    unsigned long shndx;
} elf_section_header_table_t;

typedef struct multiboot_info
{
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    unsigned long cmdline;
    unsigned long mods_addr;
    union
    {
        aout_symbol_table_t aout_sym_t;
        elf_section_header_table_t elf_sec_t;
    } u;
    unsigned long mmap_length;
    unsigned long mmap_addr;
} multiboot_info;
void kernel_main(multiboot_info *mi)
{
    uint32 low_pages = 256;
    uint32 high_pages = (mi->mem_upper * 1024) / 4096;

    uint32 total_frames = high_pages + low_pages;
    debug_terminal_initialize();
    init_gdt();
    remap_pic();
    init_idt();
    initialize_memory(total_frames, 0xA0000, 64000);
    char *c = malloc(3);
    c[0] = 'h';
    c[1] = 'i';
    c[2] = 0;
    int kbd_success = init_keyboard();
    if (kbd_success == 0)
    {
        debug_terminal_writestring("Keyboard initialized.\n");
    }
    setup_video_mode(); // after this point debug terminal will not work
    init_shell();
    int sync = get_timer_tick(); // make sure its synced with frame updates (I doubt this works but worth trying)
    while (1)
    {
        int time_cur = get_timer_tick();
        if (time_cur == sync + 1)
        {
            render_shell();
            swap_buffers();
            sync = time_cur;
        }
    }
}
