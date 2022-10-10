#pragma once
#include <stdint.h>
static inline bool bit_addr_from_byte(uint8_t to_convert, int to_return)
{
    int mask = 1 << (to_return - 1);
    return ((to_convert & mask) != 0);
}