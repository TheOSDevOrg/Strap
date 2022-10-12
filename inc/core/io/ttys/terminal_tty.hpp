#pragma once

#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <std/string.h>
#include <std/dict.h>
#include <std/tty.h>
#include <std/string.h>
#include <core/io/layout.hpp>

namespace system::core::io::ttys
{
    class terminal_tty final : public std::tty
    {
    public:
        typedef void (*cmd_callback)(std::arraylist<std::string> &args);
    private:
        layout *_layout;
        std::dictionary<std::array<std::key_t>, std::combination_handler> _combinations;
        std::dictionary<std::string, cmd_callback> _callbacks;
        std::string _input;
    public:
        terminal_tty(layout *kb_layout, system::core::io::color foreground = system::core::io::color::white, system::core::io::color background = system::core::io::color::black);
    public:
        std::key_t parse_scancode(system::hal::drivers::kb::scancode_t scan, bool right = false) override;
        bool handle_sequence(std::array<std::key_t> &seq) override;
        void handle_input(std::key_t &k) override;
    public:
        bool register_sequence(std::array<std::key_t> &seq, std::combination_handler handler) override;
    private:
        void print_term();
        void handle_command();
    public:
        inline void register_command(std::string name, cmd_callback handler)
        {
            _input.clear();
            return _callbacks.add(name.make_resident(), handler);
        }
    public:
        void enter() override;
        void exit() override;
    };
}