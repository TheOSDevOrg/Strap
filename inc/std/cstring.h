#pragma once


#include <stdint.h>
#include <stddef.h>
#include <core/memory.h>
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
static inline int strlen(const char* str)
{
	int len = 0;
	while (str[len] != '\0') len++;
	return len;
}
static inline char* strrev(char* str)
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
static inline void numstr(uint32_t num, int base, char out[])
{
    int sz;
	if (num == 0) 
	{
		out[0] = '0';
		out[1] = '\0';
		return;
	}
	char* str = out;
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
static inline bool strcmp(const char *str1, const char *str2)
{
	int i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i]) return str1[i] - str2[i];
		i++;
	}
	return str1[i] - str2[i];
}
static inline bool strcmpis(const char *str1, const char *str2, int ind1, int ind2, size_t sz)
{
	while (str1[ind1] && str2[ind2])
	{
		if (str1[ind1] != str2[ind2]) return str1[ind1] - str2[ind2];
		ind1++, ind2++;
	}
	return 0;
}