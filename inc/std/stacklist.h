#pragma once

#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
#include <std/ptr.h>
#include <std/arraylist.h>
#include <std/array.h>
#include <std/ptr.h>

#define MAX_EXTRA 3

namespace std
{
    template<typename T> class stacklist final
    {
    private:
        size_t _count;
        size_t _limit;
        size_t _extra;
        bool _limited;
        T * _list;
        T * _dummy;
    public:
        stacklist(size_t limit = 0)
        : _count(0),
          _list(nullptr),
          _dummy(nullptr),
          _limit(limit),
          _limited(limit != 0),
          _extra(1)
        {
            _list = (T *)alloc(sizeof(T));
        }
        ~stacklist()
        {
            if (_list != nullptr) freelst((void **&)_list);
            //free((void *&)_dummy);
            _count = 0;
            _extra = 0;
            _limit = 0;
            _limited = false;
            _list = nullptr;
            _dummy = nullptr;
        }
    public:
        void push(T object)
        {
            if (_limited && _count+1 >= _limit)
            {
                dprint_warn("reached maximum stack size");
                return;
            }

            if (!_extra)
            {
                _list = (T *)realloc((void *)_list, (++_count)*sizeof(T));
                memcpy((void*)_list+(_count-1), (void*)&object, sizeof(T));
            }
            else
            {
                memcpy((void*)_list+((++_count)-1), (void*)&object, sizeof(T));
                _extra--;
            }
        }
        T pop()
        {
            if (_count == 0)
            {
                dprint_warn("stack bottom. Avoided stack corruption");
                memset((void *)_dummy, 0, sizeof(T));
                return *_dummy;
            }

            memcpy((void *)_dummy, (void *)&_list[--_count], sizeof(T));

            _extra++;
            if (_extra > MAX_EXTRA)
            {
                _extra--;
                _list = (T *)realloc((void *)_list, (_count+_extra)*sizeof(T));
            }

            return *_dummy;
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
            _list = realloc((void *)_list, 1*sizeof(T));
            _count = 0;
            _extra = 1;
        }
        void dispose()
        {
            this->~stacklist();
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
            this->~arraylist();

            this->_dummy = right._dummy;
            this->_count = right._count;
            this->_list = right._list;
            this->_limit = right._limit;
            this->_limited = right._limited;
        }
    public:
        size_t size() const { return _count; }
        T * c_list() { return (T *)_list; }
    };
}

#undef MAX_EXTRA