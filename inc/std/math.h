#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int abs(int i) { return i < 0 ? -i : i; }

static inline int clamp(int i, int min, int max)
{
    if (i < min) { i = min; }
    if (i > max) { i = max; }
    return i;
}

static inline double ceil(double x)
{
    int inum = (int)x;
    if (x == (float)inum) { return inum; }
    return inum + 1;
}

static inline double floor(double x)
{
    if (x >= INT64_MAX || x <= INT64_MIN || x != x) { return x; }
    uint64_t n = (uint64_t)x;
    double d = (double)n;
    if (d == x || x >= 0) { return d; } else { return d - 1; }
    return 0;
}

static inline double pow(double x, int y)
{
    double temp;
    if (y == 0)
    return 1;
    temp = pow (x, y / 2);
    if ((y % 2) == 0) { return temp * temp; } 
    else 
    {
        if (y > 0) { return x * temp * temp; }
        else { return (temp * temp) / x; }
    }
}

static inline double trunc(double x) { return x < 0 ? -floor(-x) : floor(x); }

static inline double fmod(double x, double y) { return x - trunc(x / y) * y; }

#ifdef __cplusplus
}
#endif
