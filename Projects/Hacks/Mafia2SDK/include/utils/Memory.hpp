#pragma once

#include <Windows.h>
#include <malloc.h>

#ifndef WIN32
#include <sys/mman.h>
#ifndef PAGESIZE
#define PAGESIZE 4096
#endif
#endif

namespace Mem
{
    #ifndef M2_Address
    #define M2_Address unsigned long
    #define M2_Pointer unsigned int
    #define Byte unsigned char
    #endif

    enum X86Instructions : unsigned char
    {
        NOP = 0x90,
        RETN = 0xC3,
        CALL = 0xE8,
        JMP = 0xE9
    };

    enum CallingConventions : unsigned char
    {
        call_cdecl,
        call_std,
        call_this
    };

    // Helpers
    //inline M2_Address GetModuleAddress(const char *szModule) { return reinterpret_cast<uint32_t>(GetModuleHandle(szModule)); }

    template <typename T> T Read(M2_Address addr) { return *reinterpret_cast<T*>(addr); }

    // Scoped protect
    class ScopedProtect
    {
    public:
        ScopedProtect(M2_Address addr, size_t size) :m_addr(addr), m_size(size)
        {
            VirtualProtect(reinterpret_cast<void *>(m_addr), m_size, PAGE_EXECUTE_READWRITE, &m_protect);
        }

        ~ScopedProtect(void)
        {
            VirtualProtect(reinterpret_cast<void *>(m_addr), m_size, m_protect, nullptr);
        }
    private:
        unsigned long m_protect;
        M2_Address m_addr;
        size_t m_size;
    };

    template<typename T>
    inline void Write(M2_Address addr, T value)
    {
        ScopedProtect protect(addr, sizeof(T));
        *(T*)(addr) = value;
    }

    template<typename T>
    inline void WriteFast(M2_Address addr, T value)
    {
        *(T*)(addr) = value;
    }

    // Function invoker
    template<CallingConventions CConv, typename Ret, typename ...Args>
    Ret InvokeFunction(M2_Address address, Args ...args)
    {
        if (CConv == call_cdecl)
            return (reinterpret_cast<Ret(__cdecl *)(Args...)>(address))(args...);
        else if (CConv == call_std)
            return (reinterpret_cast<Ret(__stdcall *)(Args...)>(address))(args...);
        else
            return (reinterpret_cast<Ret(__thiscall *)(Args...)>(address))(args...);
    }

    namespace Hooks
    {
        extern  BYTE                * InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize = 5);
        extern  void                UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize = 5);

        extern  void                * InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress);
        extern  void                * InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress);
        extern  bool                UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress);

        extern  void                * InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize = 5);
        extern  void                * InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize = 5);

        extern  M2_Address             InstallNotDumbJMP(M2_Address target_addr, M2_Address hookfnc_addr, size_t len = 5);
    };

    namespace Utilites
    {
        extern  void                InstallNopPatch(uint32_t dwAddress, int iSize = 1);
        extern  void                PatchAddress(uint32_t dwAddress, BYTE * bPatch, size_t iSize);
        extern  void                PatchAddress(uint32_t dwAddress, uint32_t dwPatch);

        extern  bool                bDataCompare(const unsigned char * pData, const unsigned char * bMask, const char * szMask);
        extern  unsigned long       FindPattern2(uint32_t dwAddress, uint32_t dwLen, unsigned char * bMask, char * szMask);
        extern  unsigned long       FindPattern(unsigned char * bMask, char * szMask);

        extern  void                DumpVFTable(uint32_t dwAddress, int iFunctionCount);
    };

    extern void Initialize(void);
}

// ---------------------------------------------------------------------------------------------
// MemoryPatcher: ported identically from MafiaSDK/Utils/MemoryPatcher.h so both SDKs patch
// memory the same way. All methods are defined inline in the class body (same as MafiaSDK's
// copy), so this needs no MAFIA2_SDK_IMPLEMENTATION translation unit and no separate .cpp.
// ---------------------------------------------------------------------------------------------

#define X86_NOP 0x90
#define X86_RETN 0xC3
#define X86_CALL 0xE8
#define X86_JMP 0xE9

struct ProtectionInfo
{
	DWORD dwAddress;
	DWORD dwOldProtection;
	int   iSize;
};

class MemoryPatcher
{
public:
	static ProtectionInfo Unprotect(DWORD dwAddress, int iSize)
	{
		ProtectionInfo protectionInfo;
		protectionInfo.dwAddress = dwAddress;
		protectionInfo.iSize = iSize;
#ifdef WIN32
		VirtualProtect((void *)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &protectionInfo.dwOldProtection);
#else
		mprotect((void *)((dwAddress / PAGESIZE) * PAGESIZE), PAGESIZE, (PROT_EXEC | PROT_READ | PROT_WRITE));
#endif
		return protectionInfo;
	}

