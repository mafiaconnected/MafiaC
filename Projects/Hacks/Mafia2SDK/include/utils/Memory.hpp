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

    template<typename T>
    inline void Write(M2_Address addr, T value)
    {
        Protect protect(addr, sizeof(T));
        *(T*)(addr) = value;
    }

    template<typename T>
    inline void WriteFast(M2_Address addr, T value)
    {
        *(T*)(addr) = value;
    }

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

    class Hooks
    {
    public:
        static  BYTE                * InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize = 5);
        static  void                UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize = 5);

        static  void                * InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress);
        static  void                * InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress);
        static  bool                UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress);

        static  void                * InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize = 5);
        static  void                * InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize = 5);

        static  M2_Address             InstallNotDumbJMP(M2_Address target_addr, M2_Address hookfnc_addr, size_t len = 5);

    };

    class Utilites
    {
    public:
        static  void                InstallNopPatch(uint32_t dwAddress, int iSize = 1);
        static  void                PatchAddress(uint32_t dwAddress, BYTE * bPatch, size_t iSize);
        static  void                PatchAddress(uint32_t dwAddress, uint32_t dwPatch);

        static  bool                bDataCompare(const unsigned char * pData, const unsigned char * bMask, const char * szMask);
        static  unsigned long       FindPattern2(uint32_t dwAddress, uint32_t dwLen, unsigned char * bMask, char * szMask);
        static  unsigned long       FindPattern(unsigned char * bMask, char * szMask);


        static  void                DumpVFTable(uint32_t dwAddress, int iFunctionCount);
    };

    void Initialize(void);
}

struct ProtectionInfo
{
	DWORD dwAddress;
	DWORD dwOldProtection;
	int   iSize;
};

ProtectionInfo Unprotect(DWORD dwAddress, int iSize)
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

void InstallCallHook(DWORD address, DWORD function)
{
	DWORD lpflOldProtect;
	VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	*(BYTE*)(address) = 0xE8;
	*(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
	VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
}

void Reprotect(ProtectionInfo protectionInfo)
{
#ifdef WIN32
	DWORD dwProtection;
	VirtualProtect((void *)protectionInfo.dwAddress, protectionInfo.iSize, protectionInfo.dwOldProtection, &dwProtection);
#else
	//get old protection
#endif
}

DWORD GetFunctionAddress(LPCWSTR szLibrary, char * szFunction)
{
	return (DWORD)GetProcAddress(LoadLibraryW(szLibrary), szFunction);
}

DWORD GetFunctionAddress(LPCWSTR szLibrary, unsigned int uOrdinal)
{
	return GetFunctionAddress(szLibrary, (char *)MAKELONG(uOrdinal, 0));
}

#ifdef MAFIA2_SDK_IMPLEMENTATION

BYTE * Mem::Hooks::InstallDetourPatchInternal(uint32_t dwAddress, uint32_t dwDetourAddress, BYTE byteType, int iSize)
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

void Mem::Hooks::UninstallDetourPatchInternal(uint32_t dwAddress, void ** pTrampoline, int iSize)
{
	ProtectionInfo protectionInfo = Unprotect(dwAddress, iSize);
	memcpy((void *)dwAddress, pTrampoline, iSize);
	Reprotect(protectionInfo);
	free(pTrampoline);
}

void * Mem::Hooks::InstallDetourPatch(char * szLibrary, char * szFunction, uint32_t dwFunctionAddress)
{
	return InstallDetourPatchInternal(GetFunctionAddress((LPCWSTR)szLibrary, (char *)szFunction), dwFunctionAddress, JMP, 5);
}

void * Mem::Hooks::InstallDetourPatch(uint32_t dwAddress, uint32_t dwFunctionAddress)
{
	return InstallDetourPatchInternal(dwAddress, dwFunctionAddress, JMP, 5);
}

bool Mem::Hooks::UninstallDetourPatch(void * pTrampoline, uint32_t dwFunctionAddress)
{
	ProtectionInfo protectionInfo = Unprotect(dwFunctionAddress, 5);
	memcpy((void *)dwFunctionAddress, pTrampoline, 5);
	Reprotect(protectionInfo);
	free(pTrampoline);
	return true;
    //return DetourRemove((BYTE *)pTrampoline, (BYTE *)dwFunctionAddress);
}

void Mem::Utilites::PatchAddress(uint32_t dwAddress, BYTE *bPatch, size_t iSize)
{
	DWORD d, ds;

	VirtualProtect((void*)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &d);
	memcpy((void*)dwAddress, bPatch, iSize);
	VirtualProtect((void*)dwAddress, iSize, d, &ds);
}

void Mem::Utilites::PatchAddress(uint32_t dwAddress, uint32_t dwPatch)
{
	DWORD d, ds;

	VirtualProtect((void*)dwAddress, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &d);
	*(DWORD *)(dwAddress) = dwPatch;
	VirtualProtect((void*)dwAddress, sizeof(DWORD), d, &ds);
}

void * Mem::Hooks::InstallCallPatch(uint32_t dwAddress, uint32_t dwCallAddress, int iSize)
{
	return InstallDetourPatchInternal(dwAddress, dwCallAddress, CALL, iSize);
}

void * Mem::Hooks::InstallJmpPatch(uint32_t dwAddress, uint32_t dwJmpAddress, int iSize)
{
	return InstallDetourPatchInternal(dwAddress, dwJmpAddress, JMP, iSize);
}

M2_Address Mem::Hooks::InstallNotDumbJMP(M2_Address target_addr, M2_Address hookfnc_addr, size_t len)
{
	return (M2_Address)InstallDetourPatchInternal(target_addr, hookfnc_addr, JMP, len);

    //ScopedProtect(target_addr, len);
    //std::vector<Byte> patch_data(len, 0x90);
    //patch_data[0] = X86Instructions::JMP;
    //*reinterpret_cast<M2_Address *>(patch_data.data() + 1) = hookfnc_addr - (target_addr + 5);
    //std::copy_n(patch_data.data(), patch_data.size(), reinterpret_cast<std::vector<Byte>::value_type*>(target_addr));
    //// zpl_memcopy(reinterpret_cast<std::vector<Byte>::value_type*>(target_addr), patch_data.data(), patch_data.size());
    //return target_addr + len;
}


void Mem::Utilites::InstallNopPatch(uint32_t dwAddress, int iSize)
{
	DWORD dwAddr = dwAddress;
	ProtectionInfo protectionInfo = Unprotect(dwAddr, iSize);
	memset((void *)dwAddr, NOP, iSize);
	Reprotect(protectionInfo);

    //ScopedProtect(dwAddress, iSize);
    //memset((void *)dwAddress, NOP, iSize);
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

#endif // MAFIA2_SDK_IMPLEMENTATION