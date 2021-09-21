#pragma once

#include <Windows.h>
#include <malloc.h> 

#ifndef WIN32 
#include <sys/mman.h> 
#ifndef PAGESIZE 
#define PAGESIZE 4096 
#endif 
#endif 


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
