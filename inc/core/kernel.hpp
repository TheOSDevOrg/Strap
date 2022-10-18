#pragma once
#include <stdint.h>
#include <std/stdio.h>
#include <std/ubsan.h>
#include <std/math.h>
#include <std/decl.h>
#include <std/array.h>
#include <std/ptr.h>
#include <core/memory.h>
#include <boot/mboot.h>
#include <hal/drivers/serial.h>
#include <boot/loader.h>
#include <core/memory/heap.hpp>
#include <core/io/debug.hpp>
#include <std/arraylist.h>
#include <std/stacklist.h>
#include <core/io/out.hpp>
#include <std/tty.h>
#include <std/dict.h>
#include <std/cstring.h>
#include <std/hash.h>
#include <std/string.h>
#include <hal/drivers/keyboard.h>
#include <hal/interrupts/gdt.h>
#include <hal/interrupts/idt.h>
#include <core/io/ttys/debug_tty.hpp>
#include <core/io/ttys/terminal_tty.hpp>
#include <core/io/ttys/vbe_tty.hpp>
#include <hal/drivers/vbe.h>

namespace system::kernel
{
    namespace tests
    {
        void alloc_routine_check();
    }
    extern system::core::memory::heap_t heap;
    extern std::tty *current_tty;
    extern std::arraylist<std::tty *> ttys;
    extern system::hal::drivers::kb::driver_t *kb;
    struct environment
    {
        system::hal::drivers::serial::serial_t stddbg;
    };
    extern environment *env;

    void bootstrap(system::grub::multiboot_hdr *mboot);
    void beforerun();
    void run();
}