#pragma once
#include <stdint.h>
#include <std/decl.h>
namespace system::kernel::gfx
{
    typedef struct 
    {
        uint32_t* buffer;
        uint32_t width;
        uint32_t height;
        uint32_t size;
    }buffer_info_t;

    buffer_info_t  vbe_buffer_init(uint32_t w, uint32_t h);
}