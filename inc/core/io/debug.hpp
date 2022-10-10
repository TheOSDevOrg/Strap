#pragma once

#include <stdint.h>
#include <std/decl.h>

#ifdef __cplusplus
namespace std
{
    namespace dbgio
    {
        enum class dbgtype
        {
            NONE,
            ERROR,
            WARNING,
            INFO,
            DEBUG
        };

        extern dbgtype debug_lvl;
        extern bool serial;
        extern bool tty;

        bool print_debug(const char* fmt);
        bool print_error(const char* fmt);
        bool fatal_error(const char* fmt);
        bool print_info(const char* fmt);
        bool print_warn(const char* fmt);
    };
}
#endif

__cdecl bool dprint_debug(const char* fmt);
__cdecl bool dprint_error(const char* fmt);
__cdecl bool dprint_info(const char* fmt);
__cdecl bool dprint_warn(const char* fmt);
__cdecl bool fatal(const char* fmt);