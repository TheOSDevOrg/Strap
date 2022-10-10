#pragma once
#define __cdecl extern "C"
#define section__(x) __attribute__((section(#x)))
#define packed__ __attribute__((packed))