#pragma once
#include "C_str.hpp"
#include "LS3D.h"
#include <d3d8.h>

enum ITEXTURE_CREATE_FLAGS {
    ITC_USE_ALPHA = 4
};

enum ITEXTURE_FLAGS { 
    IT_USE_TEXTURE_MANAGER = 0x40 
};

#pragma pack(push, 1)
struct TEXHeader {
    char szFormat[4];
    uint16_t uVersion;
    uint32_t uWidth;
    uint32_t uHeight;
    uint32_t uDummy[2];
};

struct ARGB_t {
    uint8_t uA;
    uint8_t uR;
    uint8_t uG;
    uint8_t uB;
};

struct RGB_t {
    uint8_t uR;
    uint8_t uG;
    uint8_t uB;
};

class ITexture {
public:
    /* public interface */
    virtual int32_t __stdcall Release();
    virtual LS3D_RESULT __stdcall Duplicate(ITexture* pSrc);
    virtual LS3D_RESULT __stdcall Create(const char* pSzDiffuseTexture, const char* pSzAlphaTexture, uint32_t uFlags, uint16_t uWidth, uint16_t uHeight, uint16_t uUnk);
    virtual bool __stdcall IsInit();
    virtual LS3D_RESULT __stdcall Close();
    virtual LS3D_RESULT __stdcall Reload();
    virtual void* __stdcall Lock(RECT* pTargetRect, uint32_t& uPitchOut, uint16_t uSurfaceLevel);
    virtual HRESULT __stdcall Unlock();
    virtual void __stdcall UpdateToVRAM(RECT* pTargetRect);
    virtual LS3D_RESULT __stdcall Preload();
    virtual bool __stdcall Fill(RECT* pTargetRect, uint32_t uColor, uint16_t uSurfaceLevel);
    virtual bool __stdcall Draw2DEx(RECT sRect, int32_t iX, int32_t iY, int32_t iW, int32_t iH);
    virtual bool __stdcall Draw2D(int32_t iX, int32_t iY);
    virtual bool __stdcall SaveToTGA(const char* pSzFilePath);
    virtual bool __stdcall IsAvailable();

    /* inlined accessors */
    inline IDirect3DTexture8* GetD3DTexture() {
        return m_pD3DTexture;
    }

    inline int32_t GetNumRefs() {
        return m_iNumRefs;
    }

    inline const C_string& GetDiffuseTexturePath() {
        return m_szDiffuseTexturePath;
    }

    inline const C_string& GetAlphaTexturePath() {
        return m_szAlphaTexturePath;
    }
    
private:
                                                    // 0  - 4    - vtable
    uint32_t m_uTextureUID;                         // 4  - 6    - texture uid (meybe its entire uint32_t)
    //char pad1[2];                                 // 6  - 8    - pad |
    IDirect3DTexture8* m_pD3DTexture;               // 8  - 12   - texture
    IDirect3DSurface8* m_pD3DLocedTextureSurface;   // 12 - 16   - locked texture surface
    bool m_bIsInited;                               // 16 - 17   - is inited
    char pad2_1[3];                                 // 17 - 20   - pad                 
    int32_t m_iNumRefs;                             // 20 - 24   - ref cnter
    C_string m_szDiffuseTexturePath;                // 24 - 28   - diff texture path
    const char* m_pSzDiffueTextureName;             // 28 - 32   - diff texture name
    C_string m_szAlphaTexturePath;                  // 32 - 36   - alpha texture path
    const char* m_pSzAlphaTextureName;              // 36 - 40   - alpha texture name
    uint16_t m_uWidth;                              // 40 - 42   - width
    uint16_t m_uHeight;                             // 42 - 44   - height
    D3DFORMAT m_eFormat;                            // 44 - 48   - format
    int32_t m_iFlags;                               // 48 - 52   - flags ( dont have to be flags actually :/ )
    int32_t m_iFlags2;                              // 52 - 56   - flags again ?
    uint32_t m_uBits;                               // 56 - 60
    uint8_t* m_pTextureData;                        // 60 - 64   - texture data
    uint16_t m_uLevels;                             // 64 - 66   - texture levels
    uint32_t m_uSize;                               // 66 - 70   - texture size in bytes
    uint32_t m_uUnk1;                               // 70 - 74   - unk
    uint16_t m_uUnk2;                               // 74 - 76   - unk2;
    friend class IGraph;
};
#pragma pack(pop)

/*
* 
*  TEXTURE MANAGER SECTION
*
*/

struct CREATED_TEXTURE {
    ITexture* m_pTexture;
    char pad[40];
};

struct MANAGED_TEXTURE {
    ITexture* m_pTexture;
    uint32_t m_uPathSize;
    D3DFORMAT m_eFormat;
    char* m_pStuff;
    char pad[4];
};
