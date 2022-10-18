#include <core/io/debug.hpp>
#include <core/io/out.hpp>
#include <core/kernel.hpp>
#include <core/memory.h>

using namespace system::core::io;
using namespace system::hal::drivers;

out::out(uintptr_t addr, size_t maxx, size_t maxy, size_t depth)
: _fg(color::white),
  _bg(color::blue),
  _lock(false),
  _cmd(0x3D4, ports::type::bidirectional),
  _dat(0x3D5, ports::type::bidirectional)
{
    _cmd = 0x09;
    _dat = 0x0F;

    _cmd = 0x0B;
    _dat = 0x0F;

    _cmd = 0x0A;
    _dat = 0x0E;
}

void out::clean()
{
    for (int i = get_init_off(); i < _buffer.size(); i++)
    {
        _buffer[i].attrib.bg = _bg;
        _buffer[i].attrib.fg = _fg;
        _buffer[i].character = '\0';
    }
    x = ix, y = iy;
}
void out::clean(color fg, color bg)
{
    for (int i = get_init_off(); i < _buffer.size(); i++)
    {
        _buffer[i].attrib.bg = bg;
        _buffer[i].attrib.fg = fg;
        _buffer[i].character = '\0';
    }
    x = ix, y = iy;
}
void out::clean(color fg, color bg, uint8_t x, uint8_t y)
{
    for (int i = x+(y*MAXX); i < MAXX+(y*MAXX); i++)
    {
        _buffer[i].attrib.bg = bg;
        _buffer[i].attrib.fg = fg;
        _buffer[i].character = '\0';
    }
}
void out::clean(color fg, color bg, uint8_t x, uint8_t y, uint8_t ex, uint8_t ey)
{
    for (int i = x+(y*MAXX); i < ex+(ey*MAXX); i++)
    {
        _buffer[i].attrib.bg = bg;
        _buffer[i].attrib.fg = fg;
        _buffer[i].character = '\0';
    }
}
void out::putc(char c)
{
    if (c < ' ' || c > '~')
        { parse_special(c); return; } 
    
    int off;
    _buffer[off = get_off()].character = c;
    _buffer[off].attrib.bg = _bg;
    _buffer[off].attrib.fg = _fg;

    x++;
    if (x >= MAXX) new_line();
}
void out::putc_at(char c, uint8_t x, uint8_t y)
{
    if (c < ' ' || c > '~')
        { parse_special(c); return; } 
    
    uint8_t ox = this->x;
    uint8_t oy = this->y;

    this->x = x;
    this->y = y;

    int off;
    _buffer[off = get_off()].character = c;
    _buffer[off].attrib.bg = _bg;
    _buffer[off].attrib.fg = _fg;

    this->x = ox;
    this->y = oy;
}
void out::putc_color(char c, color fg, color bg)
{
    if (c < ' ' || c > '~')
        { parse_special(c); return; } 
    
    int off;
    _buffer[off = get_off()].character = c;
    _buffer[off].attrib.bg = bg;
    _buffer[off].attrib.fg = fg;

    x++;
    if (x >= MAXX) new_line();
}
void out::putc_color_at(char c, color fg, color bg, uint8_t x, uint8_t y)
{
    if (c < ' ' || c > '~')
        { parse_special(c); return; } 
    
    uint8_t ox = this->x;
    uint8_t oy = this->y;

    this->x = x;
    this->y = y;

    int off;
    _buffer[off = get_off()].character = c;
    _buffer[off].attrib.bg = bg;
    _buffer[off].attrib.fg = fg;

    this->x = ox;
    this->y = oy;
}
void out::puts(char *s)
{
    while (*s)
    {
        putc(*(s++));
    }
}
void out::puts_at(char *s, uint8_t x, uint8_t y)
{
    while (*s)
    {
        putc_at(*(s++), x++, y);
    }
}
void out::puts_color(char *s, color fg, color bg)
{
    while (*s)
    {
        putc_color(*(s++), fg, bg);
    }
}
void out::puts_color_at(char *s, color fg, color bg, uint8_t x, uint8_t y)
{
    while (*s)
    {
        putc_color_at(*(s++), fg, bg, x++, y);
    }
}
void out::puts(std::string &s)
{
    for (auto c : s)
    {
        putc(c);
    }
}
void out::puts_at(std::string &s, uint8_t x, uint8_t y)
{
    for (auto c : s)
    {
        putc_at(c, x++, y);
    }
}
void out::puts_color(std::string &s, color fg, color bg)
{
    for (auto c : s)
    {
        putc_color(c, fg, bg);
    }
}
void out::puts_color_at(std::string &s, color fg, color bg, uint8_t x, uint8_t y)
{
    for (auto c : s)
    {
        putc_color_at(c, fg, bg, x++, y);
    }
}
void out::render()
{
    if (_lock) return;
    uint16_t pos = x+(y*MAXX);
    _cmd = 0x0F;
    _dat = (uint8_t)(pos & 0xFF);
    _cmd = 0x0E;
    _dat = (uint8_t)((pos >> 8) & 0xFF);
    memcpy((void *)VGA_ADDR, (void *)_buffer.c_list(), _buffer.size());
}

void out::init(uintptr_t addr, size_t maxx, size_t maxy, size_t depth, uint8_t initx, uint8_t inity)
{
    _lock = true;
    VGA_ADDR = addr;
    MAXX = maxx;
    MAXY = maxy;
    VGA_SZ = maxx*maxy;
    _buffer = std::array<char_t>(VGA_SZ*depth, true);
    _fg = color::white;
    _bg = color::black;
    ix = initx, iy = inity;
    x = ix, y = iy;
    clean();
}
void out::dispose()
{
    _buffer.dispose();
}

void out::format_string(char* str, const char* fmt, va_list args)
{
    vsprintf(str, fmt, args);
}

void out::parse_special(char c)
{
    switch (c)
    {
    case '\n':
        new_line();
        break;
    case '\b':
        backspace();
        break;
    default:
        break;
    }
}
void out::new_line()
{
    x = ix, y++;
    if (get_off() >= VGA_SZ) move_up();
}
void out::backspace()
{
    if (!x)
    {
        y--;
        for (; x < MAXX; x++)
        {
            if (_buffer[get_off()].character == '\0') break;
        }
    }
    else x--;
    char_t c = {};
    c.attrib.fg = _fg;
    c.attrib.bg = _bg;
    c.character = '\0';
    _buffer[get_off()] = c;
}
void out::move_up()
{
    render();
    for (int i = iy+1; i < MAXY; i++)
    {
        memcpy((void *)&_buffer[ix+((i-1)*MAXX)], (void *)&_buffer[ix+(i*MAXX)], MAXX*sizeof(char_t));
    }
    char_t dummy = char_t();

    dummy.attrib.fg = _fg;
    dummy.attrib.bg = _bg;
    dummy.character = '\0';

    y = MAXY-1;
    x = ix;

    memsetw((uint16_t *)&_buffer[get_off()], *((uint16_t*)&dummy), sizeof(char_t)*MAXX);
}

void out::sinit()
{
    ports::port8_t _cmd = ports::port8_t(0x3D4, ports::type::bidirectional);
    ports::port8_t _dat = ports::port8_t(0x3D5, ports::type::bidirectional);
    
    _cmd = 0x09;
    _dat = 0x0F;

    _cmd = 0x0B;
    _dat = 0x0F;

    _cmd = 0x0A;
    _dat = 0x0E;
}