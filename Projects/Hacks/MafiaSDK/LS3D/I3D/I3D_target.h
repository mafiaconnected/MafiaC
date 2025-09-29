#pragma once

#include <I3D/I3D_frame.h>

class I3D_target : public I3D_frame {
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_target() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall AddToTarget(I3D_frame* frame);
    virtual LS3D_RESULT __stdcall DeleteFromTarget(I3D_frame* frame);
    virtual void __stdcall SetTargetFlags(uint32_t flags);
};