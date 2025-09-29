#pragma once
#include "LS3D.h"
#include "ISound.h"
#include "C_list.h"

class ISND_driver {
  public:
    virtual LS3D_RESULT __stdcall Init(ISND_INIT*);
    virtual LS3D_RESULT __stdcall Init2(HWND hWindow, uint32_t, uint32_t, uint32_t, uint32_t);
    virtual void __stdcall Close();
    virtual bool __stdcall IsInit();
    virtual LS3D_RESULT __stdcall InitBuffer(uint32_t, uint32_t, uint32_t);
    virtual LS3D_RESULT __stdcall SetVolume(float);
    virtual LS3D_RESULT __stdcall GetCaps(uint32_t* const);

    virtual LS3D_RESULT __stdcall CreateSource(ISND_source**);
    virtual ISND_source* __stdcall CreateSource();

    virtual LS3D_RESULT __stdcall CreateSound(ISND_sound**);
    virtual ISND_sound* __stdcall CreateSound();

    virtual LS3D_RESULT __stdcall Create3DSound(ISND_3Dsound**);
    virtual ISND_3Dsound* __stdcall Create3DSound();

    virtual LS3D_RESULT __stdcall CreateListener(ISND_3Dlistener**);
    virtual ISND_3Dlistener* __stdcall CreateListener();
};

extern "C" {
extern __declspec(dllexport) ISND_driver* ISndGetDriver();
}