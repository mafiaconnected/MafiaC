#pragma once
#include "I3D_math.h"
#include "LS3D.h"
#include "common.h"
#include <dsound.h>

class ISND_3Dlistener {
  public:
    virtual int32_t __stdcall AddRef();
    virtual int32_t __stdcall Release();
    virtual LS3D_RESULT __stdcall SetPos(const S_vector& vPos);
    virtual LS3D_RESULT __stdcall SetDir(const S_vector& vDirFront, const S_vector& vDirUp);
    virtual LS3D_RESULT __stdcall SetVelocity(const S_vector& vVelocity);
    virtual LS3D_RESULT __stdcall SetDistanceFactor(float fDistanceFactor);
    virtual LS3D_RESULT __stdcall SetDopplerFactor(float fDopplerFactor);
    virtual LS3D_RESULT __stdcall SetRollOffFactor(float fRolloffFactor);
    virtual LS3D_RESULT __stdcall SetEAXProperty(uint32_t eType, void* pData);
    virtual LS3D_RESULT __stdcall SetEAX3Property(uint32_t eType, void* pData, uint32_t uDataLen);
    virtual LS3D_RESULT __stdcall GetEAX3Property(uint32_t eType, void* pData, uint32_t uDataLen, uint32_t* pReturnDataLen);
    virtual LS3D_RESULT __stdcall Update();
};