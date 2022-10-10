#include <hal/drivers/keyboard.h>
#include <core/kernel.hpp>

using namespace system::hal::drivers::kb;
using namespace system::hal::drivers::ports;
using namespace system::hal;

driver_t::driver_t()
: _cmd_port(0x64, ports::type::bidirectional),
  _dat_port(0x60, ports::type::bidirectional)
{
}

void driver_t::start()
{
    uint8_t r;
    
    _cmd_port = 0xAD;

    _cmd_port = 0xAA;
    if (*_dat_port == 0x55) dprint_info("keyboard is good to go");

    _cmd_port = 0xAE;
    _cmd_port = 0x20;

    uint8_t status = (*_dat_port | 1) & ~0x10;
    
    _cmd_port = 0x60;
    _dat_port = status;

    _dat_port = 0xFF;
    while (*_dat_port != 0xAA) ;
    dprint_info("keyboard reset");

    _dat_port = 0xF4;
    if (_dat_port == 0xFA) dprint_debug("keyboard scan enabled");

    interrupts::idt::manager_t::register_irq(1, handle);
}
void driver_t::stop()
{
    interrupts::idt::manager_t::register_irq(1, nullptr);
    _dat_port = 0xF5;
    if (_dat_port == 0xFA) dprint_debug("keyboard scan disabled");
}

static bool _skip = false;
static bool _right = false;

void driver_t::handle(interrupts::idt::regs32_t *r)
{
    port8_t dat = port8_t(0x60, type::input);
    port8_t cmd = port8_t(0x64, type::output);
    if (_skip)
    {
        dat.read();
        _skip = false;
        return;
    }


    uint8_t scan = dat;

    scancode_t scancode = {};

    if (scan == 0xF0)
    {
        kernel::ttys[0]->write_line("RELEASE");
        scancode.key = dat;
        scancode.release = true;
        _skip = true;
    }
    else if (scan == 0xE0)
    {
        _right = true;
        return handle(r);
    }
    else
    {
        scancode_t dump = *((scancode_t *)&scan);
        scancode.key = dump.key;
        scancode.release = dump.release;
    }

    _skip = true;
    system::kernel::current_tty->handle_scancode(scancode, _right);
    _skip = false;

    if (_right)
    {
        _right = false;
        _skip = true;
    }
}