#pragma once

#include <Visuals/I3D_object.h>

class I3D_lensflare : public I3D_object {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_lensflare() {}
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetSpace(bool) override;
    virtual void __stdcall Draw(bool);
};