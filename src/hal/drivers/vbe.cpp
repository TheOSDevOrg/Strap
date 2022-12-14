#include <hal/drivers/vbe.h>
#include <core/io/debug.hpp>
#include <core/memory.h>
#include <core/kernel.hpp>
#define VBE_CTRL_PTR 0x80000
using namespace system::kernel::gfx;
using namespace system::hal::drivers::realmode;
namespace system::hal::drivers::vbe
{
    void Driver::Init(system::kernel::gfx::buffer_info_t buffer)
    {
        if (running) return;
        SetMode(800,600);
        vbe_buffer = buffer;
    }
    void Driver::SetMode(int w, int h)
    {
        FillHeaders();
        registers16_t regs;
        memset(&regs, 0, sizeof(registers16_t));
        
        system::grub::vbe_mode_info_t* minfo = (system::grub::vbe_mode_info_t*)(VBE_CTRL_PTR + sizeof(system::grub::vbe_ctrl_info_t) + 512);
        uint16_t* modes = (uint16_t*)REAL_PTR(CtrlInfo.videomode);
        uint16_t mode;

        for (int i = 0; modes[i] != 0xFFFF; i++)
        {
            regs.ax = 0x4F01;
            regs.cx = modes[i];
            regs.es = SEG(minfo);
            regs.di = OFF(minfo);
            int32(0x10, &regs);

            if (minfo->width == w && minfo->height == h && minfo->depth == 32)
            {
                mode = modes[i];
                regs.ax = 0x4F02;
                regs.bx = mode | 0x4000;
                int32(0x10, &regs);
                ModeInfo = *minfo;
                Size     = ModeInfo.width * ModeInfo.height * 4;
                return;
            }
        }
    }
    void Driver::FillHeaders()
    {
        memset(&ModeInfo, 0, sizeof(system::grub::vbe_mode_info_t));
        memset(&CtrlInfo, 0, sizeof(system::grub::vbe_ctrl_info_t));
        system::grub::vbe_ctrl_info_t* info = (system::grub::vbe_ctrl_info_t*)VBE_CTRL_PTR;
        registers16_t regs;

        memset(&regs, 0, sizeof(registers16_t));
        regs.ax = 0x4F00;
        regs.es = 0x8000;
        regs.di = 0x0000;
        int32(0x10, &regs);

        if (regs.ax != 0x4F) { dprint_error("Failed to initialize VBE controller"); return; }
        CtrlInfo = *info;
        ModeInfo = *(system::grub::vbe_mode_info_t*)system::grub::get()->vbe_mode_info;
        Size     = ModeInfo.width * ModeInfo.height * 4;
    }
    void Driver::SetPixel(uint32_t x,uint32_t y,uint32_t color)
    {
        //Set Pixel using Pitch
        vbe_buffer.buffer[y * (ModeInfo.pitch / 4) + x] = color;
    }
    void Driver::FilledRect(int x, int y, int w, int h, uint32_t color)
    {
        for (int i = 0; i < w * h; i++) { SetPixel(x + (i % w), y + (i / w), color); }
    }
    void Driver::DrawChar(int x, int y, char c, system::kernel::gfx::Font font, uint32_t fg, uint32_t bg)
    {
        if (bg != 0x00FFFFFF) { FilledRect(x, y, font.Width, font.Height, bg); }
        uint32_t p = font.Height * c;

        int i = 0;
        while (i < font.Width * font.Height)
        {
            int xx = x + (font.Width - (i % font.Width));
            int yy = y + (i / font.Width);
            if (bit_addr_from_byte(font.Data[p + (i / font.Width)], (i % font.Width) + 1)) { SetPixel(xx, yy, fg); }
            i++;
        }
    }
    void Driver::DrawString(int x, int y, char* str, system::kernel::gfx::Font font, uint32_t fg, uint32_t bg)
    {
        if (str == NULL) { return; }

        int xx = x, yy = y, i = 0;
        while (str[i] != 0)
        {
            if (str[i] == '\n') { xx = x; yy += font.Height; }
            else { DrawChar(xx, yy, str[i], font, fg, bg); xx += font.Width; }
            i++;
        }
    }
    void Driver::Clear()
    {
        memset(vbe_buffer.buffer,0,Size);
    }
    void Driver::Clear(uint32_t Color)
    {
        memset(vbe_buffer.buffer,Color,Size);
    }
    void Driver::Clear(VBE_COLOR Color)
    {
        memset(vbe_buffer.buffer,(uint32_t)Color,Size);
    }
    void Driver::Render()
    {
        memcpy((uint32_t*)ModeInfo.physical_base,vbe_buffer.buffer,Size);
    }
    void Driver::Disable()
    {
        registers16_t regs;
        memset(&regs, 0, sizeof(registers16_t));
        regs.ax = 0x4F02;
        regs.bx = 0x03;
        int32(0x10, &regs);
        system::kernel::current_tty->write_line("VBE Driver Disabled");
    }

}