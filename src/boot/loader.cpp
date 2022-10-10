/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
(C) Copyright 2022, All rights reserved.
*/
#include <std/decl.h>
#include <boot/loader.h>
#include <core/kernel.hpp>
#include <core/env.hpp>

using namespace system::kernel;

system::core::memory::heap_t *system::env::current_heap;
system::core::memory::heap_t system::env::common_heap;
system::core::memory::heap_t system::env::lostfound_heap;

__cdecl uintptr_t KEND;

multiboot_header_loader multiboot section__(.multiboot) = {
    .magic = LOADER_MAGIC,
    .flags = {
        .align = true,
        .mmap = true,
        #ifdef VBE_MODE
        .video = true,
        #else
        .video = false,
        #endif
    },
    #ifdef VBE_MODE
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1) | (1 << 2))),
    #else
    .checksum = -(LOADER_MAGIC + ((1 << 0) | (1 << 1))),
    #endif
    .reserved = {0},
    .vbe_info = {
        .video_mode = 0,
        .width = 800,
        .height = 600,
        .depth = 32
    }
};
memory_regions regions;

void *mboot;
__cdecl void __init()
{
    system::kernel::environment e =
    {
        .stddbg = system::hal::drivers::serial::serial_t(system::hal::drivers::serial::type::COM1, 0xFF, true)
    };
    system::kernel::env = &e;
    std::dbgio::debug_lvl = std::dbgio::dbgtype::DEBUG;

    system::kernel::heap.init((uintptr_t)regions.kernel_heap, (uintptr_t)regions.common_heap - (uintptr_t)regions.kernel_heap, true);
    system::env::common_heap.init((uintptr_t)regions.common_heap, (uintptr_t)regions.lf_heap-(uintptr_t)regions.common_heap, true);
    system::env::lostfound_heap.init((uintptr_t)regions.lf_heap, regions.end-(uintptr_t)regions.lf_heap, true);

    system::env::current_heap = &system::kernel::heap;

    bootstrap((system::grub::multiboot_hdr*)mboot);

    beforerun();
    for(;;) run();
}

__cdecl void _start()
{
    asm volatile("cli");
    uintptr_t stack = (uintptr_t)regions.kernel_stack + 2*1024*1024;

    asm("movl %0, %%esp" : : "m"(stack));
    asm("movl %0, %%ebp" : : "m"(stack));
    
    regions.kernel_stack = (uint8_t*)KEND;
    regions.kernel_heap = regions.kernel_stack+2*1024*1024;
    regions.common_heap = regions.kernel_heap+40*1024*1024;
    regions.lf_heap = regions.common_heap+40*1024*1024;
    regions.end = (uintptr_t)regions.lf_heap+2*1024*1024;

    asm("mov %%ebx, %0" : "=m"(mboot));

    __init();
}