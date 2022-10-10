#pragma once

#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
#include <std/ptr.h>
#include <std/cstring.h>

#define MAX_EXTRA 3
#define MAGIC 0xFAFB

namespace std
{
    template <typename T> class arraylist final
    {
    private:
        size_t _count;
        size_t _limit;
        size_t _extra;
        bool   _limited;
        uint16_t _magic;
        bool   _stay;
    private:
        T *_dummy;
        T *_list;
    public:
        arraylist(size_t limit = 0, bool stay = false)
        : _limit(limit),
          _limited(limit != 0),
          _list(nullptr),
          _count(0),
          _extra(0),
          _magic(0),
          _stay(stay)
        {
            _dummy = nullptr;
            _list = nullptr;
        }
        ~arraylist()
        {
            if (_stay) return;
            if (_magic == MAGIC && _list != nullptr)
            {
                free((void *)_list);
                //free((void *&)_dummy);
            }
            _list = nullptr;
            _dummy = nullptr;
            _limit = 0;
            _limited = false;
            _count = 0;
        }
    public:
        void add(T object)
        {
            if (_magic != MAGIC)
            {
                dprint_debug("init list");
                _magic = MAGIC;
                _list = (T *)alloc(sizeof(T));
                _dummy = nullptr;
                _extra++;
            }

            if (_count+1 >= _limit && _limited)
            {
                dprint_warn("reached maximum arraylist size");
                return;
            }
            
            if (!_extra)
            {
                _list = (T *)realloc((void *)_list, (++_count)*sizeof(T));
                _list[_count-1] = object;
            }
            else
            {
                _list[_count++] = object;
                _extra--;
            }

            dprint_debug("added element to list");
        }
        void remove_at(int index)
        {
            if (_count == 0) return;
            if (index >= _count) return;
            if (_count == 1) return clear();

            _count--;
            for (int i = index; i < _count; i++)
            {
                _list[i] = _list[i+1];
            }

            _extra++;
            if (_extra > MAX_EXTRA)
            {
                _extra--;
                _list = (T *)realloc((void *)_list, (_count+_extra)*sizeof(T));
            }
        }
        void insert(T object, int index)
        {
            if (_count < index) return;
            else if (_count == index) return add(object);
            else if (_count+1 >= _limit && _limited)
            {
                dprint_warn("reached maximum arraylist size");
                return;
            }

            if (_extra == 0) 
            {
                _extra++;
                _list = (T *)realloc((void *)_list, (_count+1)*sizeof(T));
            }
            _extra--, _count++;
            for (int i = _count-1; i >= index; i--) _list[i] = _list[i-1];

            _list[index] = object;
        }
        int find(T object)
        {
            for (int i = 0; i < _count; i++)
                if (_list[i] == object) return i;
            return -1;
        }
    public:
        T & at(int index)
        {
            if (index < 0) index = _count + index;
            if (index >= _count)
            {
                dprint_error("index was out of range");
                memset((void*)_dummy, 0, sizeof(T));
                return _dummy[0];
            }
            return _list[index];
        }
        const T & at_ro(int index) const
        {
            if (index < 0) index = _count + index;
            if (index >= _count)
            {
                dprint_error("index was out of range");
                memset((void*)_dummy, 0, sizeof(T));
                return ((const T *)_dummy)[0];
            }
            return ((const T *)_list)[index];
        }
        void clear()
        {
            if (_magic == MAGIC || _list != nullptr)
            {
                free((void *)_list);
                //free((void *&)_dummy);
            }
            _dummy = nullptr;
            _list = nullptr;
            _count = 0, _extra = 0, _magic = 0;
        }
        void dispose()
        {
            this->_stay = false;
            this->~arraylist();
        }
    public:
        void clone(arraylist<T> &right)
        {
            this->clear();
            for (auto e : right)
                this->add(e);
        }
    public:
        ptr<T> begin()
        {
            return _list[0];
        }
        ptr<T> end()
        {
            return _list[_count];
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
        arraylist<T> operator=(arraylist<T> right)
        {
            if(_magic == MAGIC) this->~arraylist();

            this->_dummy = right._dummy;
            this->_count = right._count;
            this->_list = right._list;
            this->_limit = right._limit;
            this->_limited = right._limited;
        }
    public:
        size_t size() const { return _count; }
        T * c_list() { return (T *)_list; }
    public:
        void make_volatile()
        {
            _stay = false;
        }
    };
}

#undef MAX_EXTRA
#undef MAGIC