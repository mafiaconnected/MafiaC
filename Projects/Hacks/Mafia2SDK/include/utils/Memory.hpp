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

struct ProtectionInfo
{
	DWORD dwAddress;
	DWORD dwOldProtection;
	int   iSize;
};

extern ProtectionInfo Unprotect(DWORD dwAddress, int iSize);

extern void InstallCallHook(DWORD address, DWORD function);

extern void Reprotect(ProtectionInfo protectionInfo);

extern DWORD GetFunctionAddress(LPCWSTR szLibrary, char * szFunction);

extern DWORD GetFunctionAddress(LPCWSTR szLibrary, unsigned int uOrdinal);