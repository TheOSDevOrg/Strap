#pragma once
#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
#include <std/math.h>
#include <std/ptr.h>

static inline int numlen(uint32_t num)
{
	int len = 0;
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return len;
}

static inline int strlen(const char * str)
{
	int len = 0;
	while (str[len] != '\0') len++;
	return len;
}

static inline char* strrev(char * str)
{
	int i = 0;
	int j = strlen(str) - 1;
	while (i < j)
	{
		char c = str[i];
		str[i] = str[j];
		str[j] = c;
		i++;
		j--;
	}
	return str;
}

static inline char* strnrev(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
	return str;
}

static inline char* numstr(uint32_t num, int base, char out[])
{
    int sz;
	if (num == 0) 
	{
		out[0] = '0';
		out[1] = '\0';
		return out;
	}
	char * str = out;
	int i = 0;
	while (num > 0)
	{
		int r = num % base;
		if (r >= 10)
		{
			str[i] = r - 10 + 'a';
		}
		else
		{
			str[i] = r + '0';
		}
		num = num / base;
		i++;
	}
	str[i] = '\0';
	strrev(str);
	return out;
}

static inline int _ftoa_conv(int x, char str[], int d)
{
    int i = 0;
    while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }

    while (i < d) { str[i++] = '0'; }
  
    strnrev(str, i);
    str[i] = '\0';
    return i;
}

static inline char* ftoa(float num, char* str, int dplace)
{
    int ipart = (int)num;
    float fpart = num - (float)ipart;
    int i = _ftoa_conv(ipart, str, 0);
  
    if (dplace != 0) 
    {
        str[i] = '.';
        fpart = fpart * pow(10, dplace);
        _ftoa_conv((int)fpart, str + i + 1, dplace);
    }
    return str;
}

static inline char* hexstr(uint32_t num, char *str, bool prefix, uint8_t sz)
{
	static const char hexvals[] = "0123456789ABCDEF";
    char* orig = str;
    orig[0] = 0;
    if (prefix) { str[0] = '0'; str[1] = 'x'; str += 2; }
    if (sz == 1)
    {
        *str = hexvals[(uint8_t)((num & 0xF0) >> 4)]; str++;
        *str = hexvals[(uint8_t)((num & 0x0F))];
    }
    else if (sz == 2)
    {
        *str = hexvals[(uint8_t)((num & 0xF000) >> 12)]; str++;
        *str = hexvals[(uint8_t)((num & 0x0F00) >> 8)];  str++;
        *str = hexvals[(uint8_t)((num & 0x00F0) >> 4)];  str++;
        *str = hexvals[(uint8_t)((num & 0x000F))]; 
    }
    else if (sz == 4)
    {
        *str = hexvals[(uint8_t)((num & 0xF0000000) >> 28)]; str++;
        *str = hexvals[(uint8_t)((num & 0x0F000000) >> 24)]; str++;
        *str = hexvals[(uint8_t)((num & 0x00F00000) >> 20)]; str++;
        *str = hexvals[(uint8_t)((num & 0x000F0000) >> 16)]; str++;
        *str = hexvals[(uint8_t)((num & 0x0000F000) >> 12)]; str++;
        *str = hexvals[(uint8_t)((num & 0x00000F00) >> 8)];  str++;
        *str = hexvals[(uint8_t)((num & 0x000000F0) >> 4)];  str++;
        *str = hexvals[(uint8_t)((num & 0x0000000F))];
    }
    return orig;
}


static inline int strnum(char str[], int base)
{
	int ret = 0;
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			ret = ret * base + str[i] - '0';
		}
		else if (str[i] >= 'a' && str[i] <= 'z')
		{
			ret = ret * base + str[i] - 'a' + 10;
		}
		else if (str[i] >= 'A' && str[i] <= 'Z')
		{
			ret = ret * base + str[i] - 'A' + 10;
		}
		i++;
	}
	return ret;
}

static inline int strcmp(const char *str1, const char *str2)
{
	int i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i]) return str1[i] - str2[i];
		i++;
	}
	return str1[i] - str2[i];
}

static inline int strcmpis(const char *str1, const char *str2, int ind1, int ind2, size_t sz)
{
	while (str1[ind1] && str2[ind2])
	{
		if (str1[ind1] != str2[ind2]) return str1[ind1] - str2[ind2];
		ind1++, ind2++;
	}
	return 0;
}

static inline int strncmp(const char *str1, const char *str2, size_t n)
{
	return 0;
}

static inline char* strcat(char *dest, const char *src)
{
	if (dest == NULL) { return NULL; }
    if (src == NULL)  { return dest; }
    size_t src_len = strlen(src);
    size_t dest_len = strlen(dest);
    for (size_t i = 0; i < src_len; i++) { dest[dest_len + i] = src[i]; }
    dest[strlen(dest)] = 0;
    return dest;
}

static inline char* strncat(char *dest, const char *src, size_t n)
{
	 if (dest == NULL) { return NULL; }
    if (src == NULL)  { return dest; }
    size_t dest_len = strlen(dest);
    for (size_t i = 0; i < n; i++) { dest[dest_len + i] = src[i]; }
    dest[strlen(dest)] = 0;
    return dest;
}

static inline char* strcatc(char* str, char c)
{
    if (str == NULL) { return NULL; }
    size_t len = strlen(str);
    str[len] = (char)c;
    str[len + 1] = 0;
    return str;
}

static inline char* strchr(const char *str, char c)
{
	do { if (*str == c) { return (char*)str; } } while (*str++);
    return NULL;
}

static inline char* strcpy(char *dest, const char *src)
{
	if (dest == NULL) { return NULL; }
    if (src == NULL) { return NULL; }

    int i = 0;
    while (src[i] != 0) { dest[i] = src[i]; i++; }
    return dest;
}

static inline char* strncpy(char *dest, const char *src, size_t n)
{
	for (uint32_t i = 0; i < n; i++) 
    { 
        if (src[i] == 0) { break; }
        dest[i] = src[i]; 
    }
    return dest;
}