	static void InstallCallHook(DWORD address, DWORD function)
	{
		DWORD lpflOldProtect;
		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		*(BYTE*)(address) = 0xE8;
		*(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
		VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
	}

	static void Reprotect(ProtectionInfo protectionInfo)
	{
#ifdef WIN32
		DWORD dwProtection;
		VirtualProtect((void *)protectionInfo.dwAddress, protectionInfo.iSize, protectionInfo.dwOldProtection, &dwProtection);
#else
		//get old protection
#endif
	}

	static void InstallNopPatch(DWORD dwAddress, int iSize)
	{
		DWORD dwAddr = dwAddress;
		ProtectionInfo protectionInfo = Unprotect(dwAddr, iSize);
		memset((void *)dwAddr, X86_NOP, iSize);
		Reprotect(protectionInfo);
	}

	static void * InstallDetourPatchInternal(DWORD dwAddress, DWORD dwDetourAddress, BYTE byteType, int iSize)
	{
		BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);
		Unprotect((DWORD)pbyteTrampoline, (iSize + 5));
		ProtectionInfo protectionInfo = Unprotect(dwAddress, (iSize + 5));
		memcpy(pbyteTrampoline, (void *)dwAddress, iSize);
		DWORD dwTrampoline = (DWORD)(pbyteTrampoline + iSize);
		*(BYTE *)dwTrampoline = byteType;
		*(DWORD *)(dwTrampoline + 1) = ((dwAddress + iSize) - dwTrampoline - 5);
		*(BYTE *)dwAddress = byteType;
		*(DWORD *)(dwAddress + 1) = (dwDetourAddress - dwAddress - 5);
		Reprotect(protectionInfo);
		return pbyteTrampoline;
	}

	static void UninstallDetourPatchInternal(DWORD dwAddress, void * pTrampoline, int iSize)
	{
		ProtectionInfo protectionInfo = Unprotect(dwAddress, iSize);
		memcpy((void *)dwAddress, pTrampoline, iSize);
		Reprotect(protectionInfo);
		free(pTrampoline);
	}

	static void InstallPatchType(DWORD dwAddress, DWORD dwTypeAddress, BYTE byteType, int iSize)
	{
		switch (byteType)
		{
		case X86_JMP:
		{
			InstallDetourPatchInternal(dwAddress, dwTypeAddress, X86_JMP, iSize);
		}
		case X86_CALL:
		{
			InstallDetourPatchInternal(dwAddress, dwTypeAddress, X86_CALL, iSize);
		}
		}
	}

	static void InstallRetnPatch(DWORD dwAddress)
	{
		DWORD dwAddr = dwAddress;
		ProtectionInfo protectionInfo = Unprotect(dwAddr, 1);
		*(BYTE *)dwAddr = X86_RETN;
		Reprotect(protectionInfo);
	}

	static void InstallStringPatch(DWORD dwAddress, char * szString, int iSize)
	{
		DWORD dwAddr = dwAddress;
		ProtectionInfo protectionInfo = Unprotect(dwAddr, iSize);
		memcpy((void *)dwAddr, szString, iSize);
		Reprotect(protectionInfo);
	}

	static void InstallMethodPatch(DWORD dwHookAddress, DWORD dwFunctionAddress)
	{
		DWORD dwHookAddr = dwHookAddress;
		ProtectionInfo protectionInfo = Unprotect(dwHookAddr, 4);
		*(DWORD *)dwHookAddr = (DWORD)dwFunctionAddress;
		Reprotect(protectionInfo);
	}

	static DWORD GetFunctionAddress(LPCWSTR szLibrary, char * szFunction)
	{
		return (DWORD)GetProcAddress(LoadLibraryW(szLibrary), szFunction);
	}

	static DWORD GetFunctionAddress(LPCWSTR szLibrary, unsigned int uOrdinal)
	{
		return GetFunctionAddress(szLibrary, (char *)MAKELONG(uOrdinal, 0));
	}

	static void * InstallDetourPatch(LPCWSTR szLibrary, char * szFunction, DWORD dwFunctionAddress)
	{
		return InstallDetourPatchInternal(GetFunctionAddress(szLibrary, szFunction), dwFunctionAddress, X86_JMP, 5);
	}

	static void * InstallDetourPatch(LPCWSTR szLibrary, unsigned int uOrdinal, DWORD dwFunctionAddress)
	{
		return InstallDetourPatchInternal(GetFunctionAddress(szLibrary, uOrdinal), dwFunctionAddress, X86_JMP, 5);
	}

