#include <gfx/doublebuffer.hpp>
#include <core/memory.h>

namespace system::kernel::gfx
{
    buffer_info_t vbe_buffer_init(uint32_t w,uint32_t h)
    {
        buffer_info_t _buffer;
        _buffer.buffer = (uint32_t*)malloc(w*h*4);
        _buffer.width = w;
        _buffer.height = h;
        _buffer.size = w*h*4;
        return _buffer;
    }
}