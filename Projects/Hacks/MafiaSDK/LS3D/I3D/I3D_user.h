#pragma once

#include <I3D/I3D_frame.h>

class I3D_user : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_user() {}
};