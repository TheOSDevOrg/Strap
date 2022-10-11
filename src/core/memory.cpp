#include <core/memory.h>
#include <core/kernel.hpp>
#include <core/env.hpp>

using namespace system::core::memory;

__cdecl void memcpyd(uint32_t *dst, uint32_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
__cdecl void memsetd(uint32_t *dst, uint32_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

__cdecl void memcpyw(uint16_t *dst, uint16_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
__cdecl void memsetw(uint16_t *dst, uint16_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

__cdecl void memcpyb(uint8_t *dst, uint8_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
__cdecl void memsetb(uint8_t *dst, uint8_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

__cdecl void memcpy(void *dst, void *src, size_t sz)
{
    int i;
    int s,k;
    
    s = sz/4;
    sz -= (k  = s*4);
    memcpyd((uint32_t*)dst, (uint32_t*)src, s);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    s = sz/2;
    sz -= (k = s*2);
    memcpyw((uint16_t*)dst, (uint16_t*)src, s);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    memcpyb((uint8_t*)dst, (uint8_t*)src, sz);
}
__cdecl void memset(void *dst, int data, size_t sz)
{
    if (data < UINT8_MAX) memsetb((uint8_t*)dst, (uint8_t)data, sz);
    else if (data < UINT16_MAX) memsetw((uint16_t*)dst, (uint16_t)data, sz/2);
    else memsetd((uint32_t*)dst, (uint32_t)data, sz/4);
}

__cdecl void * kalloc(size_t sz)
{
    return system::kernel::heap.unsafe_alloc(sz);
}
__cdecl bool kfree(void *data)
{
    return system::kernel::heap.free(data);
}
__cdecl void * krealloc(void *data, size_t sz)
{
    if (!system::kernel::heap.contains((uintptr_t)data))
    {
        dprint_warn("tried to realloc a pointer outside of the kernel heap");
        return data;
    }
    
    std::ptr<data_t> e = std::ptr<data_t>((data_t*)((uintptr_t)data-sizeof(data_t)));
    void *d = system::kernel::heap.unsafe_alloc(sz);
    
    size_t off = sz > e->size ? e->size : sz;
    memcpy(d, data, off);
    system::kernel::heap.free(data);

    return d;
}

__cdecl void * malloc(size_t sz)
{
    return system::env::common_heap.unsafe_alloc(sz);
}
__cdecl bool mfree(void *data)
{
    return system::env::common_heap.free(data);
}
__cdecl void * mrealloc(void *data, size_t sz)
{
    if (!system::env::common_heap.contains((uintptr_t)data))
    {
        dprint_warn("tried to realloc a pointer outside of the kernel heap");
        return data;
    }
    
    std::ptr<data_t> e = std::ptr<data_t>((data_t*)((uintptr_t)data-sizeof(data_t)));
    void *d = system::env::common_heap.unsafe_alloc(sz);
    
    size_t off = sz > e->size ? e->size : sz;
    memcpy(d, data, off);
    system::env::common_heap.free(data);

    return d;
}

__cdecl void * alloc(size_t sz)
{
    return system::env::current_heap->unsafe_alloc(sz);
}
__cdecl bool free(void *data)
{
    bool r = system::env::current_heap->free(data);
    system::env::current_heap->defragment();
    return r;
}
__cdecl bool freelst(void **lst)
{
    return false;
    if (!system::env::current_heap->contains((uintptr_t)lst)) return false;
    size_t sz = ((data_t*)((uintptr_t)lst-sizeof(data_t)))->size/sizeof(void*);
    for (int i = 0; i < sz; i++) if (system::env::current_heap->contains((uintptr_t)lst[i])) system::env::current_heap->free(lst[i]);
    return system::env::current_heap->free(lst);
}
__cdecl void * realloc(void *data, size_t sz)
{
    if (!system::env::current_heap->contains((uintptr_t)data))
    {
        dprint_warn("tried to realloc a pointer outside of the kernel heap");
        return data;
    }
    
    std::ptr<data_t> e = std::ptr<data_t>((data_t*)((uintptr_t)data-sizeof(data_t)));
    void *d = system::env::current_heap->unsafe_alloc(sz);
    
    size_t off = sz > e->size ? e->size : sz;
    memcpy((void *)d, (void *)data, off);
    system::env::current_heap->free(data);

    return d;
}