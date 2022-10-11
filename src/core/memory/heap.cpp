#include <core/memory/heap.hpp>
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <core/io/debug.hpp>
#include <std/cstring.h>
#include <core/kernel.hpp>

#define MAGIC 0xAEFB

using namespace std;
using namespace system::core::memory;

ptr<data_t> heap_t::create_entry(size_t sz)
{
    //char s[3];
    //numstr(sz, 10, s);
    //dprint_warn(s);
    //numstr(_entries_count, 10, s);
    //dprint_error(s);

    if (!_enabled) return ptr<data_t>();
    else if (sz == 0) return nullptr;

    ptr<data_t> n = ptr<data_t>(nullptr, false, true);
    
    if (_stream.size() - _stream.tell() <= sizeof(data_t) + sz) return (data_t *)nullptr;

    _stream.frame_as(n);

    n->free = true;
    n->size = sz;
    n->type = data_type::UNDEFINED;
    n->dirty = true;
    n->magic = MAGIC;

    _stream.seek(_stream.tell() + sz);

    _entries_count++;
    dprint_info("a new heap entry was created");
    return n;
}
ptr<data_t> heap_t::combine_entries(ptr<data_t> left, ptr<data_t> right)
{
    if (left->magic != MAGIC)
    {
        fatal("tried to combine two non-entries");
        return left;
    }
    //if (!dbgio::serial) return left;

    
    ptr<data_t> after = right;
    after += sizeof(data_t)+right->size;
    left->size += after-right;
    left->dirty = true;
    left->free = true;
    left->type = data_type::UNDEFINED;

    _entries_count--;
    dprint_debug("two heap entries were combined");
    return left;
}
ptr<data_t> heap_t::shrink_entry(ptr<data_t> e, size_t s)
{
    //return nullptr;
    if (e.is_null()) return nullptr;
    if (e->magic != MAGIC)
    {
        fatal("tried to shrink a non-entry");
        return e;
    }

    if (e->size == s) return e;

    size_t sz = e->size;
    ptr<data_t> n = e;

    if (sz - s <= sizeof(data_t) || sz - s <= 0 || s > sz) return nullptr;
    
    e->size = s;
    n += s + sizeof(data_t);
    n->dirty = true;
    n->free = true;
    n->size = sz-s-sizeof(data_t);
    n->type = data_type::UNDEFINED;
    n->magic = MAGIC;

    dprint_info("a heap entry was shrank");
    _entries_count++;
    return e;
}
ptr<data_t> heap_t::at(int i)
{
    ptr<data_t> _current;
    size_t pos = 0;
    if (i >= _entries_count) return ptr<data_t>(nullptr);
    for (int y = -1; y != i; y++)
    {
        _stream.frame_at_as(pos, _current);
        pos += sizeof(data_t) + _current->size;
    }
    //if (_current->magic != MAGIC) return ptr<data_t>(nullptr);
    return _current;
}
void heap_t::defragment(bool silent)
{
    ptr<data_t> _prev;
    ptr<data_t> _curr;
    size_t _pos = 0;

    size_t sts = 0;

    for (int y = 0; y < _entries_count; y++)
    {
        _stream.frame_at_as(_pos, _curr);
        
        sts = _curr->size; 

        if (_prev.is_null()) goto end;
        
        //if (_curr->size == 0) { _entries_count = y; goto end; }

        if (_prev->free && _curr->free)
        {
            //if(dbgio::serial) system::kernel::ttys[1]->write_line("combining entries");
            _curr = combine_entries(_prev, _curr);
            y--;
        }
    end:
        _prev = _curr;
        _pos  += sts + sizeof(data_t);
    }

    if (!silent) dprint_info("heap defragmentation complete");
}

void *heap_t::unsafe_alloc(size_t size, data_type type)
{
    defragment(true);
    if (size == 0) return nullptr;
    ptr<data_t> _curr = ptr<data_t>(nullptr, false, true);
    size_t _pos = 0;

    for (int y = 0; y < _entries_count; y++)
    {
        _stream.frame_at_as(_pos, _curr);

        if (_curr.is_null()) break;

        size_t sts = _curr->size;

        if (_curr->free && _curr->size >= size && _curr->magic == MAGIC)
        {
            _curr = shrink_entry(_curr, size);
            if (!_curr.is_null()) break;
        }

        _pos += sizeof(data_t)+sts;
    }
    
    if (_curr.is_null() || _curr->size < size || !_curr->free) _curr = create_entry(size);
    void *res = (void*)(((uintptr_t)_curr.c_ptr()) + sizeof(data_t));

    if (_curr == nullptr)
    {
        fatal("ALLOC: no memory left for allocation");
        return nullptr;
    }

    _curr->free = false;
    _curr->type = type;
    
    dprint_info("allocated new entry");

    if (_curr->dirty) memset(res, 0, size);
    return res;
}
bool heap_t::free(void *data)
{
    if (data == nullptr)
    {
        dprint_warn("tried to free nullptr");
        return false;
    }
    else if (!contains((uintptr_t) data))
    {
        dprint_warn("tried to free an address outside the heap");
        return false;
    }

    ptr<data_t> _entry = ptr<data_t>((data_t*)(((uintptr_t)data)-sizeof(data_t)));

    if (_entry->free)
    {
        dprint_info("tried to free an already free entry");
        return false;
    }
    else if (_entry->magic != MAGIC)
    {
        dprint_warn("tried to free a non-entry");
        return false;
    }

    _entry->dirty = true;
    _entry->free = true;

    dprint_info("free'd entry");
    defragment();
    return true;
}
void heap_t::print_table()
{
    defragment();

    char s[10];

    uint32_t used = 0;
    uint32_t zero_sz = 0;
    uint32_t invalid_sz = 0;
    for (int i = 0; i < _entries_count; i++)
    {
        auto e = at(i);
        if (!e->free) used++;
        if (e->size <= 0) zero_sz++;
        if (e->magic != MAGIC) invalid_sz++;
    }

    numstr(used, 10, s);
    system::kernel::ttys[2]->write(s);

    system::kernel::ttys[2]->write('/');

    uint32_t u = _entries_count;

    numstr(_entries_count, 10, s);
    system::kernel::ttys[2]->write(s);

    system::kernel::ttys[2]->write(" entries used (");

    numstr(u-used, 10, s);
    system::kernel::ttys[2]->write(s);

    system::kernel::ttys[2]->write_line(" free)");

    numstr(zero_sz, 10, s);
    system::kernel::ttys[2]->write(s);
    system::kernel::ttys[2]->write_line(" entries with size 0");

    numstr(invalid_sz, 10, s);
    system::kernel::ttys[2]->write(s);
    system::kernel::ttys[2]->write_line(" invalid entries");

    system::kernel::current_tty->write_line("visualizer:");
    for (int i = 0; i < _entries_count; i++)
    {
        char s[4];
        auto e = at(i);
        numstr(e->size, 10, s);
        system::kernel::current_tty->write(s);
        if(!e->free) system::kernel::current_tty->write('*');
        system::kernel::current_tty->write(" ");
    }
    system::kernel::current_tty->write_line("");
}