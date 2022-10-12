#include <std/decl.h>
#include <stdint.h>

#define LNG_PTR(seg, off) ((seg << 4) | off)
#define REAL_PTR(arr) LNG_PTR(arr[1], arr[0])
#define SEG(addr) (((uint32_t)addr >> 4) & 0xF000)
#define OFF(addr) ((uint32_t)addr & 0xFFFF)

namespace system::hal::drivers::realmode
{
    typedef struct
    {
        uint16_t di, si, bp, sp, bx, dx, cx, ax;
        uint16_t gs, fs, es, ds, eflags;
    } packed__ registers16_t;
    __cdecl { extern void int32(uint8_t interrupt, registers16_t* regs); }
}