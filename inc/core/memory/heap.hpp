#pragma once

#include <std/ptr.h>
#include <std/array.h>
#include <std/stream.h>
#include <std/decl.h>
#include <stdint.h>
#include <stddef.h>

namespace system::core::memory
{
    enum class heap_type
    {
        Kernel,
        Common,
        User,
        LostFound
    };
    enum class data_type : uint8_t
    {
        UNDEFINED,
        STRING,
        ARRAY,
        STREAM,
        OBJECT,
        OTHER
    };
    struct data_t final
    {
        size_t size;
        uint16_t magic;

        bool      free: 1;
        bool     dirty: 1;
        data_type type: 6;
    } packed__;
    class heap_t final
    {
    private:
        std::stream _stream;
        size_t _entries_count;
        bool _enabled;
        bool _dirty;
    private:
        std::ptr<data_t> create_entry(size_t size);
        std::ptr<data_t> combine_entries(std::ptr<data_t> left, std::ptr<data_t> right);
        std::ptr<data_t> shrink_entry   (std::ptr<data_t> entry, size_t size);
        std::ptr<data_t> at(int ind);
    public:
        void defragment(bool silent = false);
        void *unsafe_alloc(size_t size, data_type type = data_type::UNDEFINED);
        bool free(void *data);
        template <typename T> inline bool safe_free(std::ptr<T> pointer) { return free((void*)pointer.c_ptr()); }
    public:
        void print_table();
    public:
        template<typename T> inline std::alloc_ptr<T> safe_alloc()
        {
            return std::alloc_ptr<T>((T *)unsafe_alloc(sizeof(T)));
        }
        inline bool contains(uintptr_t ptr)
        {
            return ptr > _stream.c_ptr() && ptr < _stream.c_ptr() + _stream.size();
        }
        inline size_t unused() { return _stream.size()-_stream.tell(); }
        inline size_t unused_kb() { return unused()/1024; }
        inline size_t unused_mb() { return unused()/1024/1024; }
        inline size_t unused_gb() { return unused()/1024/1024/1024; }
    public:
        inline size_t used() { return _stream.tell(); }
        inline size_t used_kb() { return used()/1024; }
        inline size_t used_mb() { return used()/1024/1024; }
        inline size_t used_gb() { return used()/1024/1024/1024; }
    public:
        inline size_t available() { return _stream.size(); }
        inline size_t available_kb() { return available()/1024; }
        inline size_t available_mb() { return available()/1024/1024; }
        inline size_t available_gb() { return available()/1024/1024/1024; }
    public:
        inline heap_t(uintptr_t ptr, size_t size, bool dirty = false)
        :   _stream(ptr, size),
            _dirty(dirty),
            _enabled(true)
        {
        }
        inline heap_t(std::stream stream, bool dirty = false)
        :   _stream(stream),
            _dirty(dirty),
            _enabled(true)
        {
        }
        inline heap_t()
        :   _stream(),
            _dirty(false),
            _enabled(false)
        {
        }
        ~heap_t()
        {
        }
    public:
        inline std::ptr<const data_t> operator[](int i) { return at(i).c_ptr(); }
    public:
        inline void init(uintptr_t ptr, size_t size, bool dirty = false)
        {
            if (_enabled) return;
            _stream.init(ptr, size);
            _enabled = true;
            _dirty = dirty;
            _entries_count = 0;
        }
    };
}
