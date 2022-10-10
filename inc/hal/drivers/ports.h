#pragma once
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>

namespace system::hal::drivers::ports
{
    enum class type
    {
        input,
        output,
        bidirectional
    };

    class port32_t final
    {
    private:
        uint16_t _port;
        type _type;
    public:
        inline port32_t(uint16_t port, type type)
        : _port(port),
          _type(type)
        {
        }
        inline port32_t()
        : _port(0),
          _type(type::bidirectional)
        {
        }
        inline ~port32_t()
        {
        }
    public:
        uint32_t read() const;
        bool reads(uint32_t* data, size_t size) const;
    public:
        bool write(uint32_t data);
        bool writes(const uint32_t* data, size_t size);
    public:
        inline void init(uint16_t port, type type)
        {
            _port = port;
            _type = type;
        }
    public:
        inline operator uint32_t() const
        {
            return read();
        }
        inline operator int() const
        {
            return read();
        }
        inline port32_t & operator=(uint32_t data)
        {
            write(data);
            return *this;
        }
        inline port32_t & operator=(port32_t &right)
        {
            write(right.read());
            return *this;
        }
        inline port32_t & operator+=(uint32_t right)
        {
            write(right + read());
            return *this;
        }
        inline port32_t & operator+=(port32_t &right)
        {
            write(right.read()+read());
            return *this;
        }
        inline port32_t & operator-=(uint32_t right)
        {
            write(read()-right);
            return *this;
        }
        inline port32_t & operator-=(port32_t &right)
        {
            write(read()-right.read());
            return *this;
        }
        inline port32_t & operator*=(uint32_t right)
        {
            write(read()*right);
            return *this;
        }
        inline port32_t & operator*=(port32_t &right)
        {
            write(read()*right.read());
            return *this;
        }
        inline port32_t & operator/=(uint32_t right)
        {
            write(read()/right);
            return *this;
        }
        inline port32_t & operator/=(port32_t &right)
        {
            write(read()/right.read());
            return *this;
        }
        inline bool operator==(uint32_t right)
        {
            return read() == right;
        }
        inline bool operator==(port32_t &right)
        {
            return read() == right.read();
        }
        inline bool operator!=(uint32_t right)
        {
            return read() != right;
        }
        inline bool operator!=(port32_t &right)
        {
            return read() != right.read();
        }
        inline uint32_t operator*()
        {
            return read();
        }
    };

    class port16_t final
    {
    private:
        uint16_t _port;
        type _type;
    public:
        inline port16_t(uint16_t port, type type)
        : _port(port),
          _type(type)
        {
        }
        inline port16_t()
        : _port(0),
          _type(type::bidirectional)
        {
        }
        inline ~port16_t()
        {
        }
    public:
        uint16_t read() const;
        bool reads(uint16_t* data, size_t size) const;
    public:
        bool write(uint16_t data);
        bool writes(const uint16_t* data, size_t size);
    public:
        inline void init(uint16_t port, type type)
        {
            _port = port;
            _type = type;
        }
    public:
        inline operator uint16_t() const
        {
            return read();
        }
        inline operator int() const
        {
            return read();
        }
        inline port16_t & operator=(int data)
        {
            write(data);
            return *this;
        }
        inline port16_t & operator=(port16_t &right)
        {
            write(right.read());
            return *this;
        }
        inline port16_t & operator+=(int right)
        {
            write(right + read());
            return *this;
        }
        inline port16_t & operator+=(port16_t &right)
        {
            write(right.read()+read());
            return *this;
        }
        inline port16_t & operator-=(int right)
        {
            write(read()-right);
            return *this;
        }
        inline port16_t & operator-=(port16_t &right)
        {
            write(read()-right.read());
            return *this;
        }
        inline port16_t & operator*=(int right)
        {
            write(read()*right);
            return *this;
        }
        inline port16_t & operator*=(port16_t &right)
        {
            write(read()*right.read());
            return *this;
        }
        inline port16_t & operator/=(int right)
        {
            write(read()/right);
            return *this;
        }
        inline port16_t & operator/=(port16_t &right)
        {
            write(read()/right.read());
            return *this;
        }
        inline bool operator==(int right)
        {
            return read() == right;
        }
        inline bool operator==(port16_t &right)
        {
            return read() == right.read();
        }
        inline bool operator!=(int right)
        {
            return read() != right;
        }
        inline bool operator!=(port16_t &right)
        {
            return read() != right.read();
        }
        inline uint16_t operator*()
        {
            return read();
        }
    };

    class port8_t final
    {
    private:
        uint16_t _port;
        type _type;
    public:
        inline port8_t(uint16_t port, type type)
        : _port(port),
          _type(type)
        {
        }
        inline port8_t()
        : _port(0),
          _type(type::bidirectional)
        {
        }
        inline ~port8_t()
        {
        }
    public:
        uint8_t read() const;
        bool reads(uint8_t* data, size_t size) const;
    public:
        bool write(uint8_t data);
        bool writes(const uint8_t* data, size_t size);
        bool writes(const char* data, size_t size);
    public:
        inline void init(uint16_t port, type type)
        {
            _port = port;
            _type = type;
        }
    public:
        inline operator uint8_t() const
        {
            return read();
        }
        inline operator char() const
        {
            return (char)read();
        }
        inline operator int() const
        {
            return read();
        }
        inline port8_t & operator=(int data)
        {
            write(data);
            return *this;
        }
        inline port8_t & operator=(char data)
        {
            write((int)data);
            return *this;
        }
        inline port8_t & operator=(port8_t &right)
        {
            write(right.read());
            return *this;
        }
        inline port8_t & operator+=(int right)
        {
            write(right + read());
            return *this;
        }
        inline port8_t & operator+=(port8_t &right)
        {
            write(right.read()+read());
            return *this;
        }
        inline port8_t & operator-=(int right)
        {
            write(read()-right);
            return *this;
        }
        inline port8_t & operator-=(port8_t &right)
        {
            write(read()-right.read());
            return *this;
        }
        inline port8_t & operator*=(int right)
        {
            write(read()*right);
            return *this;
        }
        inline port8_t & operator*=(port8_t &right)
        {
            write(read()*right.read());
            return *this;
        }
        inline port8_t & operator/=(int right)
        {
            write(read()/right);
            return *this;
        }
        inline port8_t & operator/=(port8_t &right)
        {
            write(read()/right.read());
            return *this;
        }
        inline bool operator==(int right)
        {
            return read() == right;
        }
        inline bool operator==(port8_t &right)
        {
            return read() == right.read();
        }
        inline bool operator!=(int right)
        {
            return read() != right;
        }
        inline bool operator!=(port8_t &right)
        {
            return read() != right.read();
        }
        inline uint8_t operator *()
        {
            return read();
        }
    };
}