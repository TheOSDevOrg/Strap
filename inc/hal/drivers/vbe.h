#pragma once
#include <stdint.h>
#include <std/decl.h>
#include <boot/mboot.h>
#include <gfx/colors.hpp>
#include <gfx/gfx_common.hpp>
#include <gfx/font.h>
#ifdef __cplusplus

namespace system::hal::drivers::vbe
{
    typedef struct
    {
        uint16_t di, si, bp, sp, bx, dx, cx, ax;
        uint16_t gs, fs, es, ds, eflags;
    } packed__ registers16_t;


    __cdecl { extern void int32(uint8_t interrupt, system::hal::drivers::vbe::registers16_t* regs); }

    class Driver
    {
        public:
            void Init();
            void SetMode(int w, int h);
            void SetPixel(uint32_t x,uint32_t y, uint32_t color);
            void FilledRect(int x, int y, int w, int h, uint32_t color);
            void DrawChar(int x, int y, char c, system::kernel::gfx::Font font, uint32_t fg, uint32_t bg);
            void DrawString(int x, int y, char* str, system::kernel::gfx::Font font, uint32_t fg, uint32_t bg);
            void Clear();
            void Clear(uint32_t color);
            void Clear(VBE_COLOR color);
            void Render();
        private:
            void FillHeaders();
            system::grub::vbe_ctrl_info_t CtrlInfo;
            system::grub::vbe_mode_info_t ModeInfo;
            uint32_t Size;
    };
 }
#endif