#pragma once
#include "I3D_frame.h"
#include "LS3D.h"

class I3D_dummy : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_dummy() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall Normalize(uint32_t unk);
};