#pragma once
#include <stddef.h>
#include <stdint.h>
#include <std/decl.h>
#include <std/array.h>
#include <std/tty.h>
#include <std/dict.h>
#include <core/io/layouts/english.hpp>

namespace system::core::io::ttys
{
    class debug_tty final : public std::tty
    {
    private:
        std::dictionary<std::array<std::key_t>*, std::combination_handler> _combinations;
        layout *_layout;
    public:
        debug_tty(layout *layout);
        ~debug_tty();
    public:
        std::key_t parse_scancode(system::hal::drivers::kb::scancode_t scan, bool right = false) override;
        bool handle_sequence(std::array<std::key_t> &seq) override;
        void handle_input(std::key_t &k) override;
    public:
        bool register_sequence(std::array<std::key_t> &seq, std::combination_handler handler) override;
    };
}