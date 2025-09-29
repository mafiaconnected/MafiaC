#pragma once

#include <I3D/I3D_frame.h>

class I3D_camera : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_camera() {}
    virtual void UpdateV() override;
    virtual void __stdcall SetFOV(float fov);
    virtual void __stdcall SetNearFOV(float fov);
    virtual void __stdcall SetRange(float min, float max);
    virtual void __stdcall SetNearRange(float min, float max);
    virtual void __stdcall SetOrthoScale(float scale);
    virtual void __stdcall SetOrthogonal(bool enable);
    virtual void __stdcall SetAspectRatio(float ratio);

    inline float GetFOV() { return *(float*)((int)this + 320); }
    inline void GetRange(float& nearRange, float& farRange) { nearRange = *(float*)((int)this + 324);
        farRange = *(float*)((int)this + 328);
    }
};