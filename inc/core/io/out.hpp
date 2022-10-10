#pragma once

#include <stdint.h>
#include <std/decl.h>
#include <std/string.h>
#include <std/array.h>
#include <hal/drivers/ports.h>

namespace std
{
    class tty;
}

namespace system::core::io
{
    enum class color : uint8_t
    {
        black,
        blue,
        green,
        cyan,
        red,
        magenta,
        brown,
        light_gray,
        dark_gray,
        light_blue,
        light_green,
        light_cyan,
        light_red,
        light_magenta,
        yellow,
        white
    };
    struct attrib_t final
    {
        color fg : 4;
        color bg : 4;
    } packed__;
    struct char_t final
    {
        char character;
        attrib_t attrib;
    } packed__;

    class out final
    {
    friend class std::tty;
    private:
        std::array<char_t> _buffer;
        color _fg;
        color _bg;
        uint8_t x, y, ix, iy;
        bool _lock;
        hal::drivers::ports::port8_t _cmd;
        hal::drivers::ports::port8_t _dat;
        size_t VGA_SZ;
        uintptr_t VGA_ADDR;
        size_t MAXX;
        size_t MAXY;

    public:
        out(uintptr_t addr = 0xb8000, size_t max_x = 80, size_t max_y = 25, size_t depth = 2);
    public:
        void clean();
        void clean(color fg, color bg);
        void clean(color fg, color bg, uint8_t x, uint8_t y);
        void clean(color fg, color bg, uint8_t x, uint8_t y, uint8_t ex, uint8_t ey);
        void putc(char c);
        void putc_at(char c, uint8_t x, uint8_t y);
        void putc_color(char c, color fg, color bg);
        void putc_color_at(char c, color fg, color bg, uint8_t x, uint8_t y);
        void puts(char *s);
        void puts(std::string &s);
        void puts_at(char *s, uint8_t x, uint8_t y);
        void puts_at(std::string &s, uint8_t x, uint8_t y);
        void puts_color(char *s, color fg, color bg);
        void puts_color(std::string &s, color bg, color fg);
        void puts_color_at(char *s, color fg, color bg, uint8_t x, uint8_t y);
        void puts_color_at(std::string &s, color bg, color fg, uint8_t x, uint8_t y);
        void render();
    public:
        void init(uintptr_t addr = 0xb8000, size_t max_x = 80, size_t max_y = 25, size_t depth = 2, uint8_t init_x = 0, uint8_t init_y = 0);
        void dispose();
    private:
        inline int get_off()
        {
            return x+(y*MAXX);
        }
        inline int get_init_off()
        {
            return ix+(iy*MAXX);
        }
    public:
        void parse_special(char c);
        void new_line();
        void backspace();
        void move_up();
    public:
        inline color get_fg() { return _fg; }
        inline color get_bg() { return _bg; }
    public:
        inline void set_fg(color c) { _fg = c; }
        inline void set_bg(color c) { _bg = c; }
    public:
        inline size_t get_vga_sz() { return VGA_SZ; }
        inline size_t get_max_x() { return MAXX; }
        inline size_t get_max_y() { return MAXY; }
    public:
        static void sinit();
    };
}