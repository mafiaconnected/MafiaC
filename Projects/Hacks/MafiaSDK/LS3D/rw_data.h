#pragma once

#include <Windows.h>
#include <cstdint>

class C_rw_data {
public:
	virtual ~C_rw_data() {};
	virtual int32_t __thiscall AddRef() = 0;
	virtual int32_t __thiscall Release() = 0;
	virtual bool __thiscall UnlockPack(uint32_t uKey1, uint32_t uKey2) = 0;
};

#define RW_DATA_API __declspec(dllexport)

extern "C" {
	RW_DATA_API const char* __stdcall dtaBin2Text(int iFileHandle, uint32_t uUnk);
	RW_DATA_API bool __stdcall dtaClose(int iFileHandle);
	RW_DATA_API C_rw_data* __stdcall dtaCreate(const char* pSzDtaFileName);
	RW_DATA_API bool __stdcall dtaDelete(const char* pSzDtaFileName);
	RW_DATA_API bool __stdcall dtaDumpMemoryLeaks(bool bDumpLeaks);
	RW_DATA_API bool __stdcall dtaGetTime(int iFileHandle, _FILETIME* pFileTime, uint32_t _unk3);
	RW_DATA_API int __stdcall dtaOpen(const char* pSzFileName, uint32_t uFlags);
	RW_DATA_API int __stdcall dtaOpenWrite(const char* pSzFileName, uint32_t uFlags);
	RW_DATA_API int __stdcall dtaRead(int iFileHandle, void* pBufferOut, int32_t uBufferSize);
	RW_DATA_API int __stdcall dtaSeek(int iFileHandle, int32_t uOffset, int32_t uOrigin);
	RW_DATA_API void __stdcall dtaSetDtaFirstForce();
	RW_DATA_API int __stdcall dtaWrite(int iFileHandle, void* pBufferIn, int32_t uBufferSize);
}

static int dtaGetSize(int iFileHandle) {
	int pos = dtaSeek(iFileHandle, 0, SEEK_END);

	dtaSeek(iFileHandle, 0, SEEK_SET);

	return pos;
};