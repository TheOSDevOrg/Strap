#pragma once

#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
#include <std/ptr.h>

namespace std
{
    template<typename T> class array final
    {
    private:
        size_t _size;
        T * _list;
        T * _dummy;
        bool _stay;
    public:
        array(size_t size, bool stay = false)
        : _size(size),
          _stay(stay)
        {
            _list = (T *)alloc(sizeof(T)*_size);
            _dummy = nullptr;
        }
        array()
        : _size(0),
          _list(nullptr),
          _dummy(nullptr),
          _stay(false)
        {
        }
        ~array()
        {
            if (_stay || _size <= 0 || _list == nullptr) return;
            if (_list != nullptr) free((void *)_list);
            //if (_dummy != nullptr) free((void*&)_dummy);
            _size = 0;
            _list = nullptr;
            _dummy = nullptr;
        }
    public:
        T & at(int index)
        {
            if (index < 0) index = _size + index;
            if (index >= _size)
            {
                dprint_error("index was out of range");
                memset((void*)_dummy, 0, sizeof(T));
                return _dummy[0];
            }
            return _list[index];
        }
        const T & at_ro(int index) const
        {
            if (index < 0) index = _size + index;
            if (index >= _size)
            {
                dprint_error("index was out of range");
                memset((void*)_dummy, 0, sizeof(T));
                return ((const T *)_dummy)[0];
            }
            return ((const T *)_list)[index];
        }
        void clear()
        {
            memset((void *)_list, 0, sizeof(T)*_size);
        }
        void dispose()
        {
            _stay = false;
            this->~array();
        }
    public:
        ptr<T> begin()
        {
            return _list[0];
        }
        ptr<T> end()
        {
            return _list[_size];
        }
    public:
        T & operator[](int index)
        {
            return at(index);
        }
        const T & operator[](int index) const
        {
            return at_ro(index);
        }
        array<T> & operator=(array<T> right)
        {
            if (_size != 0) this->~array();
            
            this->_dummy = right._dummy;
            this->_list = right._list;
            this->_size = right._size;
            this->_stay = right._stay;
            return *this;
        }
        bool operator==(array<T> &right)
        {
            if (right._size != _size) return false;
            for (int i = 0; i < _size; i++)
                if (right._list[i] != _list[i]) return false;
            return true;
        }
        bool operator!=(array<T> &right)
        {
            return !this->operator==(right);
        }
    public:
        T * c_list() { return (T *)_list; }
        size_t size() const { return _size; }
        bool is_null() const { return _list == nullptr; }
        bool is_resident() const { return _stay; }
        array<T> & make_volatile()
        {
            _stay = false;
            return *this;
        }
        array<T> & make_resident()
        {
            _stay = true;
            return *this;
        }
    };
}