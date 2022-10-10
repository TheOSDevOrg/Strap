#include <hal/drivers/serial.h>

using namespace system::hal::drivers::serial;

bool serial_t::has_received() const
{
    return _line_status.read() & 0x01;
}
bool serial_t::can_send() const
{
    return _line_status.read() & 0x20;
}
bool serial_t::has_error() const
{
    return _line_status.read() & 0x80;
}

char serial_t::read_one() const
{
    if (!_enabled) return 0;
    while (!has_received());
    return _data;
}
void serial_t::write_one(char c)
{
    if (!_enabled) return;
    while (!can_send());
    _data = c;
}

bool serial_t::writes(const char* data, size_t size)
{
    if (!_enabled) return false;
    for (size_t i = 0; i < size; i++)
    {
        write_one(data[i]);
    }
    return true;
}
bool serial_t::writes_ln(const char* data, size_t size)
{
    if (!_enabled) return false;
    for (size_t i = 0; i < size; i++)
    {
        write_one(data[i]);
    }
    write_one('\n');
    return true;
}
bool serial_t::writes_color(const char* data, size_t size, uint8_t color)
{
    if (!_enabled) return false;
    uint8_t c = _current_color;
    set_color(color);
    for (size_t i = 0; i < size; i++)
    {
        write_one(data[i]);
    }
    set_color(c);
    return true;
}
bool serial_t::writes_color_ln(const char* data, size_t size, uint8_t color)
{
    if (!_enabled) return false;
    uint8_t c = _current_color;
    set_color(color);
    for (size_t i = 0; i < size; i++)
    {
        write_one(data[i]);
    }
    set_color(c);
    write_one('\n');
    return true;
}