	static void UninstallDetourPatch(void * pTrampoline, DWORD dwFunctionAddress)
	{
		ProtectionInfo protectionInfo = Unprotect(dwFunctionAddress, 5);
		memcpy((void *)dwFunctionAddress, pTrampoline, 5);
		Reprotect(protectionInfo);
		free(pTrampoline);
	}

	static BYTE InstallDetourPatchWithData(LPCWSTR szLibrary, unsigned int uOrdinal, DWORD dwFunctionAddress)
	{
		DWORD dwAddress = GetFunctionAddress(szLibrary, uOrdinal);
		DWORD dwDetourAddress = dwFunctionAddress;
		BYTE byteType = X86_JMP;
		int iSize = 5;
		BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);
		Unprotect((DWORD)pbyteTrampoline, (iSize + 5));
		ProtectionInfo protectionInfo = Unprotect(dwAddress, (iSize + 5));
		memcpy(pbyteTrampoline, (void *)dwAddress, iSize);
		DWORD dwTrampoline = (DWORD)(pbyteTrampoline + iSize);
		*(BYTE *)dwTrampoline = byteType;
		*(DWORD *)(dwTrampoline + 1) = ((dwAddress + iSize) - dwTrampoline - 5);
		*(BYTE *)dwAddress = byteType;
		*(DWORD *)(dwAddress + 1) = (dwDetourAddress - dwAddress - 5);
		Reprotect(protectionInfo);
		return (pbyteTrampoline != NULL);
	}

	static void InstallPushPatch(DWORD dwAddress, DWORD dwFunc)
	{
		ProtectionInfo protectionInfo = Unprotect(dwAddress, 5);
		*(BYTE*)(dwAddress) = 0x68;
		*(DWORD*)(dwAddress + 1) = dwFunc;
		Reprotect(protectionInfo);
	}

	static void PatchAddress(DWORD dwAddress, BYTE *bPatch, size_t iSize)
	{
		DWORD d, ds;

		VirtualProtect((void*)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &d);
		memcpy((void*)dwAddress, bPatch, iSize);
		VirtualProtect((void*)dwAddress, iSize, d, &ds);
	}

	static void PatchAddress(DWORD dwAddress, DWORD dwPatch)
	{
		DWORD d, ds;

		VirtualProtect((void*)dwAddress, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &d);
		*(DWORD *)(dwAddress) = dwPatch;
		VirtualProtect((void*)dwAddress, sizeof(DWORD), d, &ds);
	}

	static void InstallJmpHook(DWORD address, DWORD function)
	{
		DWORD lpflOldProtect;
		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		*(BYTE*)(address) = 0xE9;
		*(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
		VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
	}
};

// ---------------------------------------------------------------------------------------------
// Implementations of the Mem::Hooks / Mem::Utilites / Mem::Initialize API declared above.
// These have external linkage (unlike MemoryPatcher's in-class methods), so exactly one
// translation unit must `#define MAFIA2_SDK_IMPLEMENTATION` before including m2sdk.h - see
// Main.cpp. They all delegate to MemoryPatcher above, so patching stays identical to MafiaSDK.
// ---------------------------------------------------------------------------------------------

#ifdef MAFIA2_SDK_IMPLEMENTATION

BYTE * Mem::Hooks::InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize)
{
	return (BYTE *)MemoryPatcher::InstallDetourPatchInternal(dwAddress, dwDetourAddress, byteType, iSize);
}

void Mem::Hooks::UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize)
{
	MemoryPatcher::UninstallDetourPatchInternal(dwAddress, *pTrampoline, iSize);
	*pTrampoline = nullptr;
}

void * Mem::Hooks::InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress)
{
	int iWideLen = MultiByteToWideChar(CP_ACP, 0, szLibrary, -1, nullptr, 0);
	WCHAR szWideLibrary[MAX_PATH];
	if (iWideLen <= 0 || iWideLen > MAX_PATH)
		return nullptr;
	MultiByteToWideChar(CP_ACP, 0, szLibrary, -1, szWideLibrary, iWideLen);

	return MemoryPatcher::InstallDetourPatch(szWideLibrary, szFunction, dwFunctionAddress);
}

void * Mem::Hooks::InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress)
{
	return MemoryPatcher::InstallDetourPatchInternal(dwAddress, dwFunctionAddress, X86_JMP, 5);
}

bool Mem::Hooks::UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress)
{
	MemoryPatcher::UninstallDetourPatch(pTrampoline, dwFunctionAddress);
	return true;
}

void * Mem::Hooks::InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize)
{
	return MemoryPatcher::InstallDetourPatchInternal(dwAddress, dwCallAddress, X86_CALL, iSize);
}

