#pragma once

#include <I3D/I3D_frame.h>

class I3D_joint : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_joint() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
};