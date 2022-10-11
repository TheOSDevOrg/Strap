#pragma once
#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>


__cdecl void * kalloc  (size_t sz);
__cdecl bool   kfree   (void *data);
__cdecl void * krealloc(void *data, size_t sz);

__cdecl void * malloc  (size_t sz);
__cdecl bool   mfree   (void *data);
__cdecl void * mrealloc(void *data, size_t sz);

__cdecl void * alloc  (size_t sz);
__cdecl bool   free   (void *data);
__cdecl bool   freelst(void **lst);
__cdecl void * realloc(void *data, size_t sz);

__cdecl void memcpyd(uint32_t *dst, uint32_t *src, size_t sz);
__cdecl void memsetd(uint32_t *dst, uint32_t data, size_t sz);

__cdecl void memcpyw(uint16_t *dst, uint16_t *src, size_t sz);
__cdecl void memsetw(uint16_t *dst, uint16_t data, size_t sz);

__cdecl void memcpyb(uint8_t  *dst, uint8_t  *src, size_t sz);
__cdecl void memsetb(uint8_t  *dst, uint8_t  data, size_t sz);

__cdecl void memcpy(void *dst, void *src, size_t sz);
__cdecl void memset(void *dst, int  data, size_t sz);

#ifdef __cplusplus
namespace system::core::memory
{
    namespace kernel
    {
        void * alloc  (size_t sz);
        void   free   (void *data);
        void * realloc(void *data, size_t sz);
    }
    namespace common
    {
        void * alloc  (size_t sz);
        void   free   (void *data);
        void * realloc(void *data, size_t sz);
    }
}
#endif