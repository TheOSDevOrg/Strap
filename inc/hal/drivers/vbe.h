#pragma once
#include <stdint.h>
#include <std/decl.h>
#include <boot/mboot.h>
#include <gfx/colors.hpp>
#include <gfx/gfx_common.hpp>
#include <gfx/font.h>
#include <gfx/doublebuffer.hpp>
#include <hal/drivers/realmode.h>
#ifdef __cplusplus

#define __RGB_COLOR_(r, g, b) ((r << 16) | (g << 8) | b)

namespace system::hal::drivers::vbe
{
    class Driver
    {
        private:
        system::kernel::gfx::buffer_info_t vbe_buffer;
        bool running = false;
        public:
            void Init(system::kernel::gfx::buffer_info_t buffer);
            void SetMode(int w, int h);
            void SetPixel(uint32_t x,uint32_t y, uint32_t color);
            void FilledRect(int x, int y, int w, int h, uint32_t color);
            void DrawChar(int x, int y, char c, system::kernel::gfx::Font font, uint32_t fg, uint32_t bg);
            void DrawString(int x, int y, char* str, system::kernel::gfx::Font font, uint32_t fg, uint32_t bg);
            void Clear();
            void Clear(uint32_t color);
            void Clear(VBE_COLOR color);
            void Render();
            void Disable();
        private:
            void FillHeaders();
            system::grub::vbe_ctrl_info_t CtrlInfo;
            system::grub::vbe_mode_info_t ModeInfo;
            uint32_t Size;
    };
 }
#endif