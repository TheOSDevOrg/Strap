#pragma once
#include <stdint.h>
#include <std/decl.h>

#ifdef __cplusplus

namespace system::hal::drivers::vbe
{
    struct vbe_mode_block
    {
        uint16_t attrs;
        uint8_t  win_a, win_b;
        uint16_t gran;
        uint16_t win_size;
        uint16_t seg_a, seg_b;
        uint16_t winfunc[2];
        uint16_t pitch, width, height;
        uint8_t  char_width, char_height, planes, depth, banks;
        uint8_t  memory_model, bank_size, image_pages;
        uint8_t  reserved0;
        uint8_t  red_mask, red_position;
        uint8_t  green_mask, green_position;
        uint8_t  blue_mask, blue_position;
        uint8_t  rsv_mask, rsv_position;
        uint8_t  direct_color;
        uint32_t phy_addr;
        uint32_t reserved1;
        uint16_t reserved2;
    } packed__;
}

#endif