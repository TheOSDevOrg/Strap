#pragma once

#include <stdint.h>
#include <core/memory/heap.hpp>
#include <std/array.h>
#include <std/ptr.h>

namespace system::env
{
    extern system::core::memory::heap_t *current_heap;
    
    extern system::core::memory::heap_t common_heap;
    extern system::core::memory::heap_t lostfound_heap;
}