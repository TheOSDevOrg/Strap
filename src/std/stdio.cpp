#include <std/stdio.h>
#include <core/kernel.hpp>

#ifdef __cplusplus
extern "C" {
#endif

static char _fmt_buff[8192];

int sprintf(char* str, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int v = vsprintf(str, fmt, args);
    va_end(args);
    return v;
}

int vsprintf(char* str, const char* fmt, va_list args)
{ 
    if (str == NULL) { return 1; }
    if (strlen(fmt) == 0) { return 1; }

    while (*fmt != 0)
    {
        if (*fmt == '%')
        {
            fmt++;
            if (*fmt == '%') { strcatc(str, '%'); fmt++; }
            else if (*fmt == 'c') { strcatc(str, va_arg(args, int)); }
            else if (*fmt == 'd')
            {
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                int num = va_arg(args, int);
                if (num < 0) { strcatc(str, '-'); }
                strcat(str, numstr(num, 10, _fmt_buff));
            }
            else if (*fmt == 'u')
            {
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                strcat(str, numstr(va_arg(args, int), 10, _fmt_buff));
            }
            else if (*fmt == 'x')
            {
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                strcat(str, numstr(va_arg(args, int), 16, _fmt_buff));
            }
            else if (*fmt == '2')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { strcatc(str, *fmt); fmt++; continue; }
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                strcat(str, hexstr(va_arg(args, uint32_t), _fmt_buff, false, 1));
            }
            else if (*fmt == '4')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { strcatc(str, *fmt); fmt++; continue; }
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                strcat(str, hexstr(va_arg(args, uint32_t), _fmt_buff, false, 2));
            }
            else if (*fmt == '8')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { strcatc(str, *fmt); fmt++; continue; }
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                strcat(str, hexstr(va_arg(args, uint32_t), _fmt_buff, false, 4));
            }
            else if (*fmt == 'f')
            {
                fmt++;
                memset(_fmt_buff, 0, sizeof(_fmt_buff));
                strcat(str, ftoa(va_arg(args, float), _fmt_buff, 3));
            }
            else if (*fmt == 's') { strcat(str, va_arg(args, char*)); }
            else { strcatc(str, *fmt); }
        }
        else { strcatc(str, *fmt); }
        fmt++;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif