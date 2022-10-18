#pragma once

#include <core/memory.h>
#include <stdint.h>
#include <std/decl.h>
#include <core/io/debug.hpp>

namespace std
{
    template <typename T> class alloc_ptr final
    {
    public:
        T *_ptr;
        alloc_ptr(T *ptr)
        :   _ptr(ptr)
        {
        }
    };

    template<typename T> class ptr final
    {
    friend class std::alloc_ptr<T>;
    private:
        T *e;
        T *_org;
        bool _keep;

    public:
        ptr(T *ptr, bool readonly = false, bool keep = true)
        : e   (ptr),
          _org(ptr),
          _keep(keep)
        {
        }
        ptr(T &ref, bool readonly = false, bool keep = true)
        : e   (&ref),
          _org(&ref),
          _keep(keep)
        {
        }
        ptr(alloc_ptr<T> ptr)
        : e   (ptr._ptr),
          _org(ptr._ptr),
          _keep(false)
        {
        }
        ptr()
        : e   (nullptr),
          _org(nullptr),
          _keep(true)
        {
        }

        ~ptr()
        {
            /*if (!_keep) dispose();
            e = nullptr;*/
        }
    public:
        void dispose()
        {
            free((void*&)_org);
        }
        const T *c_ptr()
        {
            if (e == nullptr) fatal("tried to access an unassigned pointer");
            return (const T *)e;
        }
        bool is_null() const { return e == nullptr; }
    public:
        T * operator ->()
        {
            if (e == nullptr) fatal("tried to access an unassigned pointer");
            return e;
        }
        T & operator *()
        {
            if (e == nullptr) fatal("tried to access an unassigned pointer");
            return *e;
        }
        std::ptr<T> & operator=(T * right)
        {
            if (e == right) return *this;
            else if (right == nullptr && !_keep) dispose();
            else if (e != nullptr && !_keep) dispose();

            this->e = right;
            this->_org = right;
            return *this;
        }
        
        std::ptr<T> & operator=(std::ptr<T> right)
        {
            if (e == right.e) return *this;
            else if (right == nullptr && !_keep) dispose();
            else if (e != nullptr && !_keep) dispose();

            this->e = right.e;
            this->_org = right._org;
            this->_keep = right._keep;
            return *this;
        }
        std::ptr<T> & operator=(alloc_ptr<T> right)
        {
            if (e != nullptr || _org != nullptr) return *this;

            this->e = right._ptr;
            this->_org = right._ptr;
            this->_keep = false;
            return *this;
        }
        bool operator==(T *right)
        {
            return right == e;
        }
        bool operator==(std::ptr<T> right)
        {
            return right.e == e;
        }
        bool operator!=(T *right)
        {
            return right != e;
        }
        bool operator!=(std::ptr<T> right)
        {
            return right.e != e;
        }
        std::ptr<T> & operator+=(size_t sz)
        {
            e = (T*)(((uintptr_t)e) + sz);
            return *this;
        }
        std::ptr<T> & operator-=(size_t sz)
        {
            e = (T*)(((uintptr_t)e) - sz);
            return *this;
        }
        std::ptr<T> & operator++(int)
        {
            e++;
            return *this;
        }
        std::ptr<T> & operator--(int)
        {
            e--;
            return *this;
        }
        std::ptr<T> & operator++()
        {
            e++;
            return *this;
        }
        std::ptr<T> & operator--()
        {
            e--;
            return *this;
        }
        T & operator[](size_t ind)
        {
            return e[ind];
        }
        uint32_t operator-(std::ptr<T> &other)
        {
            return ((uintptr_t)this->e) - ((uintptr_t)other.e);
        }
    };
}