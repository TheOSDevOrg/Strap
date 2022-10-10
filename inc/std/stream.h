#pragma once

#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
#include <std/ptr.h>

namespace std
{
    class stream final
    {
    private:
        size_t _size;
        uintptr_t _ptr;
        bool _seekable;
        size_t _pos;
        bool _keep;

    public:
        inline stream(uintptr_t ptr, size_t size, bool seekable = true, size_t start = 0, bool keep = true)
        : _ptr(ptr),
          _seekable(seekable),
          _pos(start),
          _size(size),
          _keep(keep)
        {
        }
        inline stream()
        : _ptr(0),
          _seekable(false),
          _pos(0),
          _size(0),
          _keep(false)
        {
        }
        inline ~stream()
        {
            if(!_keep) free((void *&)_ptr);
            _size = 0;
            _ptr = 0;
            _seekable = false;
            _pos = -1;
            _keep = false;
        }
    public:
        inline bool seekable() { return _seekable; }
        inline bool seek(size_t pos)
        {
            if (!_seekable) return false;
            else if (pos >= _size) return false;
            _pos = pos;
            return true;
        }
        inline size_t tell() { return _pos; }
        inline size_t size() { return _size; }
        inline bool put(uint8_t byte)
        {
            if (_pos >= _size || _pos < 0) return false;
            ((uint8_t*)_ptr)[_pos] = byte;
            _pos++;
        }
        inline bool puts(const uint8_t* data, size_t size)
        {
            if (_pos + size >= _size) return false;
            for (size_t s = 0; s < size; s++) put(data[s]);
            return true;
        }
        template<typename T> inline bool frame_as(std::ptr<T> &ref)
        {
            if (sizeof(T) + _pos >= _size) return false;
            ref = (T*)(_ptr + _pos);
            _pos += sizeof(T);
            return true;
        }
        template <typename T> inline bool frame_at_as(size_t pos, std::ptr<T> &ref)
        {
            if (!_seekable) return false;
            else if (sizeof(T) + pos >= _size) return false;
            ref = (T*)(_ptr + pos);
            return true;
        }
        inline uintptr_t c_ptr() { return _ptr; }
        template<typename T> inline ptr<T> ptr() { return ptr<T>((T*)_ptr); }

    public:
        uint8_t& operator[](int pos)
        {
            uint8_t n = 0;
            if (pos > _size) return n; 
            return ((uint8_t*)_ptr)[pos];
        }
    public:
        inline void init(uintptr_t ptr, size_t size, bool seekable = true, size_t start = 0, bool keep = true)
        {
            if (_ptr != 0) return;

            this->_ptr = ptr;
            this->_size = size;
            this->_seekable = seekable;
            this->_pos = start;
            this->_keep = keep;
        }
    };
}