#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std/decl.h>
#include <std/tty.h>
#include <std/dict.h>
#include <hal/drivers/keyboard.h>
#include <core/io/layout.hpp>

namespace system::core::io::layouts
{
    class english final : public layout
    {
    private:
        std::key_t _specials_right[100];
        std::key_t _layout[92];
        std::key_t _layout_sft[90];
        std::key_t _layout_alt_gr[90];
        std::key_t _layout_ctrl_gr[90];
    public:
        english();
        ~english();
    public:
        std::key_t scan(system::hal::drivers::kb::scancode_t code, std::array<std::special_t> &combination, bool right = false) override;
    };
}