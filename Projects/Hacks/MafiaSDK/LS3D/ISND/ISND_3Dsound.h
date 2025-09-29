#pragma once
#include "ISND_sound.h"

class ISND_3Dsound: public ISND_sound {
public:
    virtual LS3D_RESULT __stdcall Open(ISND_source* pSource, uint32_t uFlags) override;
    virtual LS3D_RESULT __stdcall Close() override;
    virtual ~ISND_3Dsound() {}
    virtual bool __stdcall Is3DInit();
    virtual LS3D_RESULT __stdcall SetPos(const S_vector& vPos);
    virtual LS3D_RESULT __stdcall SetDir(const S_vector& vDir);
    virtual LS3D_RESULT __stdcall SetVelocity(const S_vector& vVelocity);
    virtual LS3D_RESULT __stdcall SetDistances(float fNearDist, float fFarDist);
    virtual LS3D_RESULT __stdcall SetCone(float fInnerAngle, float fOuterAngle);
    virtual LS3D_RESULT __stdcall SetOutVolume(float fVolume);
};