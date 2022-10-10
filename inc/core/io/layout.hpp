#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std/decl.h>
#include <std/tty.h>
#include <hal/drivers/keyboard.h>

namespace system::core::io
{
    class layout
    {
    public:
        inline layout() {}
        inline ~layout() {}
    public:
        virtual std::key_t scan(system::hal::drivers::kb::scancode_t code, std::array<std::special_t> &combination, bool right = false);
    };
}