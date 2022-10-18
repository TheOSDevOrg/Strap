#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char* str, const char* fmt, ...);
int vsprintf(char* str, const char* fmt, va_list args);

#ifdef __cplusplus
}
#endif