#pragma once

#include <std/arraylist.h>
#include <std/hash.h>
#include <stddef.h>
#include <stdint.h>
#include <core/memory.h>
#include <std/ptr.h>
#include <std/cstring.h>

namespace std
{
    template<class T> class base_string final
    {
    private:
        arraylist<T> _buffer;
        uint32_t _hash;
        bool _stay;
    public:
        base_string(size_t limit = 0, bool stay = false)
        : _buffer(limit, true),
          _stay(stay)
        {
            _hash = 0;
            _buffer.add(0);
        }
        base_string(char str[], bool stay = false)
        : _buffer(0, true),
          _stay(stay)
        {
            while (*str) _buffer.add(*(str++)); 
            _buffer.add(0);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        ~base_string()
        {
            if (_stay) return;
            _buffer.dispose();
            _hash = 0;
        }
    public:
        void dispose()
        {
            _stay = false;
            this->~base_string();
        }
    public:
        void clear()
        {
            _buffer.clear();
            _buffer.add(0);
        }
    public:
        void insert(T str[], int index)
        {
            int sz = strlen((const char *)str);
            for (int i = 0; i < sz; i++)
                _buffer.insert(str[i], index+i);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void insert(base_string<T> str, int index)
        {
            int i = 0;
            for (auto c : str)
            {
                _buffer.insert(c, index+i);
                i++;
            }
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void insert(T c, int index)
        {
            _buffer.insert(c, index);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void add(T str[])
        {
            int sz = strlen((const char *)str);
            for (int i = 0; i < sz; i++)
                _buffer.insert(str[i], _buffer.size()-1);
            _buffer.at(_buffer.size()-1) = 0;
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void add(base_string<T> str)
        {
            for (auto c : str)
                _buffer.insert(c, _buffer.size()-1);
            _buffer.at(_buffer.size()-1) = 0;
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void to_upper()
        {
            auto it = _buffer.begin();
            auto ed = _buffer.end();
            for (; it != ed; it++)
            {
                T d;
                if((d = *it) >= 'a' && d <= 'z') *it -= (T)' ';
            }
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void to_lower()
        {
            auto it = _buffer.begin();
            auto ed = _buffer.end();
            for (; it != ed; it++)
            {
                T d;
                if((d = *it) >= 'A' && d <= 'Z') *it += (T)' ';
            }
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void add(T c)
        {
            _buffer.insert(c, _buffer.size()-1);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove(T c)
        {
            int i = find(c);
            if (i != -1) _buffer.remove_at(i);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove(T str[])
        {
            int i = find(str), sz = strlen(str);
            for (int x = 0; x < sz; x++, i++) _buffer.remove_at(i--);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove(base_string<T> str)
        {
            int i = find(str), sz = str.size();
            for (int x = 0; x < sz; x++, i++) _buffer.remove_at(i--);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove_all(char c)
        {
            int i;
            while ((i = find(c)) != -1) _buffer.remove_at(i);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove_all(T str[])
        {
            int i, sz = strlen(str);
            while ((i = find(str)) != -1)
                for (int x = 0; x < sz; x++, i++) _buffer.remove_at(i--);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove_all(base_string<T> str)
        {
            int i, sz = str.size();
            while ((i = find(str)) != -1)
                for (int x = 0; x < sz; x++, i++) _buffer.remove_at(i--);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        void remove_at(int index)
        {
            _buffer.remove_at(index);
            _hash = hash_str((const void *)_buffer.c_list());
        }
        int find(T str[])
        {
            size_t sz = strlen(str);
            size_t tsz = _buffer.size();
            T *ptr = _buffer.c_list();

            for (int i = 0; i + sz < tsz; i++)
            {
                if (!strcmpis((const char *)str, (const char *)ptr, 0, i, sz))
                    return i;
            }

            return -1;
        }
        int find(base_string<T> str)
        {
            return find(str.c_str());
        }
        int find(T c)
        {
            int i = 0;
            for (auto a : *this)
            {
                if (a == c) return i;
                i++;
            }
            return -1;
        }
        void split(T separator, arraylist<base_string<T>> &out, bool stay = false)
        {
            if (find(separator) == -1)
            {
                out.add(*this);
                return;
            }

            base_string<T> temp = base_string<T>((size_t)0, stay);

            for (auto c : *this)
            {
                if (c == separator)
                {
                    out.add(base_string<T>(temp.c_str(), stay));
                    temp = "";
                    continue;
                }
                else if (c == '\0') break;
                temp.add(c);
            }
            if (temp != "") out.add(base_string<T>(temp.c_str(), stay));
            temp.dispose();
        }
        int to_number(int base = 10)
        {
            return strnum(_buffer.c_list(), base);
        }
        static base_string<T> from_number(int number, int base = 10)
        {
            int sz = numlen(number);
            base_string<T> result = base_string<T>();
            for (int i = 0; i < sz+1; i++) result._buffer.add(0);
            numstr(number, base, result._buffer.c_list());
            return result;
        }
    public:
        ptr<T> begin() { return _buffer.begin(); }
        ptr<T> end()   {  return _buffer.end();  }
    public:
        base_string<T> & operator=(char *str)
        {
            _buffer.clear();
            while (*str) _buffer.add(*str++); 
            _buffer.add(0);
            _hash = hash_str((const void *)_buffer.c_list());
            return *this;
        }
        base_string<T> & operator=(base_string<T> &right)
        {
            _buffer.clone(right._buffer);
            _stay = right._stay;
            _hash = right._hash;
            return *this;
        }
        bool operator==(char *str)
        {
            return this->_hash == hash_str((const void *)str);
        }
        bool operator==(base_string<T> &right)
        {
            return _hash == right._hash;
        }
        bool operator!=(char *str)
        {
            return !(this->operator==(str));
        }
        bool operator!=(base_string<T> &right)
        {
            return !(this->operator!=(right));
        }
        T & operator[](int index)
        {
            return _buffer.at(index);
        }
    public:
        T * c_str() { return (T *)_buffer.c_list(); }
        size_t size() { return _buffer.size()-1; }
        uint32_t hash() { return _hash; }
    public:
        bool is_resident() { return _stay; }
        base_string<T> & make_volatile() { _stay = false; return *this; }
        base_string<T> & make_resident() { _stay = true; return *this; }
    };
    typedef base_string<char> string;
}