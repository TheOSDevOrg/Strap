#pragma once
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <hal/drivers/ports.h>
#include <std/stacklist.h>
#include <hal/interrupts/idt.h>

namespace system::hal::drivers::kb
{
    struct scancode_t final
    {
        uint8_t key : 7;
        bool release : 1;
    };
    class driver_t final
    {
    private:
        ports::port8_t _cmd_port;
        ports::port8_t _dat_port;
    public:
        driver_t();
        inline ~driver_t() {}
    public:
        void start();
        void stop();
    private:
        static void handle(interrupts::idt::regs32_t *r);
    };
}