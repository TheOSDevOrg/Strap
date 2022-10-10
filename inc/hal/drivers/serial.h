#pragma once
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <hal/drivers/ports.h>

namespace system::hal::drivers::serial
{
    enum class type : uint16_t
    {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8
    };

    class serial_t final
    {
    private:
        uint16_t _type;
        ports::port8_t _data;
        ports::port8_t _interrupt_enable;
        ports::port8_t _interrupt_identification;
        ports::port8_t _fifo_control;
        ports::port8_t _line_control;
        ports::port8_t _modem_control;
        ports::port8_t _line_status;
        ports::port8_t _modem_status;
        ports::port8_t _scratch;
        bool _enabled = false;
        uint8_t _current_color;
        bool _keep;
    public:
        inline serial_t(type type, uint8_t color = 0xFF, bool keep = false)
        :   _type((uint16_t)type),
            _data(_type + 0, ports::type::bidirectional),
            _interrupt_enable(_type + 1, ports::type::output),
            _interrupt_identification(_type + 2, ports::type::input),
            _fifo_control(_type + 2, ports::type::output),
            _line_control(_type + 3, ports::type::output),
            _modem_control(_type + 4, ports::type::output),
            _line_status(_type + 5, ports::type::input),    
            _modem_status(_type + 6, ports::type::input),
            _scratch(_type + 7, ports::type::input),
            _current_color(color),
            _keep(keep)
        {
            _interrupt_enable = 0;
            _line_control = 0x80;
            _data = 0x03;
            _interrupt_enable = 0;
            _line_control = 0x03;
            _fifo_control = 0xC7;
            _modem_control = 0x0B;

            _modem_control = 0x1E;
            _data = 0xAE;
            if (_data != 0xAE) return;
            _modem_control = 0x0F;

            _enabled = true;
        }
        inline serial_t()
        :   _type((uint16_t)type::COM1),
            _data(_type + 0, ports::type::bidirectional),
            _interrupt_enable(_type + 1, ports::type::output),
            _interrupt_identification(_type + 2, ports::type::input),
            _fifo_control(_type + 2, ports::type::output),
            _line_control(_type + 3, ports::type::output),
            _modem_control(_type + 4, ports::type::output),
            _line_status(_type + 5, ports::type::input),    
            _modem_status(_type + 6, ports::type::input),
            _scratch(_type + 7, ports::type::input),
            _current_color(0xFF),
            _keep(false)
        {
            _interrupt_enable = 0;
            _line_control = 0x80;
            _data = 0x03;
            _interrupt_enable = 0;
            _line_control = 0x03;
            _fifo_control = 0xC7;
            _modem_control = 0x0B;

            _modem_control = 0x1E;
            _data = 0xAE;
            if (_data != 0xAE) return;
            _modem_control = 0x0F;

            _enabled = true;
        }
        inline ~serial_t()
        {
            if (!_enabled || _keep) return;
        }
    private:
        inline void set_color(uint8_t c)
        {
            switch (c)
            {
                case 0x00: { writes("\u001b[30m", 5); break; }
                case 0x01: { writes("\u001b[34m", 5); break; }
                case 0x02: { writes("\u001b[32m", 5); break; }
                case 0x03: { writes("\u001b[36m", 5); break; }
                case 0x04: { writes("\u001b[31m", 5); break; }
                case 0x05: { writes("\u001b[35m", 5); break; }
                case 0x06: { writes("\u001b[33m", 5); break; }
                case 0x07: { writes("\u001b[37m", 5); break; }
                case 0x08: { writes("\u001b[37m", 5); break; }
                case 0x09: { writes("\u001b[34m", 5); break; }
                case 0x0A: { writes("\u001b[32m", 5); break; }
                case 0x0B: { writes("\u001b[36m", 5); break; }
                case 0x0C: { writes("\u001b[31m", 5); break; }
                case 0x0D: { writes("\u001b[35m", 5); break; }
                case 0x0E: { writes("\u001b[33m", 5); break; }
                case 0x0F: { writes("\u001b[37m", 5); break; }
                case 0xFF: { writes("\u001b[0m",  4); break; }
                default:   { writes("\u001b[0m",  4); break; }
            }
        }
    private:
        bool has_received() const;
        bool has_error() const;
        bool can_send() const;
        char read_one() const;
        void write_one(char c);
    public:
        bool writes(const char* data, size_t size);
        bool writes_ln(const char* data, size_t size);
        
        bool switch_color(uint8_t color);

        bool writes_color(const char* data, size_t size, uint8_t color);
        bool writes_color_ln(const char* data, size_t size, uint8_t color);
    public:
        inline void disable()
        {
            if (!_enabled) return;
            _modem_control = 0x0;
            _data = 0x0;
            _interrupt_enable = 0;
            _line_control = 0x0;
            _fifo_control = 0x0;
            _modem_control = 0x0;
            _enabled = false;
        }
    public:
        inline uint8_t color() const { return _current_color; }
        inline bool enabled() const { return _enabled; }
    };
}