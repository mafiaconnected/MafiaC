#pragma once
#include <cstdio>
#include "common.h"
#include <LS3D.h>

extern __declspec(dllexport) S_matrix __stdcall RotateXMatrix(float);
extern __declspec(dllexport) S_matrix __stdcall RotateYMatrix(float);
extern __declspec(dllexport) S_matrix __stdcall RotateZMatrix(float);

extern __declspec(dllexport) bool __stdcall LineLineIntersect(S_vector const&, S_vector const&, S_vector const&, S_vector const&, float&, float&);

extern "C" {
extern __declspec(dllexport) double RandFloat();

extern __declspec(dllexport) int GetEngineVersion();

extern __declspec(dllexport) const char* I3DGetResultMessage(LS3D_RESULT res);

extern __declspec(dllexport) bool TEV(LPCSTR caption, int version);

extern __declspec(dllexport) void dbgPrintf1(const char* fmt, ...);
extern __declspec(dllexport) void dbgPrintf(const char* fmt, ...);

static void debugPrintf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char buf[512];
	vsprintf(buf, fmt, args);
	va_end(args);

	char buf2[512];
	sprintf(buf2, "%s\n", buf);
	OutputDebugStringA(buf2);
	dbgPrintf(buf);
}
}