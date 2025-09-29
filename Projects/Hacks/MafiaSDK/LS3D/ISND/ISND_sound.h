#pragma once
#include "LS3D.h"

class ISND_source;
class ISND_3Dsound;
struct IDirectSoundBuffer8;

enum ISND_sound_flags {
    SNDFL_NOVOLCHANGE = 0x2,
    SNDFL_NOPARAMCHANGE = 0x4,
    SNDFL_FREQCHANGE = 0x10,
    SNDFL_NOPLAYBACKSETTINGS = 0x20,
    SNDFL_NOPLAYBACKSETTINGS2 = 0x40,
    SNDFL_LOOP = 0x100,
    SNDFL_3DSOUND = 0x200,
    SNDFL_RESETPOS = 0x400,
    SNDFL_PRIORITY = 0x100000
};

class ISND_sound {
  public:
    virtual LS3D_RESULT __stdcall Release();
    virtual LS3D_RESULT __stdcall Open(ISND_source* pSource, uint32_t uFlags);
    virtual LS3D_RESULT __stdcall Close();
    virtual LS3D_RESULT __stdcall Play(uint32_t uFlags, uint32_t uPriority);
    virtual LS3D_RESULT __stdcall Play(ISND_source* pSource, uint32_t uFlags);
    virtual LS3D_RESULT __stdcall Stop();
    virtual uint32_t __stdcall GetCurrTime();
    virtual LS3D_RESULT __stdcall SetCurrTime(uint32_t uTime);
    virtual LS3D_RESULT __stdcall SetVolume(float fVolume);
    virtual LS3D_RESULT __stdcall SetVolumeLOG(float fVolume);
    virtual LS3D_RESULT __stdcall SetVolumeLOG(int32_t iVolume);
    virtual LS3D_RESULT __stdcall SetFrequency(float fFreq);
    virtual LS3D_RESULT __stdcall SetPanning(float fPan);
    virtual bool __stdcall IsPlaying();
    virtual ISND_source* __stdcall GetSource();
};