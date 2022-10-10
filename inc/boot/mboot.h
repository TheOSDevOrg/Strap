#include <stdint.h>
#include <std/decl.h>

namespace system::grub
{
    struct multiboot_hdr
    {
        uintptr_t flags;
        
        uintptr_t mem_lower;
        uintptr_t mem_upper;
        
        uintptr_t boot_dev;
        
        uintptr_t cmd_line;
        
        uintptr_t modules_count;
        uintptr_t modules_addr;
        
        uintptr_t symbols[4];
        
        uintptr_t memmap_length;
        uintptr_t memmap_addr;
        
        uintptr_t devs_count;
        uintptr_t devs_addr;
        
        uintptr_t configs;
        uintptr_t bootloader;
        
        uintptr_t apm_table;
        
        uintptr_t vbe_ctrl_info;
        uintptr_t vbe_mode_info;
        uintptr_t vbe_mode;

        uintptr_t vbe_interface_seg;
        uintptr_t vbe_interface_off;
        uintptr_t vbe_interface_len;
    } packed__;
}