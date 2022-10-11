#pragma once

#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <std/string.h>
#include <std/dict.h>
#include <std/tty.h>
#include <std/string.h>
#include <core/io/layout.hpp>
#include <gfx/doublebuffer.hpp>
#include <hal/drivers/vbe.h>

namespace system::core::io::ttys
{
    class vbe_tty final : std::tty
    {
    private:
        std::dictionary<std::array<std::key_t>, std::combination_handler> _combinations;
        layout *_layout;
        bool _in;
        system::kernel::gfx::buffer_info_t _buffer;
        system::hal::drivers::vbe::Driver vesa;
    public:
        vbe_tty(layout *layout, uint16_t width = 800, uint16_t height = 600);
        ~vbe_tty();
    public:
        std::key_t parse_scancode(system::hal::drivers::kb::scancode_t scan, bool right = false) override;
        bool handle_sequence(std::array<std::key_t> &seq) override;
        void handle_input(std::key_t &k) override;
    public:
        bool register_sequence(std::array<std::key_t> &seq, std::combination_handler handler) override;
    public:
        void enter() override;
        void exit() override;
        void render() override;
    };
}