#pragma once

#include <std/decl.h>
#include <stddef.h>
#include <stdint.h>
#include <std/cstring.h>

__cdecl inline uint32_t hash_str(const void *dat)
{
    const char *str = (const char *)dat;

    uint32_t s = 0;
    int i = 0;
    int sz = strlen(str);
    while (*str && i++ < sz) s += ((sz*(*(str++)^97)*(i^89)))^83;
    return s;
}
__cdecl inline uint32_t hash_str_s(const void *dat, size_t sz)
{
    const char *str = (const char *)dat;

    uint32_t s = 0;
    int i = 0;
    while (*str && i++ < sz) s += ((sz*(*(str++)^97)*(i^89)))^83;
    return s;
}
__cdecl inline uint32_t hash_str_ind(const void *dat, int i)
{
    const char *str = (const char *)dat;

    uint32_t s = 0;
    int sz = strlen(str);
    while (*str && i++ < sz) s += ((sz*(*(str++)^97)*(i^89)))^83;
    return s;
}
__cdecl inline uint32_t hash_str_inds(const void *dat, int i, size_t sz)
{
    const char *str = (const char *)dat;

    uint32_t s = 0;
    while (*str && i++ < sz) s += ((sz*(*(str++)^97)*(i^89)))^83;
    return s;
}