void * Mem::Hooks::InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize)
{
	return MemoryPatcher::InstallDetourPatchInternal(dwAddress, dwJmpAddress, X86_JMP, iSize);
}

M2_Address Mem::Hooks::InstallNotDumbJMP(M2_Address target_addr, M2_Address hookfnc_addr, size_t len)
{
	return (M2_Address)MemoryPatcher::InstallDetourPatchInternal(target_addr, hookfnc_addr, X86_JMP, (int)len);
}

void Mem::Utilites::InstallNopPatch(uint32_t dwAddress, int iSize)
{
	MemoryPatcher::InstallNopPatch(dwAddress, iSize);
}

void Mem::Utilites::PatchAddress(uint32_t dwAddress, BYTE *bPatch, size_t iSize)
{
	MemoryPatcher::PatchAddress(dwAddress, bPatch, iSize);
}

void Mem::Utilites::PatchAddress(uint32_t dwAddress, uint32_t dwPatch)
{
	MemoryPatcher::PatchAddress(dwAddress, dwPatch);
}

bool Mem::Utilites::bDataCompare(const unsigned char * pData, const unsigned char * bMask, const char * szMask)
{
    for (; *szMask; ++szMask, ++pData, ++bMask)
    {
        if (*szMask == 'x' && *pData != *bMask)
        {
            return false;
        }
    }

    return (*szMask) == 0;
}

unsigned long Mem::Utilites::FindPattern2(uint32_t dwAddress, uint32_t dwLen, unsigned char * bMask, char * szMask)
{
    for (unsigned long i = 0; i < dwLen; i++)
    {
        if (bDataCompare((unsigned char *)(dwAddress + i), bMask, szMask))
        {
            return (unsigned long)(dwAddress + i);
        }
    }

    return 0;
}

unsigned long Mem::Utilites::FindPattern(unsigned char * bMask, char * szMask)
{
    return FindPattern2((uint32_t)GetModuleHandle(NULL), 0xFFFFFFFF, bMask, szMask);
}

void Mem::Utilites::DumpVFTable(uint32_t dwAddress, int iFunctionCount)
{
    /*CLogFile::Printf("Dumping Virtual Function Table at 0x%p...", dwAddress);
    for (int i = 0; i < iFunctionCount; i++)
    {
        CLogFile::Printf("VFTable Offset: %d, Function: 0x%p (At M2_Address: 0x%p)", (i * 4), *(Puint32_t)(dwAddress + (i * 4)), (dwAddress + (i * 4)));
    }*/
}

void Mem::Initialize(void)
{
    // Prepare headers
    PBYTE pbImageBase = (PBYTE)GetModuleHandle(NULL);
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pbImageBase;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)(pbImageBase + pDosHeader->e_lfanew);
    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHeaders);

    // Loop thought all sections
    for (int iSection = 0; iSection < pNtHeaders->FileHeader.NumberOfSections; iSection++, pSection++)
    {
        char * szSectionName = (char*)pSection->Name;
        if (!strcmp(szSectionName, ".text") || !strcmp(szSectionName, ".rdata") || !strcmp(szSectionName, ".textnc"))
        {
            // Unprotect segment
            DWORD dwOld = 0;//Temp variable
            VirtualProtect((void *)(pbImageBase + pSection->VirtualAddress), ((pSection->Misc.VirtualSize + 4095)&~4095), PAGE_EXECUTE_READWRITE, &dwOld);
        }
    }
}

ProtectionInfo Unprotect(DWORD dwAddress, int iSize)
{
	return MemoryPatcher::Unprotect(dwAddress, iSize);
}

void InstallCallHook(DWORD address, DWORD function)
{
	MemoryPatcher::InstallCallHook(address, function);
}

void Reprotect(ProtectionInfo protectionInfo)
{
	MemoryPatcher::Reprotect(protectionInfo);
}

DWORD GetFunctionAddress(LPCWSTR szLibrary, char * szFunction)
{
	return MemoryPatcher::GetFunctionAddress(szLibrary, szFunction);
}

DWORD GetFunctionAddress(LPCWSTR szLibrary, unsigned int uOrdinal)
{
	return MemoryPatcher::GetFunctionAddress(szLibrary, uOrdinal);
}

#endif // MAFIA2_SDK_IMPLEMENTATION

extern ProtectionInfo Unprotect(DWORD dwAddress, int iSize);

extern void InstallCallHook(DWORD address, DWORD function);

extern void Reprotect(ProtectionInfo protectionInfo);

extern DWORD GetFunctionAddress(LPCWSTR szLibrary, char * szFunction);

extern DWORD GetFunctionAddress(LPCWSTR szLibrary, unsigned int uOrdinal);
