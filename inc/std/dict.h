#pragma once


#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
#include <std/ptr.h>
#include <std/arraylist.h>
#include <std/cstring.h>

namespace std
{
    template<class TKey, class TVal> class dictionary final
    {
    private:
        arraylist<TKey> _keys;
        arraylist<TVal> _values;
        TVal *_dummy;
    public:
        dictionary(size_t limit = 0, bool stay = false)
        : _keys(limit, stay),
          _values(limit, stay)
        {
            _dummy = nullptr;
        }
        ~dictionary()
        {
            _keys.~arraylist();
            _values.~arraylist();
            //free((void *&)_dummy);
            //_dummy = nullptr;
        }
    public:
        TVal & at(TKey key)
        {
            ptr<TKey> it_key = _keys.begin();
            ptr<TKey> end_key = _keys.end();

            ptr<TVal> it_val = _values.begin();
            ptr<TVal> end_val = _values.end();

            for (; it_key != end_key && it_val != end_val; it_key++, it_val++)
            {
                if (key == *it_key) return *it_val;
            }
            
            dprint_warn("tried to access unknown dictionary key");
            memset((void *)_dummy, 0, sizeof(TVal));
            return _dummy[0];
        }
        void remove(TKey key)
        {
            char s[1000];
            ptr<TKey> it_key = _keys.begin();
            ptr<TKey> end_key = _keys.end();

            int i;
            for (i = 0; it_key != end_key; it_key++, i++)
            {
                if (key == *it_key)
                {
                    dprint_info("found");
                    break;
                }
            }

            _keys.remove_at(i);
            _values.remove_at(i);
        }
        void add(TKey key, TVal value)
        {
            ptr<TKey> it = _keys.begin();
            ptr<TKey> end = _keys.end();
            for (int i = 0; it != end; i++, it++)
            {
                auto k = *it;
                if (k == key) 
                {
                    dprint_warn("dictionary key already exists");
                    return;
                }
            }
            _keys.add(key);
            _values.add(value);
            
            dprint_debug("added dictionary key");
        }
    public:
        size_t size() { return _keys.size(); }
        arraylist<TKey> & keys() { return _keys; }
        arraylist<TVal> & values() { return _values; }
    public:
        TVal & operator[](TKey key) { return at(key); }
    public:
        void dispose()
        {
            _keys.dispose();
            _values.dispose();
        }
    };
}