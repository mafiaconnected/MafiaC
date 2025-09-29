#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>

#ifndef LOWORD
#define LOWORD(l)   ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)   ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define BYTEn(x, n) (*((BYTE*)&(x) + n))
#define WORDn(x, n) (*((WORD*)&(x) + n))
#endif

#if defined _WIN32
#include <windows.h>
#endif

#ifndef NULL
#define NULL 0
#endif

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;