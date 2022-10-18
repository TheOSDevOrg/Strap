#include <stdint.h>
#include <std/stdio.h>
#include <std/ubsan.h>
#include <std/decl.h>
#include <std/array.h>
#include <std/ptr.h>
#include <core/kernel.hpp>

void *__dso_handle;
static int _ubsan_cx = 0, _ubsan_cy = 0;
static char _ubsan_buff[512];

__cdecl int atexit(void *func)
{
    return 0;
}

__cdecl void __cxa_finalize(void *f)
{

}

__cdecl void __cxa_pure_virtual(void)
{

}

__cdecl
{
    #define is_aligned(value, alignment) !(value & (alignment - 1))

    void __ubsan_output(const char* str)
    {
        for (size_t i = 0; i < strlen(str); i++)
        {
            if (str[i] == '\n') { _ubsan_cx = 0; _ubsan_cy++; }
            else
            {
                ((uint8_t*)0xB8000 + ((_ubsan_cy * 80 + _ubsan_cx) * 2))[0] = str[i];
                ((uint8_t*)0xB8000 + ((_ubsan_cy * 80 + _ubsan_cx) * 2))[1] = 0xF0;
                _ubsan_cx++;
                if (_ubsan_cx >= 80) { _ubsan_cx = 0; _ubsan_cy++; }
            }
        }
    }

    void __ubsan_handle_out_of_bounds(ubsan_outofbounds_t* info, uintptr_t pointer)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Out Of Bounds Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_type_mismatch(ubsan_typemismatch_t* info, uintptr_t pointer)
    {
        if (pointer == 0)
        {
            memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
            sprintf(_ubsan_buff, "[ERROR] Type Mismatch Exception - null pointer access\n");
            __ubsan_output(_ubsan_buff);
        }
        else if (info->alignment != 0 && is_aligned(pointer, info->alignment))
        {
            memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
            sprintf(_ubsan_buff, "[ERROR] Type Mismatch Exception - unaligned memory access\n");
            __ubsan_output(_ubsan_buff);
        }
        else { return; }

        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_add_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Add Overflow Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_sub_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Sub Overflow Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_mul_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Mul Overflow Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_negate_overflow(ubsan_overflow_t* info, uint32_t old_val)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Negate Overflow Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_divrem_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Divrem Overflow Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_missing_return(ubsan_unreachabledata_t* info)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Missing Return Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_load_invalid_value(ubsan_invalidvalue_t* info, uint32_t val)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Load Invalid Value Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_shift_out_of_bounds(ubsan_shift_outofbounds_t* info, uint32_t lhs, uint32_t rhs)
    {
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Shift Out Of Bounds Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }

    void __ubsan_handle_vla_bound_not_positive(ubsan_vla_bound_data_t* info, uint32_t bound)
    {
       memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "[ERROR] Bound Not Positive Exception\n");
        __ubsan_output(_ubsan_buff);
        memset(_ubsan_buff, 0, sizeof(_ubsan_buff));
        sprintf(_ubsan_buff, "LINE:%d COL:%d FILE:'%s'", info->location.line, info->location.column, info->location.file);
        __ubsan_output(_ubsan_buff);
        asm volatile("cli; hlt");
    }
}