#pragma once
#include "LS3D.h"
#include "IGraph.h"
#include "C_list.h"

struct OggVorbis_File;
struct tagRECT;
struct IDirectSoundNotify;
struct IDirectSoundBuffer8;

class CTextureRenderer;
class ITexture;

struct REGION_STRUCT {
    ITexture* pTexture;
    S_vertex_2d sVertices[4];
    RECT sRect;
};

enum ISHOW_FLAGS {
    ISHOW_PAUSED        = 0x10000000,
    ISHOW_PLAYING       = 0x20000000,
    ISHOW_STOP          = 0x40000000, 
    ISHOW_BUFFER_FILLED = 0x80000000
};

#define ISHOW_EVENT_PLAY 0
#define ISHOW_EVENT_STOP 1
#define ISHOW_EVENT_PAUSE 2

class IShow {
public:
    virtual int32_t __stdcall AddRef();
    virtual int32_t __stdcall Release();
    virtual bool __stdcall IsInit();
    virtual LS3D_RESULT __stdcall Open(char const* p_SzFilePath, uint32_t uFlags);
    virtual void __stdcall Close();
    virtual LS3D_RESULT __stdcall Play();
    virtual LS3D_RESULT __stdcall Stop();
    virtual LS3D_RESULT __stdcall Pause();
    virtual LS3D_RESULT __stdcall SetRect(tagRECT*);
    virtual LS3D_RESULT __stdcall GetRect(tagRECT*);
    virtual LS3D_RESULT __stdcall GetVideoRect(tagRECT*);
    virtual int32_t __stdcall GetEvent();
    virtual LS3D_RESULT __stdcall SetCurrentPos(uint32_t iPos);
    virtual LS3D_RESULT __stdcall GetCurrentPos(uint32_t* iPos);
    virtual LS3D_RESULT __stdcall SetVolume(float fVolume);
    virtual float __stdcall GetVolume();
    virtual int32_t __stdcall GetRegionCnt();
    virtual REGION_STRUCT* __stdcall GetRegions();
    virtual LS3D_RESULT __stdcall ProcessBinkFrame();
    virtual LS3D_RESULT __stdcall RenderCurrentFrame();
    virtual LS3D_RESULT __stdcall RenderVideoUntilKeyPressOrEnd();
    virtual int32_t __stdcall GetDecodedFrameNum();
    virtual LS3D_RESULT __stdcall AC3Open(char const* pSzFileName, uint32_t uFlags);
    virtual LS3D_RESULT __stdcall AC3Close();
    virtual LS3D_RESULT __stdcall AC3Play(bool);
    virtual bool __stdcall AC3IsPlaying();
    virtual LS3D_RESULT __stdcall AC3Stop();
    virtual LS3D_RESULT __stdcall AC3Pause();
    virtual LS3D_RESULT __stdcall AC3SetCurrentTime(int32_t);
    virtual LS3D_RESULT __stdcall AC3GetCurrentTime(int32_t*);
private:
    //void* m_pCoInstance;
    //char pad_1[20];
    void* m_pBink;
    bool m_bIsInited;
    //char pad_2[2];
    uint32_t m_iRefCnt;
    uint32_t m_uFlags;
    RECT m_sRect;
    float m_fVolume;
    C_list<REGION_STRUCT> m_Regions;
    CTextureRenderer* m_pTextureRenderer;
    uint32_t m_uDecodedFramesCnt;
    //char pad5[8];
    DWORD m_dwVorbisThreadId;
    HANDLE m_hVorbisThreadHandle;
    HANDLE m_hVorbisEventHandle;
    IDirectSoundNotify* m_pSoundNotify;
    IDirectSoundBuffer8* m_pSoundBuffer;
    uint32_t m_uPositionNotifyId;
    uint32_t m_uSoundBufferOffset;
    uint32_t m_uSoundBufferSize;
    bool m_bIsBufferBeingFilled;
    //char pad_4[3];
    OggVorbis_File* m_pVorbisFile;
    int32_t m_iVorbisDtaHandle;
};