#include <hal/drivers/ports.h>
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <core/io/debug.hpp>

using namespace system::hal::drivers::ports;

uint32_t inl(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %%dx, %%eax" : "=a" (data) : "d" (port));
    return data;
}
void outl(uint16_t port, uint32_t data)
{
    asm volatile("outl %%eax, %%dx" : : "d" (port), "a" (data));
}
uint16_t inw(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %%dx, %%ax" : "=a" (data) : "d" (port));
    return data;
}
void outw(uint16_t port, uint16_t data)
{
    asm volatile("outw %%ax, %%dx" : : "d" (port), "a" (data));
}
uint8_t inb(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}
void outb(uint16_t port, uint8_t data)
{
    asm volatile("outb %%al, %%dx" : : "d" (port), "a" (data));
}

uint32_t port32_t::read() const
{
    if (_type != type::input && _type != type::bidirectional) return 0;
    return inl(_port);
}
bool port32_t::reads(uint32_t* data, size_t size) const
{
    if (_type != type::input && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        data[i] = inl(_port);
    }
    return true;
}
bool port32_t::write(uint32_t data)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    outl(_port, data);
    return true;
}
bool port32_t::writes(const uint32_t* data, size_t size)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        outl(_port, data[i]);
    }
    return true;
}

uint16_t port16_t::read() const
{
    if (_type != type::input && _type != type::bidirectional) return 0;
    return inw(_port);
}
bool port16_t::reads(uint16_t* data, size_t size) const
{
    if (_type != type::input && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        data[i] = inw(_port);
    }
    return true;
}
bool port16_t::write(uint16_t data)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    outw(_port, data);
    return true;
}
bool port16_t::writes(const uint16_t* data, size_t size)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        outw(_port, data[i]);
    }
    return true;
}

uint8_t port8_t::read() const
{
    if (_type != type::input && _type != type::bidirectional) return 0;
    return inb(_port);
}
bool port8_t::reads(uint8_t* data, size_t size) const
{
    if (_type != type::input && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        data[i] = inb(_port);
    }
    return true;
}
bool port8_t::write(uint8_t data)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    outb(_port, data);
    return true;
}
bool port8_t::writes(const uint8_t* data, size_t size)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        outb(_port, data[i]);
    }
    return true;
}
bool port8_t::writes(const char* data, size_t size)
{
    if (_type != type::output && _type != type::bidirectional) return false;
    for (size_t i = 0; i < size; i++)
    {
        outb(_port, (uint8_t)data[i]);
    }
    return true;
}