#pragma once
#include "common.h"
#include "ISound.h"
#include "LS3D.h"
#include "C_str.hpp"

struct IDirectSoundBuffer8;

class ISND_source {
  public:
    virtual int32_t __stdcall Release();
    virtual LS3D_RESULT __stdcall Open(const char* pSzFile, uint32_t uFlags);
    virtual void __stdcall Close();
    virtual bool __stdcall IsOpen();
    virtual uint32_t __stdcall GetPlayTime();
    virtual LS3D_RESULT __stdcall GetFormat(S_wave_format* pFormatOut);
    virtual LS3D_RESULT __stdcall Lock(void** pLockedData);
    virtual LS3D_RESULT __stdcall Unlock();
};